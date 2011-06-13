/*
 * generator.hxx
 *
 *  Created on: 13 juin 2011
 *      Author: k1000
 */

#ifndef GENERATOR_HXX_
#define GENERATOR_HXX_

#include "stdlib.hxx"

namespace imperative {
namespace iterator {

namespace detail {

template<class F>
class gen_iterator
: public boost::iterator_facade<
    gen_iterator<It, F>
  , typename boost::result_of<F()>::type const
  , boost::incrementable_traversal_tag
  >
{
  typedef typename boost::iterator_value<It>::type T;
  typedef typename boost::result_of<F(T)>::type res_t;

  F f;

public:
  inline
  gen_iterator() {}
  inline
  gen_iterator(const gen_iterator &o)
  : f(o.f) {}
  inline gen_iterator &
  operator=(const gen_iterator &o) {
    f=o.f;
    return *this;
  }

  inline void
  swap(gen_iterator &o) {
    std::swap(f, o.f);
  }

  inline explicit
  gen_iterator(const F &f)
  : f(f) {}

  inline virtual
  ~gen_iterator() {}

private:
  friend class boost::iterator_core_access;

  inline void
  increment() {}
  inline const T &
  dereference() const {
    return f();
  }

private:
  typedef util::safe_bool<void(gen_iterator::*)(gen_iterator&)> safe_bool;
  typedef typename safe_bool::unspecified_bool_type bool_t;
public:
  inline
  operator bool_t() const {
    return safe_bool::to_unspecified_bool(
        true,
        &gen_iterator::swap
    );
  }
  inline bool
  operator!() const
  { return false; }
};

} // namespace detail

template<class F>
struct gen_type {
  typedef detail::gen_iterator<F> iterator;
  typedef typename iterator::value_type         value_type;
  typedef typename iterator::reference          reference;
  typedef typename iterator::pointer            pointer;
  typedef typename iterator::difference_type    difference_type;
  typedef typename iterator::iterator_category  iterator_category;
};

template<class F>
inline detail::gen_iterator<F>
generate(const F &f) {
  return detail::gen_iterator<F>(f);
}

}} // imperative::iterator


#endif /* GENERATOR_HXX_ */
