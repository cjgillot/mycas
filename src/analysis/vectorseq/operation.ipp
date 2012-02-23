#ifndef EXPAIRSEQ_OPERATION_IPP
#define EXPAIRSEQ_OPERATION_IPP

#include "container/unsafe_vector.hpp"
#include "analysis/vectorseq/poly.hpp"

#include <functional>
#include <boost/range.hpp>

#include "util/foreach.hpp"
#include "util/assert.hpp"
#include "util/move.hpp" // move_ptr

namespace analysis {
namespace vseq {

namespace detail {

template<class Iter>
std::size_t distance_ra_imp( const Iter &a, const Iter &b, boost::random_access_traversal_tag )
{ return b - a; }

template<class Iter>
std::size_t distance_ra_imp( const Iter &a, const Iter &b, boost::incrementable_traversal_tag )
{ return 0; }

template<class EP, class Iter>
bool is_sorted_epair( Iter first, const Iter &last )
{
  if( first == last )
    return true;

  for( Iter next = first; ++next != last; first = next )
    if( EP::compare( *first, *next ) > 0 )
      return false;

  return true;
}

} // namespace detail

template<class Iter>
std::size_t distance_ra( const Iter &a, const Iter &b )
{ return detail::distance_ra_imp( a, b, typename boost::iterator_traversal<Iter>::type() ); }

//! \brief Construct a vector from a sorted range of \c epair
template<class EP, class Iter>
poly<const basic>*
do_range_sorted( Iter beg, const Iter &end )
{
  CONCEPT_ASSERT(( boost::InputIterator<Iter> ));

  typedef typename EP::monomial_type M;
  typedef poly<const basic> vec_t;

  // test range is sorted
  ASSERT( detail::is_sorted_epair<EP>( beg, end ) );

  typename std::iterator_traits< Iter >::difference_type
    dist = std::distance( beg, end );

  ASSERT( dist >= 0 );

  util::scoped_ptr<vec_t> ret( new vec_t( std::size_t( dist ) ) );

  { // unique_copy and hash

    ptr<const M> buf ( *beg );

    while( ++beg != end )
    {
      util::cmp_t c = EP::compare( buf.get(), *beg );

      // collision : accumulate in buf
      if( c == 0 )
      {
        buf = EP::add( buf.get(), *beg );
        continue;
      }

      // no collision, commit and update
      if( ! EP::null( buf.get() ) )
        ret->push_back( buf.get() );

      // load new value in buf
      buf.reset( *beg );
    }

    // commit remaining
    if( ! EP::null( buf.get() ) )
      ret->push_back( buf.get() );
  }

  ret->shrink();

  return ret->empty() ? nullptr : ret.release();
}

//! \brief Construct a vector from an unsorted range of \c epair
template<class epair, class M, class Iter>
poly<M>*
do_range_const( const Iter &beg, const Iter &end )
{
  CONCEPT_ASSERT(( boost::InputIterator<Iter> ));

  typedef std::vector<M> vec_t;
  vec_t tmp ( beg, end );

  typedef sort_pred<epair,M> p_t;
  std::sort( tmp.begin(), tmp.end(), p_t() );

  return do_range_sorted<epair>( tmp.begin(), tmp.end() );
}

}} // namespace analysis::vseq

#endif
