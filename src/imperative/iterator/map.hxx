/*
 * map.hxx
 *
 *  Created on: 11 juin 2011
 *      Author: k1000
 */

#ifndef ITERATOR_MAP_HXX_
#define ITERATOR_MAP_HXX_

#include "stdlib.hxx"
#include<boost/type_traits/remove_reference.hpp>

namespace imperative {
namespace iterator {

namespace detail {

template<class,class> class map_iterator;

template<class Self, class It, class F>
struct map_iterator_type {
  typedef typename boost::iterator_value<It>::type iter_ref;

  typedef typename boost::result_of<F(iter_ref)>::type reference;

  typedef boost::iterator_facade<
    map_iterator<It,F>
  , boost::remove_reference<reference>::type
  , boost::incrementable_traversal_tag
  , reference
  > type;
};

template<class It, class F>
class map_iterator
: public map_iterator_type<F, It>::type
{
  typedef typename map_iterator_type<F, It>::type super_t;
  typedef typename super_t::reference reference;

  It iter;
  F func;

private:
  map_iterator();

public:
  inline
  map_iterator(const map_iterator &o)
  : iter(o.iter), func(o.func) {}
  inline map_iterator &
  operator=(const map_iterator &o) {
    iter=o.iter; func=o.func;
    return *this;
  }

  inline void
  swap(map_iterator &o) {
    std::swap(iter, o.iter);
    std::swap(func, o.func);
  }

  inline
  map_iterator(const It &s, const F &f)
  : iter(s), func(f) {}

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

private:
  typedef util::safe_bool<void(map_iterator::*)(map_iterator&)> safe_bool;
public:
  typedef typename safe_bool::unspecified_bool_type bool_t;

  inline
  operator bool_t() const {
    return safe_bool::to_unspecified_bool(
        iter,
        &map_iterator::swap
    );
  }
  inline bool
  operator!() const
  { return !iter; }
};

} // namespace detail

template<class It, class F>
struct map_type {
  typedef detail::map_iterator<It,F> iterator;
  typedef typename iterator::value_type value_type;
  typedef typename iterator::reference  reference;
};

template<class It, class F>
inline detail::map_iterator<It, F>
map(const It &it, const F &f) {
  return detail::map_iterator<It,F>(it, f);
}

}} // imperative::iterator


#endif /* ITERATOR_MAP_HXX_ */
