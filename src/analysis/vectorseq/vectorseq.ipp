#ifndef VECTORSEQ_IPP_
#define VECTORSEQ_IPP_

#include "analysis/expr.hpp"
#include "analysis/vectorseq/vectorseq.hpp"
#include "analysis/vectorseq/operation.ipp"

#include <boost/range.hpp>

#include "util/null.hpp"
#include "util/foreach.hpp"

namespace analysis {

#define VS vectorseq<I,M>

// ***** simple cdtor ***** //

template<class I, class M>
inline
VS::vectorseq(const number &n)
: vectorseq_base( n )
{}

template<class I, class M>
inline
VS::~vectorseq() {}

// ***** named constructors ***** //

//!\brief Monomial construction
template<class I, class M>
inline void
VS::construct_monomial( const M* p )
{
  vectorseq_base::construct_mon( EP::coef_hash( p ), EP::value_hash( p ) );
  poly().reset( new poly_t( 1, p ) );
}

// operation constructors

// add/sub/neg/sca -> .tpp

template<class I, class M>
inline void
VS::rehash()
{
  if( ! poly() )
    return;
  foreach( const M* p, *this )
    vectorseq_base::cons_hash( EP::coef_hash(p), EP::value_hash(p) );
}

/*!\name Monomial-range construction
  *
  * Given a number and an unsorted container of <tt>const Mono*</tt>,
  * construct a new vectorseq.
  *
  * Complexity : linearithmic in the range size
  *
  * \{
  */
//@{
template<class I, class M>
template<class Iter>
inline void
VS::construct_mono_range( const Iter &beg, const Iter &en )
{
  CONCEPT_ASSERT(( boost::InputIterator<Iter> ));
  if( beg != en )
    poly().reset( vseq::do_range_const<EP>( beg, en ) );
  rehash();
}

template<class I, class M>
template<class Iter>
inline void
VS::construct_sorted_mono_range( const Iter &beg, const Iter &en )
{
  CONCEPT_ASSERT(( boost::InputIterator<Iter> ));
  if( beg != en )
    poly().reset( vseq::do_range_sorted<EP>( beg, en ) );
  rehash();
}
//@}
/*!\}*/

/*!\brief Expression range construction
  *
  * Given a range of expression, construct an \c vectorseq from it
  *
  * \param beg the first iterator
  * \param end the last iterator
  * \param emono a unary functor converting expression to monomial
  * \param nadd a binary functor merging numbers
  */
template<class I, class M>
template<class Iter, class EM, class NA>
inline void
VS::construct_expr_range( Iter beg, const Iter &en, EM emono, NA nadd )
{
  CONCEPT_ASSERT(( boost::InputIterator<Iter> ));

  if( beg == en )
    return;

  container::ptr_vector<const M> seq;
  // quite a good start :
  //   exact if no numeric and no nested
  seq.reserve( vseq::distance_ra( beg, en ) );

  for(; beg != en; ++beg)
  {
    const expr &ex = *beg;

    if( ex.is_numerical() )
      nadd( coef(), ex.as_a< numerical >()->get() );

    else if( ex.is_a<I>() )
    { // needs flattening
      const vectorseq &eps = *ex.as_a<I>();

      nadd( coef(), eps.coef() );

      // ex has been evaluated
      ASSERT( ! eps.empty() );

      foreach( const M* p, eps )
        seq.push_back( p );
    }

    else
      seq.push_back( emono( ex ) );
  }

  std::sort( seq.begin(), seq.end(), sort_predicate() );
  construct_sorted_mono_range( seq.begin(), seq.end() );
}

// ***** comparison ***** //

// .tpp

#undef VS

} // namespace analysis

#endif // VECTORSEQ_IPP
