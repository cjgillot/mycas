/*
 * map.hxx
 *
 *  Created on: 11 juin 2011
 *      Author: k1000
 */

#ifndef ITERATOR_MAP_HXX_
#define ITERATOR_MAP_HXX_

#include "stdlib.hxx"

namespace imperative {
namespace iterator {

namespace detail {

/*
template<class It, class F>
class map_iterator
: public boost::iterator_facade<
    map_iterator<It, F>
  , typename boost::result_of<F(
      typename boost::iterator_value<It>::type
    )>::type const
  , boost::incrementable_traversal_tag
  >
{
  typedef typename boost::iterator_value<It>::type T;
  typedef typename boost::result_of<F(T)>::type res_t;

  F f;
  It src;

public:
  inline
  map_iterator() {}
  inline
  map_iterator(const map_iterator &o)
  : f(o.f), src(o.src) {}
  inline map_iterator &
  operator=(const map_iterator &o) {
    f=o.f; src=o.src;
    return *this;
  }

  inline void
  swap(map_iterator &o) {
    std::swap(f, o.f);
    std::swap(src, o.src);
  }

  inline
  map_iterator(const It &s, const F &f)
  : f(f), src(s) {}

  inline virtual
  ~map_iterator() {}

private:
  friend class boost::iterator_core_access;

  inline void
  increment() {
    assert(src);
    ++src;
  }
  inline const T &
  dereference() const {
    assert(src);
    return f(*src);
  }

private:
  typedef util::safe_bool<void(map_iterator::*)(map_iterator&)> safe_bool;
  typedef typename safe_bool::unspecified_bool_type bool_t;
public:
  inline
  operator bool_t() const {
    return safe_bool::to_unspecified_bool(
        src,
        &map_iterator::swap
    );
  }
  inline bool
  operator!() const
  { return !src; }
};
*/

template<class It, class F>
class map_iterator
: private boost::transform_iterator<F, It>
{
  typedef boost::transform_iterator<F, It> super_t;

public:
  using super_t::value_type;
  using super_t::reference;
  using super_t::pointer;
  using super_t::difference_type;
  using super_t::iterator_category;

  using super_t::operator*;

  inline map_iterator &
  operator++() {
    super_t::operator++();
    return *this;
  }

public:
  inline
  map_iterator() {}
  inline
  map_iterator(const map_iterator &o)
  : super_t(o) {}
  inline map_iterator &
  operator=(const map_iterator &o) {
    super_t::operator=(o);
    return *this;
  }

  inline void
  swap(map_iterator &o) {
    std::swap(
      static_cast<super_t&>(*this)
    , static_cast<super_t&>(o)
    );
  }

  inline
  map_iterator(const It &s, const F &f)
  : super_t(s, f) {}

  inline virtual
  ~map_iterator() {}

private:
  typedef util::safe_bool<void(map_iterator::*)(map_iterator&)> safe_bool;
public:
  typedef typename safe_bool::unspecified_bool_type bool_t;

  inline
  operator bool_t() const {
    return safe_bool::to_unspecified_bool(
        super_t::base(),
        &map_iterator::swap
    );
  }
  inline bool
  operator!() const
  { return !super_t::base(); }
};

} // namespace detail

template<class It, class F>
struct map_type {
  typedef detail::map_iterator<It,F> iterator;
  typedef typename iterator::value_type         value_type;
  typedef typename iterator::reference          reference;
  typedef typename iterator::pointer            pointer;
  typedef typename iterator::difference_type    difference_type;
  typedef typename iterator::iterator_category  iterator_category;
};

template<class It, class F>
inline detail::map_iterator<It, F>
map(const It &it, const F &f) {
  return detail::map_iterator<It,F>(it, f);
}

}} // imperative::iterator


#endif /* ITERATOR_MAP_HXX_ */
