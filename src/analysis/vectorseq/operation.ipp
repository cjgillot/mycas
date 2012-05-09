#ifndef EXPAIRSEQ_OPERATION_IPP
#define EXPAIRSEQ_OPERATION_IPP

#include "container/unsafe_vector.hpp"
#include "analysis/vectorseq/poly.hpp"

#include <functional>
#include <boost/range.hpp>

#include "util/assert.hpp"

namespace analysis {
namespace vseq {

namespace detail {

template<class Iter>
std::size_t distance_ra_imp( const Iter &a, const Iter &b, boost::random_access_traversal_tag )
{ return b - a; }

template<class Iter>
std::size_t distance_ra_imp( const Iter &a, const Iter &b, boost::incrementable_traversal_tag )
{ return 0; }

template<class EP, class I1, class I2>
bool is_sorted_epair(I1&& first, I2&& last )
{
  typedef typename EP::monomial_type M;

  return std::is_sorted(
    std::forward<I1>(first),
    std::forward<I2>(last),
    [](const M* a, const M* b) { return EP::compare(a,b) < 0; }
  );
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

  const auto dist = std::distance( beg, end );

  ASSERT( dist >= 0 );

  std::unique_ptr<vec_t> ret { new vec_t{(std::size_t)dist} };

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
template<class EP, class Iter>
poly<typename EP::monomial_type const>
do_range_const(Iter&& beg, Iter&& end)
{
  CONCEPT_ASSERT(( boost::InputIterator<Iter> ));

  typedef std::vector<typename EP::const_pointer> vec_t;
  vec_t tmp { std::forward<Iter>(beg), std::forward<Iter>(end) };

  typedef sort_pred<EP> p_t;
  std::sort( tmp.begin(), tmp.end(), p_t() );

  return do_range_sorted<EP>( tmp.begin(), tmp.end() );
}

}} // namespace analysis::vseq

#endif
