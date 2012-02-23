#ifndef EXPAIRSEQ_TPP_
#define EXPAIRSEQ_TPP_

#include "analysis/expr.hpp"
#include "analysis/vectorseq/vectorseq.hpp"

#include "analysis/vectorseq/vectorseq.ipp"

#include "analysis/vectorseq/poly.tpp"
#include "analysis/vectorseq/operation.tpp"

#include <boost/range/algorithm/for_each.hpp>

#include "util/null.hpp"
#include "util/foreach.hpp"

namespace analysis {

#define VS vectorseq<I,M>

// ***** simple cdtor ***** //

// -> .ipp

// ***** named constructors ***** //

// -> .ipp

// operation constructors
/*!\name Operation constructors
  *
  * Given two \c vectorseq, these constructors build
  * a merge depending on the call tag.
  *
  * Complexity : linear
  *   (in operations and comparisons)
  *
  * \{
  */
//@{
template<class I, class M>
void
VS::construct_add( const vectorseq &a, const vectorseq &b )
{
  vectorseq_base::construct_add( a, b );

  if( ! b.poly() )
    poly() = a.poly();

  else if( ! a.poly() )
    poly() = b.poly();

  else
    poly().reset( vseq::do_add<EP>( *a.poly(), *b.poly() ) );
}

template<class I, class M>
void
VS::construct_sub( const vectorseq &a, const vectorseq &b )
{
  vectorseq_base::construct_sub( a, b );

  if( ! b.poly() )
    poly() = a.poly();

  else if( ! a.poly() )
    poly().reset( vseq::do_neg<EP>( *b.poly() ) );

  else
    poly().reset( vseq::do_sub<EP>( *a.poly(), *b.poly() ) );
}

template<class I, class M>
void
VS::construct_neg( const vectorseq &a )
{
  vectorseq_base::construct_neg( a );

  if(a.poly())
    poly().reset( vseq::do_neg<EP>( *a.poly() ) );
}

template<class I, class M>
void
VS::construct_sca( const number &n, const vectorseq &a )
{
  vectorseq_base::construct_sca( n, a );

  if( a.poly() )
    poly().reset( vseq::do_sca<EP>( *a.poly(), n ) );
}
//@}
/*!\}*/

// range -> .ipp

// ***** comparison ***** //
template<class I, class M>
util::cmp_t
VS::partial_compare(const vectorseq &o) const
{
  util::cmp_t c = vectorseq_base::partial_compare( o );
  if( c ) return c;

  // trivial case
  if( poly().get() == o.poly().get() )
    return 0;

  if( ! poly() )
    return -1; // since poly() != o.poly()
  else if( ! o.poly() )
    return +1;

  const poly_t
    &a = *  poly()
  , &b = *o.poly();

  // compare sizes
  // doing like this saves loops,
  // since size differences are very common
  std::size_t
    d1 = a.size()
  , d2 = b.size();

  c = util::compare( d1, d2 );
  if(c) return c;

  // lexicographical comparison now
  const_iterator
    i1 = this->begin(), i2 = o.begin();

  for(; d1 != 0; --d1)
  {
    c = EP::deep_compare( *i1, *i2 );
    if(c) return c;
  }

  // now, we know we have equality -> unify
  util::unify_ptr(
    const_cast<vectorseq&>(*this).poly()
  , const_cast<vectorseq&>(o)    .poly()
  );
  return 0;
}

template<class I, class M>
util::cmp_t
VS::compare_same_type(const basic &o_) const
{
  const vectorseq &o = static_cast<const vectorseq&>( o_ );
  util::cmp_t c = vectorseq_base::compare_same_type( o );
  if(c) return c;
  return partial_compare(o);
}

#undef VS

} // namespace analysis

#endif // EXPAIRSEQ_IPP
