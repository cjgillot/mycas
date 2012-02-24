#ifndef ANALYSIS_FUNCTION_EXPRSEQ_IPP
#define ANALYSIS_FUNCTION_EXPRSEQ_IPP

#include "analysis/function/exprseq.hpp"

#include "util/foreach.hpp"
#include "util/assert.hpp"
#include "util/move.hpp"

#include "analysis/match_state.hpp"

#include <boost/functional/hash.hpp>

namespace analysis {

template<class C>
std::size_t
exprseq<C, false>::hash() const
{
  std::size_t seed = 0u;
  foreach( const expr &a, m_container )
    boost::hash_combine( seed, a.hash() );
  return seed;
}

template<class C>
util::cmp_t
exprseq<C, false>::compare_same_type(const basic &o_) const
{
  const exprseq &o = static_cast<const exprseq&>(o_);

  return util::compare_range( m_container, o.m_container, expr::compare );
}

template<class C>
bool
exprseq<C, false>::has(const symbol &s) const
{
  foreach(const expr &e, m_container)
    if( e.has(s) )
      return true;
  return false;
}

template<class C>
void
exprseq<C, false>::print_children(std::basic_ostream<char> &os) const
{
  if( size() > 0 )
  {
    const_iterator
      it = begin()
    , en = end();

    os << *it;
    while( ++it != en )
      os << ' ' << *it;
  }
}

template<class C>
bool
exprseq<C, false>::match_same_type(const basic &o, match_state &mm) const
{
  const exprseq &pat = static_cast<const exprseq&>( o );

  if( size() != pat.size() )
    return false;

  std::stack< std::pair<unsigned, std::pair<expr,expr> > > backtrack;

  typedef std::list< std::pair<expr, expr> > list_t;
  list_t ops;
  {
    const_iterator i1 =     begin(), e1 = end(),
                   i2 = pat.begin(), e2 = pat.end();
    for( ; i1 != e1 && i2 != e2; ++i1, ++i2 )
      ops.push_back( std::make_pair( *i1, *i2 ) );
  }


  for( list_t::iterator it = ops.begin(), en = ops.end(); it != en; ++it )
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
    mm.pop_state(  backtrack.top().first );
    it = ops.begin(); en = ops.end();
  }

  return true;
}

template<class C>
expr
exprseq<C, false>::subs(const exmap &map) const
{
  util::scoped_ptr<exprseq> ret;

  size_t index = 0;
  const_iterator it = begin(), en = end();
  for( ; it != en; ++it, ++index )
  {
    const expr &orig = *it;
    const expr &imag = orig.subs( map );

    if( orig.get() == imag.get() )
      continue;

    ret.reset( static_cast<exprseq*>( this->clone() ) );
    iterator it2 = ret->begin(); en = ret->end();
    for( ; index != 0; ++it2, --index );
    for( ; it2 != en; ++it2 )
      *it2 = it2->subs( map );
    break;
  }

  if( !ret )
    return this->basic::subs_once(map);

  const expr re = ret.release();

  if( RTTI_ID( re.get() ) == RTTI_ID( this ) )
    return re.get()->subs_once( map );

  return re;
}

} // namespace analysis

#endif
