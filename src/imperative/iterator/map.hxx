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

template<class It, class F>
class map_iterator
: public boost::transform_iterator<F, It>
{
  typedef boost::transform_iterator<F, It> super_t;

public:
  inline typename super_t::value_type
  operator*() const {
    return super_t::operator*();
  }
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

  inline
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
