#include "analysis/basic.hpp"
#include "analysis/expr.hpp"

#include "analysis/expr.ipp"

using namespace analysis;

expr basic::subs_once(const exmap &map) const
{
  exmap::const_iterator end = map.end()
      , it = map.find( this );

  if( it != end )
    return it->second.subs( map );

  match_state s;

  for( it =  map.begin(); it != end; ++it )
    if( match( it->first, match ) )
      return it->second.subs( s.as_map() );
}

static ptr<const basic>   sum_subs(const   sum &, const exmap &);
static ptr<const basic>  prod_subs(const  prod &, const exmap &);
static ptr<const basic> power_subs(const power &, const exmap &);

namespace {

template<class Mono>
struct subser
: std::unary_function<ptr<const basic>, const Mono*>
{
  typedef ptr<const basic> result_type;

  subser( const exmap &m )
  : m_map( &m ) {}

  inline ptr<const basic> operator()( const Mono* p ) const
  {
    result_type ret = subs_fnc( p );
    exmap::const_iterator end = m_map->end(), it = end;

    for(;;)
    {
      it = m_map->find( ret );

      if( it == end )
        return ret;

      ret = it->second;
    }
  }

private:
  inline ptr<const basic> subs_fnc( const Mono* ) const;

private:
  const exmap* m_map;
};

template<>
inline ptr<const basic> subser<power>::subs_fnc( const power* p ) const
{
  return power_subs( *p, *m_map );
}

template<>
inline ptr<const basic> subser<prod>::operator()( const prod* p ) const
{
  return prod_subs( *p, *m_map );
}

}

#include <vector>
#include "util/move.hpp"

#include "analysis/expairseq.ipp"

#include "analysis/sum.hpp"
#include "analysis/prod.hpp"

#include "util/functor.hpp"

namespace {

template<class Eps, class Mono, class F>
ptr<const Eps> eps_subs(const Eps &self, const exmap &map, F nadd)
{
  util::scoped_ptr< std::vector<expr> > children ( self.map_children( subser<Mono>( map ) ) );

  if( ! children )
    return &self;

  ASSERT( ! children->empty() );

  ptr< Eps > ret
    = Eps::from_expr_range( children->begin(), children->end() );

  nadd( ret->coef(), self.coef() );

  return ret;
}

}

ptr<const basic>   sum_subs( const sum &self, const exmap &map )
{ return eps_subs< sum,  prod>( self, map, functor::plus_eq<number>() ); }

ptr<const basic>  prod_subs( const prod &self, const exmap &map )
{ return eps_subs<prod, power>( self, map, functor::multiplies_eq<number>() ); }

ptr<const basic> power_subs( const power &self, const exmap &map )
{
  const expr &b = self.base().subs( map );
  const expr &e = self.expo().subs( map );

  if( ( b.get() != self.base().get() )
   || ( e.get() != self.expo().get() ) )
    return b.pow( e ).subs( map );

  return &self;
}

expr   sum::subs(const exmap &map) const
{ return   sum_subs( *this, map )->basic::subs( map ); }

expr  prod::subs(const exmap &map) const
{ return  prod_subs( *this, map )->basic::subs( map ); }

expr power::subs(const exmap &map) const
{ return power_subs( *this, map )->basic::subs( map ); }
