/*
 * base.hpp
 *
 *  Created on: 20 juin 2011
 *      Author: k1000
 */

#ifndef MAGMA_BASE_HPP_
#define MAGMA_BASE_HPP_

#include "stdlib.hpp"
#include "operators.hpp"

#include "structures/magma/data.hpp"

#include "algebra/compare.hpp"

namespace structures {
namespace magma {

template<class> class comm;

template<class T>
class base
: public operators::ordered<base<T> > {

  boost::intrusive_ptr<tree> impl;

protected:
  friend class comm<T>;

  inline base() {}

  inline void
  init_tree(const base &a, const base &b) {
    impl=new node(a.impl,b.impl);
  }

public:
  inline
  base(const base &o)
  : impl(o.impl) {}
  inline base&
  operator=(const base &o) {
    impl = o.impl;
    return *this;
  }
  inline void
  swap(base &o) {
    impl.swap(o.impl);
  }

public:
  inline explicit
  base(const T &l)
  : impl(new leaf<T>(l)) {}

protected:
  inline
  base(const tree* t)
  : impl(t) {}
  inline
  base(const base &a, const base &b)
  { init_tree(a,b); }

public:
  inline
  ~base() {}

public:
  inline T&
  value() const {
    assert(impl && impl->is_leaf());
    return *static_cast<T*>(impl->value());
  }
  inline base
  left () const {
    assert(impl);
    return impl->left();
  }
  inline base
  right() const {
    assert(impl);
    return impl->right();
  }

public:
  inline bool
  is_leaf() const
  { return impl && impl->is_leaf(); }

public:
  friend inline base
  operator+(const base &a, const base &b)
  { return base(a,b); }
  inline base&
  operator+=(const base &o) {
    (*this + o).swap(*this);
    return *this;
  }

public:
  static int
  compare(const base &a, const base &b) {
    if(a.is_leaf()) {
      if(! b.is_leaf())
        return -1; // leaf < node

      return algebra::compare(a.value(), b.value());
    }

    if(b.is_leaf()) return +1; // node > leaf

    int c = compare(a.left(), b.left());
    return c ? c : compare(a.right(), b.right());
  }

private:
  template<class F>
  struct map2_type {
    typedef typename boost::result_of<F(const T&)>::type type;
  };
  template<class F>
  struct map_type {
    typedef typename boost::result_of<F(const T&)>::type reference;
    typedef typename boost::remove_reference<reference>::type value_type;
    typedef base<value_type> type;
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
  inline base
  reverse() const {
    if(is_leaf())
      return *this;
    return base(
      right().reverse(),
      left ().reverse()
    );
  }

  template<class F>
  inline typename map2_type<F>::type
  rev_map2(F f) const {
    typedef typename map2_type<F>::type base2;
    if(is_leaf())
      return f(value());
    return base2(
      right().rev_map2(f),
      left() .rev_map2(f)
    );
  }
  template<class F>
  inline typename map_type<F>::type
  rev_map(F f) const {
    typedef typename map_type<F>::type base2;
    if(is_leaf())
      return base2(f(value()));
    return base2(
      right().rev_map(f),
      left() .rev_map(f)
    );
  }
};

}} // namespace structures::magma

#endif /* MAGMA_BASE_HPP_ */
