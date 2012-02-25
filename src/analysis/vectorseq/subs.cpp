#include "analysis/expr.hpp"

#include "analysis/wildcard.hpp"

#include <vector>
#include "util/move.hpp"

#include "analysis/vectorseq/vectorseq.ipp"

#include "analysis/vectorseq.hpp"

#include "util/functor.hpp"

using namespace analysis;

template<class Mono>
struct subser
: std::unary_function<const basic*, expr>
{
private:
  const exmap* m_map;

public:
  subser( const exmap &m )
  : m_map( &m ) {}

  inline expr
  operator()( const basic* p ) const
  {
    expr ret = subs_fnc( static_cast<const Mono*>(p) );
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
  inline expr
  subs_fnc( const Mono* ) const;
};

template<>
inline expr subser<power>::subs_fnc( const power* p ) const
{
  return p->power::subs( *m_map );
}

template<>
inline expr subser<prod>::subs_fnc( const prod* p ) const
{
  return p->prod::subs( *m_map );
}

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

expr   sum::subs( const exmap &map ) const
{
  return eps_subs< sum,  prod>( *this, map, functor::plus_eq<number>() )
    ->subs_once( map );
}

expr  prod::subs( const exmap &map ) const
{
  return eps_subs<prod, power>( *this, map, functor::multiplies_eq<number>() )
    ->subs_once( map );
}

expr power::subs( const exmap &map ) const
{
  const expr &b = base().subs( map );
  const expr &e = expo().subs( map );

  if( ( b.get() != base().get() )
   || ( e.get() != expo().get() ) )
    return expr::pow( b, e ).get()->subs_once( map );

  return subs_once( map );
}
