#include "analysis/function/exprarray.hpp"
#include "analysis/wildcard.hpp"

#include <boost/functional/hash.hpp>

using namespace analysis;

std::size_t
exprarray::hash() const
{
  std::size_t seed = 0u;

  const expr
    *it = pbegin(),
    *en = pend();
  for( ; it != en; ++it )
    boost::hash_combine( seed, it->hash() );

  return seed;
}

util::cmp_t
exprarray::compare_same_type(const basic &o_) const
{
  const exprarray &o = static_cast<const exprarray&>(o_);

  const expr
    *i1 =   pbegin(),
    *e1 =   pend(),
    *i2 = o.pbegin(),
    *e2 = o.pend();

  // base case
  for(; i1 != e1 && i2 != e2; ++i1, ++i2)
  {
    util::cmp_t c = expr::compare( *i1, *i2 );
    if( c ) return c;
  }

  // at least one is empty
  if(i1 != e1) return 1;
  if(i2 != e2) return -1;

  // both are empty
  return 0;
}

bool
exprarray::has(const symbol &s) const
{
  const expr
    *it = pbegin(),
    *en = pend();
  for( ; it != en; ++it )
    if( it->has(s) )
      return true;

  return false;
}

void
exprarray::print_children(std::basic_ostream<char> &os) const
{
  const expr
    *it = pbegin(),
    *en = pend();

  if( it != en )
  {
    os << *it;
    while( ++it != en )
      os << ' ' << *it;
  }
}

bool
exprarray::match_same_type(const basic &o, match_state &mm) const
{
  const exprarray &pat = static_cast<const exprarray&>( o );

  typedef std::pair<expr,expr> pair_t;
  typedef std::pair<unsigned, pair_t> state_t;
  std::stack< state_t > backtrack;

  typedef std::list<pair_t> list_t;
  list_t ops;
  {
    const expr
      *i1 =     pbegin(), *e1 =     pend(),
      *i2 = pat.pbegin(), *e2 = pat.pend();

    if( std::distance( i1, e1 ) != std::distance( i2, e2 ) )
      return false;

    for( ; i1 != e1 && i2 != e2; ++i1, ++i2 )
      ops.push_back( std::make_pair( *i1, *i2 ) );
  }

  list_t::iterator
    it = ops.begin(),
    en = ops.end();
  for( ; it != en; ++it )
  {
    const std::size_t state = mm.size_state();

    if( it->first.match( it->second, mm ) )
    {
      if( state != mm.size_state() )
        backtrack.push( std::make_pair( state, *it ) );

      ops.erase( it );
      it = ops.begin(); en = ops.end();
      continue;
    }

    if( backtrack.empty() )
      return false;

    ops.push_back( backtrack.top().second );
    mm. pop_state( backtrack.top().first  );
    it = ops.begin(); en = ops.end();
  }

  return true;
}

expr
exprarray::subs(const exmap &map) const
{
  std::unique_ptr<exprarray> ret;

  { // map_children
    const expr
      *it = pbegin(),
      *en = pend();

    for( ; it != en; ++it )
    {
      const expr &orig = *it;
      const expr  imag = orig.subs( map );

      if( orig.get() == imag.get() )
        continue;

      ret.reset( static_cast<exprarray*>( this->clone() ) );

      expr
        *it2 = ret->pbegin(),
        *en2 = ret->pend();
      it2 += std::distance( it, en );

      for( ; it2 != en2; ++it2 )
        *it2 = it2->subs( map );

      goto changed;
    }
    return this->basic::subs_once(map);
  }

changed:
  const expr re = ret.release();

  if( RTTI_ID( re.get() ) == RTTI_ID( this ) )
    return re.get()->subs_once( map );

  return re;
}
