#ifndef ANALYSIS_FUNCTION_EXPRSEQ_IPP
#define ANALYSIS_FUNCTION_EXPRSEQ_IPP

#include "analysis/function/exprseq.hpp"

#include "algebra/compare.hpp"

#include "util/foreach.hpp"
#include "util/assert.hpp"

namespace analysis {

template<class C>
std::size_t exprseq<C>::hash() const
{
  std::size_t seed = 0u;
  foreach( const expr &a, m_container )
    boost::hash_combine( seed, a.hash() );
  return seed;
}

template<class C>
util::cmp_t exprseq<C>::compare_same_type(const basic &o_) const
{
  const exprseq &o = static_cast<const exprseq&>(o_);

  return algebra::range_compare( m_container, o.m_container, expr::compare );
}

template<class C>
bool exprseq<C>::has(const symbol &s) const
{
  foreach(const expr &e, m_container)
    if( e.has(s) )
      return true;
}

template<class C>
void exprseq<C>::print_children(std::basic_ostream<char> &os) const
{
  os << '(';
  if( size() > 0 )
  {
    const_iterator
      it = begin()
    , en = end();

    os << *it;
    while( ++it != en )
      os << ' ' << *it;
  }
  os << ')';
}

template<class C>
expr exprseq<C>::match(const match_state &map) const
{
  util::scoped_ptr<exprseq<C> > ret;

  size_t index = 0;
  const_iterator it = begin(), en = end();
  for( ; it != en; ++it, ++index )
  {
    const expr &orig = *it;
    const expr &imag = orig.subs( map );

    if( orig.get() == imag.get() )
      continue;

    ret.reset( this->clone() );
    iterator it2 = ret->begin(); en = ret->end();
    for( ; index != 0; ++it2, --index );
    for( ; it2 != en; ++it2 )
      *it2 = it2->subs( map );
    break;
  }

  if( !ret )
    return this->subs_once(map);

  const expr &re = ret.release();
  if( dynamic_cast<exprseq>(re.get()) )
    return re.get()->subs_once( map );

  return re;
}

template<class C>
expr exprseq<C>::subs(const exmap &map) const
{
  util::scoped_ptr<exprseq<C> > ret;

  size_t index = 0;
  const_iterator it = begin(), en = end();
  for( ; it != en; ++it, ++index )
  {
    const expr &orig = *it;
    const expr &imag = orig.subs( map );

    if( orig.get() == imag.get() )
      continue;

    ret.reset( this->clone() );
    iterator it2 = ret->begin(); en = ret->end();
    for( ; index != 0; ++it2, --index );
    for( ; it2 != en; ++it2 )
      *it2 = it2->subs( map );
    break;
  }

  if( !ret )
    return this->subs_once(map);

  const expr &re = ret.release();
  // TODO get rid of dynamic_cast here
  if( dynamic_cast<exprseq>(re.get()) )
    return re.get()->subs_once( map );

  return re;
}


} // namespace analysis

#endif
