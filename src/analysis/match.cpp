#include "analysis/match.hpp"

#include "analysis/expr.hpp"

using namespace analysis;
using match_detail::wildcard_;

bool matched_wild_p( const wildcard_ &pattern, match_state &mm )
{
  unsigned pid = pattern.id();
  match_state::const_iterator it = mm.find( pid );

  return ( it != mm.end() && it->first == pid );
}

bool match_wild( const basic &self, const wildcard_ &pattern, match_state &mm )
{
  unsigned pid = pattern.id();
  {
    match_state::const_iterator it = mm.find( pid );

    if( it != mm.end() && it->first == pid )
      // already matched somewhere
      return expr::compare( &self, it->second ) == 0;
  }

  mm.push_state( pid, &self );
  return true;
}

bool basic::match(const expr &e, match_state &mm) const
{
  if( e.is_a< wildcard_ >() )
    return match_wild( *this, *e.as_a< wildcard_ >(), mm );

  if( RTTI_ID( this ) != RTTI_ID( e.get() ) )
    return false;

  return match_same_type( *e.get(), mm );
}

bool basic::match_same_type(const basic &o, match_state &) const
{
  return compare( *this, o ) == 0;
}

// sum
#include "analysis/vectorseq.hpp"

#include "container/unsafe_vector.hpp"

namespace {

struct sum_generator
{
  template< class Iter >
  static sum* create( const number &n, const Iter &b, const Iter &e )
  {
    util::scoped_ptr<sum> r ( sum::from_expr_range( b, e ) );
    r->coef() += n;
    return r.release();
  }
};

struct prod_generator
{
  template< class Iter >
  static prod* create( const number &n, const Iter &b, const Iter &e )
  {
    util::scoped_ptr<prod> r ( prod::from_expr_range( b, e ) );
    r->coef() *= n;
    return r.release();
  }
};

template<class Iter>
struct back_type
{
  Iter it;
  expr ex;
  size_t sz;

  back_type( const Iter &i, const expr &e, size_t s )
  : it(i), ex(e), sz(s) {}
};

template<class I, class F>
bool match_same_type_eps( const number &c, bool nullc, const I &self, const I &pattern, match_state &mm )
{
  if( &self == &pattern )
    return true;

  bool has_global_wild = false;
  ptr< const wildcard_ > global_wild;

  typedef typename std::deque< expr >::iterator listit_t;
  std::deque< expr > items ( self.begin(), self.end() );

  typedef typename I::const_iterator iterator_t;
  iterator_t pit = pattern.begin();
  const iterator_t pen = pattern.end();

  typedef back_type<iterator_t> back_t;
  std::stack< back_t > backtrack;

  for( ; pit != pen; ++pit )
  {
    const expr &pe = *pit;
    if( ! has_global_wild && pe.is_a< wildcard_ >() )
    {
      const wildcard_* w = pe.as_a< wildcard_ >();

      if( ! matched_wild_p( *w, mm ) )
      {
        has_global_wild = true;
        global_wild.reset( w );
//         std::cerr << "global : " << pe << std::endl;
        continue;
      }
    }

    listit_t sit = items.begin();
    const listit_t sen = items.end();
    for( ; sit != sen; ++sit )
    {
//       std::cerr << "try : " << *sit << " -- " << pe << std::endl;
      std::size_t back_sz = mm.size_state();
      if( sit->match( pe, mm ) )
      {
        // if some wildcard has been unified
        if( back_sz != mm.size_state() )
          backtrack.push( back_t( pit, *sit, back_sz ) );

        items.erase( sit );
        goto next_p;
      }
    }

    if( backtrack.empty() )
      // failure
      return false;

    {
      const back_t &b = backtrack.top();
      pit = b.it;
      items.push_front( b.ex );
      mm.pop_state( b.sz );

      backtrack.pop();
    }

  next_p:
    ;
  }

  if( ! has_global_wild )
    return items.empty() && nullc;

  expr ret = F::create( c, items.begin(), items.end() );
  match_wild( *ret.get(), *global_wild, mm );

  return true;
}

}

bool sum::match_same_type(const basic &o_, match_state &mm) const
{
  const sum &o = static_cast<const sum&>(o_);
  const number &c = coef() - o.coef();
  return match_same_type_eps<sum, sum_generator>(
    c, c.null()
  , *this
  , o
  , mm );
}

bool prod::match_same_type(const basic &o_, match_state &mm) const
{
  const prod &o = static_cast<const prod&>(o_);
  const number &c = coef() / o.coef();
  return match_same_type_eps<prod, prod_generator>(
    c, c.unit()
  , *this
  , o
  , mm );
}

bool power::match_same_type(const basic &o_, match_state &mm) const
{
  const power &pat = static_cast<const power&>( o_ );
  return m_base.match( pat.m_base, mm ) && m_expo.match( pat.m_expo, mm );
}
