/*
 * range.hxx
 *
 *  Created on: 13 juin 2011
 *      Author: k1000
 */

#ifndef RANGE_HXX_
#define RANGE_HXX_

#include "stdlib.hxx"

namespace imperative {
namespace iterator {

namespace detail {

template<class> class range_iterator;

/*!
 * \struct range_iterator_type
 * \brief Range iterator base typing helper
 */
template<class Range>
struct range_iterator_type {
  //! \brief Iterator type
  typedef typename boost::range_iterator<const Range>::type iterator;

  //! \brief Value type
  typedef typename boost::range_value<const Range>::type value_type;

  //! \brief The proper iterator facade type
  typedef typename boost::iterator_facade<
    range_iterator<It,F>
  , value_type
  , boost::incrementable_traversal_tag
  > type;
};

/*!
 * \class range_iterator
 * \brief An iterator on a range
 */
template<class Range>
class range_iterator
: public range_iterator_type<Range>::type
, public operators::testable<range_iterator<Range> > {

  typedef range_iterator_type<Range> types;

  typedef typename types::value_type T;
  typedef typename types::iterator iter;

  iter it, end;

private:
  range_iterator();

public:
  /*!
   * \brief Copy constructor
   */
  inline
  range_iterator(const range_iterator &o)
  : it(o.it), end(o.end) {}
  /*!
   * \brief Assignment operator
   */
  inline range_iterator &
  operator=(const range_iterator &o) {
    it=o.it; end=o.end;
    return *this;
  }
  /*!
   * \brief Non-throwing swap
   */
  inline void
  swap(range_iterator &o) {
    std::swap(it, o.it);
    std::swap(end,o.end);
  }

public:
  /*!
   * \brief Contructor from a range
   * \param r : a range
   */
  inline explicit
  range_iterator(const Range &r)
  : it(boost::begin(r)), end(boost::end(r)) {}

  /*!
   * \brief Destructor
   */
  inline
  ~range_iterator() {}

private:
  friend class boost::iterator_core_access;

  inline void
  increment() {
    assert(it != end);
    ++it;
  }
  inline const T &
  dereference() const {
    assert(it != end);
    return *it;
  }

public:
  //! \brief Validity test
  inline bool
  valid() const
  { return it != end; }
};

} // namespace detail

/*!
 * \brief Front-end to the range_iterator class
 * \param r : a range
 * \return an iterator on this range
 */
template<class Range>
inline detail::range_iterator<Range>
iterate(const Range &r) {
  return detail::range_iterator<Range>(r);
}

}} // imperative::iterator


#endif /* RANGE_HXX_ */
