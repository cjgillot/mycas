#ifndef VECTORSEQ_IPP_
#define VECTORSEQ_IPP_

#include "analysis/numeric.hpp"
#include "analysis/vectorseq/vectorseq.hpp"
#include "analysis/vectorseq/operation.ipp"

#include <boost/range.hpp>

#include "util/null.hpp"
#include "util/foreach.hpp"

namespace analysis {

// ***** simple cdtor ***** //

template<class I, class M>
inline vectorseq<I,M>::
vectorseq(const number &n)
: m_coef( n )
, m_poly( /* null */ )
, m_hash( 0ul )
{}

template<class I, class M>
inline vectorseq<I,M>::
~vectorseq() {}

// ***** named constructors ***** //

//!\brief Monomial construction
template<class I, class M>
inline void vectorseq<I,M>::
construct_monomial( const M* p )
{
  const epair &ep = p;
  m_poly.reset( new poly_t(1, ep) );
  m_hash = ep.hash();
}

// operation constructors

// add/sub/neg/sca -> .tpp

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
inline void vectorseq<I,M>::
construct_mono_range( const Iter &beg, const Iter &en )
{
  CONCEPT_ASSERT(( boost::InputIterator<Iter> ));
  if( beg != en )
    m_poly.reset( epseq::do_range_const<epair>( beg, en, m_hash ) );
}

template<class I, class M>
template<class Iter>
inline void vectorseq<I,M>::
construct_mutable_mono_range( const Iter &beg, const Iter &en )
{
  CONCEPT_ASSERT(( boost::Mutable_RandomAccessIterator<Iter> ));
  if( beg != en )
    m_poly.reset( epseq::do_range_mutable<epair>( beg, en, m_hash ) );
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
inline void vectorseq<I,M>::
construct_expr_range( Iter beg, const Iter &en, EM emono, NA nadd )
{
  CONCEPT_ASSERT(( boost::InputIterator<Iter> ));

  if( beg == en )
    return;

  std::vector<epair> seq;
  // quite a good start :
  //   exact if no numeric and no nested
  seq.reserve( epseq::distance_ra( beg, en ) );

  for(; beg != en; ++beg)
  {
    const expr &ex = *beg;

    if( ex.is_numeric() )
      nadd( m_coef, ex.as_a<numeric>() );

    else if( ex.is_a<I>() )
    { // needs flattening
      const vectorseq &eps = *ex.as_a<I>();

      nadd( m_coef, eps.coef() );

      // ex has been evaluated
      ASSERT( ! eps.empty() );

      foreach( const epair &ep, *eps.m_poly )
        seq.push_back( ep );
    }

    else
      seq.push_back( emono( ex ) );
  }

  construct_mutable_mono_range( seq.begin(), seq.end() );
}

// ***** comparison ***** //

// .tpp

// ***** range operation ***** //

/*!\brief Function mapping
  *
  * \param f a unary function mapping <tt>const Mono*</tt> to \c expr
  *
  * \return \c nullptr if nothing has changed, the mapped vector else
  *
  * Nothing has changed case includes :
  * - empty polynomial
  * - pointer equality of original \c epair and result \c expr
  */
template<class I, class M>
template<class F>
inline std::vector<expr>* vectorseq<I,M>::
map_children( F f ) const
{
  if( ! m_poly )
    return nullptr;

  raw_const_iterator
    it = m_poly->begin(),
    en = m_poly->end();

  for( ; it != en; ++it )
  {
    // not expr to avoid automatic evaluation
    const ptr<const basic> &r = f( it->get() );

    if( r.get() != it->get() )
    {
      // something has changed
      // create a new vector
      typedef std::vector< expr > vec_t;
      util::scoped_ptr< vec_t > retp ( new vec_t );
      vec_t &ret = *retp;
      ret.reserve( m_poly->size() );

      // unchanged terms
      for( raw_const_iterator i2 = m_poly->begin(); i2 != it; ++i2 )
        ret.push_back( i2->get() );

      // current term
      ret.push_back( r );
      ++it;

      // remaining terms
      for( ; it != en; ++it )
        ret.push_back( f( it->get() ) );

      return retp.release();
    }
  }

  // nothing has changed
  return nullptr;
}


} // namespace analysis

#endif // VECTORSEQ_IPP
