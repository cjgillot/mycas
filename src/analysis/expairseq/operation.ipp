#ifndef EXPAIRSEQ_OPERATION_IPP
#define EXPAIRSEQ_OPERATION_IPP

#include "container/unsafe_vector.hpp"
#include "analysis/expairseq/poly.hpp"

#include <functional>
#include <boost/range.hpp>

#include "util/foreach.hpp"
#include "util/assert.hpp"
#include "util/move.hpp" // move_ptr

namespace analysis {
namespace epseq {

namespace detail {

template<class epair>
struct sort_pred
: std::binary_function<bool, const epair&, const epair&>
{
  inline bool operator()( const epair &a, const epair &b ) const
  { return epair::compare( a, b ) < 0; }
};

template<class Iter>
std::size_t distance_ra_imp( const Iter &a, const Iter &b, boost::random_access_traversal_tag )
{ return b - a; }

template<class Iter>
std::size_t distance_ra_imp( const Iter &a, const Iter &b, boost::incrementable_traversal_tag )
{ return 0; }

} // namespace detail

template<class Iter>
std::size_t distance_ra( const Iter &a, const Iter &b )
{ return detail::distance_ra_imp( a, b, typename boost::iterator_traversal<Iter>::type() ); }

//! \brief Construct a vector from an unsorted range of \c epair
template<class epair, class Iter>
poly<epair>*
do_range_mutable(
    Iter beg
  , const Iter &end
  , std::size_t &hash
) {
  CONCEPT_ASSERT(( boost::Mutable_RandomAccessIterator<Iter> ));

  typedef poly<epair> vec_t;

  { // get canonical ordering
    detail::sort_pred< epair > p;
    std::sort( beg, end, p );
  }

  typename std::iterator_traits< Iter >::difference_type
    dist = std::distance( beg, end );

  ASSERT( dist >= 0 );

  util::scoped_ptr<vec_t> ret( new vec_t( std::size_t( dist ) ) );

  { // unique_copy and hash

    epair buf ( *beg );

    while( ++beg != end )
    {
      util::cmp_t c = epair::compare( buf, *beg );

      // collision : accumulate in buf
      if( c == 0 )
      {
        buf = buf + *beg;
        continue;
      }

      // no collision, commit and update
      if( ! buf.null() )
      {
        hash ^= buf.hash();
        ret->push_back( buf );
      }

      // load new value in buf
      buf = *beg;
    }

    // commit remaining
    if( ! buf.null() )
    {
      hash ^= buf.hash();
      ret->push_back( buf );
    }
  }

  ret->shrink();

  return ret->empty() ? nullptr : ret.release();
}

//! \brief Construct a vector from an unsorted range of \c epair
template<class epair, class Iter>
poly<epair>*
do_range_const(
    const Iter &beg
  , const Iter &end
  , std::size_t &hash
) {
  CONCEPT_ASSERT(( boost::InputIterator<Iter> ));

  typedef std::vector<epair> vec_t;
  vec_t tmp ( beg, end );

  return do_range_mutable<epair>( tmp.begin(), tmp.end(), hash );
}

}} // namespace analysis::epseq

#endif
