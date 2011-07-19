/*
 * comm.hpp
 *
 *  Created on: 20 juin 2011
 *      Author: k1000
 */

#ifndef COMM_HPP_
#define COMM_HPP_

#include "stdlib.hpp"
#include "operators.hpp"

#include "algebra/compare.hpp"

#include "structures/magma/base.hpp"

namespace structures {
namespace magma {

template<class T>
class comm
: public operators::ordered<base<T> > {

  typedef base<T> impl_t;
  impl_t impl;

protected:
  inline
  comm() {}

public:
  inline
  comm(const comm &o)
  : impl(o.impl) {}
  inline comm&
  operator=(const comm &o) {
    impl=o.impl;
    return *this;
  }
  inline void
  swap(comm &o) {
    impl.swap(o.impl);
  }

public:
  inline explicit
  comm(const T &l)
  : impl(l) {}

private:
  inline explicit
  comm(const impl_t &i)
  : impl(i) {}
  inline
  comm(const comm &a, const comm &b) {
    if(a <= b) impl.init_tree(a.impl, b.impl);
    else       impl.init_tree(b.impl, a.impl);
  }

public:
  inline
  ~comm() {}

public:
  const T &
  value() const
  { return impl.value(); }

  comm
  left () const
  { return impl.left(); }
  comm
  right() const
  { return impl.right(); }

public:
  bool
  is_leaf() const
  { return impl.is_leaf(); }

public:
  friend inline comm
  operator+(const comm &a, const comm &b)
  { return comm(a,b); }
  inline comm&
  operator+=(const comm &o) {
    (*this + o).swap(*this);
    return *this;
  }

public:
  static int
  compare(const comm &a, const comm &b)
  { return impl_t::compare(a,b); }

private:
  template<class F>
  struct map2_type {
    typedef typename boost::result_of<F(const T&)>::type type;
  };
  template<class F>
  struct map_type {
    typedef typename boost::result_of<F(const T&)>::type reference;
    typedef typename boost::remove_reference<reference>::type value_type;
    typedef comm<value_type> type;
  };

public:
  template<class F>
  inline typename map2_type<F>::type
  map2(F f) const {
    typedef typename map2_type<F>::type base2;
    if(is_leaf())
      return f(value());
    return base2(
      left().map2(f),
      right().map2(f)
    );
  }
  template<class F>
  inline typename map_type<F>::type
  map(F f) const {
    typedef typename map_type<F>::type base2;
    if(is_leaf())
      return base2(f(value()));
    return base2(
      left() .map(f),
      right().map(f)
    );
  }

public:
  inline const comm &
  reverse() const
  { return *this; }
  template<class F>
  inline typename map2_type<F>::type
  rev_map2(F f) const
  { return map2(f); }
  template<class F>
  inline typename map_type<F>::type
  rev_map(F f) const
  { return map(f); }
};

}} // namespace structures::magma

#endif /* COMM_HPP_ */
