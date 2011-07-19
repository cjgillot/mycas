/*
 * map.hpp
 *
 *  Created on: 11 juin 2011
 *      Author: k1000
 */

#ifndef ITERATOR_MAP_HPP_
#define ITERATOR_MAP_HPP_

#include "stdlib.hpp"
#include<boost/type_traits/remove_reference.hpp>

namespace imperative {
namespace iterator {

namespace detail {

template<class,class> class map_iterator;

/*!
 * \struct map_iterator_type
 * \brief Map iterator base typing helper
 */
template<class It, class F>
class map_iterator_type {
  typedef typename boost::iterator_value<It>::type iter_ref;

  typedef typename boost::result_of<F(iter_ref)>::type reference;

public:
  //! \brief The proper iterator facade type
  typedef typename boost::iterator_facade<
    map_iterator<It,F>
  , typename boost::remove_reference<reference>::type
  , boost::incrementable_traversal_tag
  , reference
  > type;
};

/*!
 * \class map_iterator
 * \brief Iterator adapter applying a function to an iterator
 */
template<class It, class F>
class map_iterator
: public map_iterator_type<It, F>::type
, public operators::testable<map_iterator<It, F> >
{
  typedef typename map_iterator_type<It, F>::type super_t;
  typedef typename super_t::reference reference;

  It iter;
  F func;

private:
  map_iterator();

public:
  /*!
   * \brief Copy constructor
   */
  inline
  map_iterator(const map_iterator &o)
  : iter(o.iter), func(o.func) {}
  /*!
   * \brief Assignment operator
   */
  inline map_iterator &
  operator=(const map_iterator &o) {
    iter=o.iter; func=o.func;
    return *this;
  }
  /*!
   * \brief Non-throwing swap
   */
  inline void
  swap(map_iterator &o) {
    std::swap(iter, o.iter);
    std::swap(func, o.func);
  }

public:
  /*!
   * \brief Contructor from iterator and function
   */
  inline
  map_iterator(const It &i, const F &f)
  : iter(i), func(f) {}

  /*!
   * \brief Destructor
   */
  inline
  ~map_iterator() {}

private:
  friend class boost::iterator_core_access;

  inline reference
  dereference() const
  { return func(*iter); }
  inline void
  increment()
  { ++iter; }

public:
  //! \brief Validity test
  inline bool
  valid() const
  { return iter; }
};

} // namespace detail

/*!
 * \struct map_type
 * \brief Map iterator typing helper
 */
template<class It, class F>
struct map_type {
  //! \brief Iterator type
  typedef detail::map_iterator<It,F> iterator;

  //! \brief Value type
  typedef typename iterator::value_type value_type;

  //! \brief Reference type
  typedef typename iterator::reference  reference;
};

/*!
 * \brief Front-end to the map_iterator class
 * \param it : a base iterator
 * \param f : a function
 * \return a map_iterator object
 */
template<class It, class F>
inline detail::map_iterator<It, F>
map(const It &it, const F &f) {
  return detail::map_iterator<It,F>(it, f);
}

}} // imperative::iterator


#endif /* ITERATOR_MAP_HPP_ */
