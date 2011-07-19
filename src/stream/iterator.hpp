/*
 * iterator.hpp
 *
 *  Created on: 12 juin 2011
 *      Author: k1000
 */

/*
 *
 * definition of several iterator classes
 * for use with the stream structure
 *
 * these are based on the abstract iterator_base class
 * and handled by the meta_iterator class
 *
 * this design allows objects deriving
 * from iterator_base to promote when incremented
 *
 */

#ifndef ITERATOR_HPP_
#define ITERATOR_HPP_

#include "stdlib.hpp"
#include "utils.hpp"

namespace streams {

// abstract base class for iterators
template<class T>
struct iterator_base
: util::refcounted
, boost::noncopyable {
  typedef T value_type;

  inline
  iterator_base() {}
  inline virtual
  ~iterator_base() {}

  // return new reference to copy
  virtual iterator_base*
  clone() const = 0;

  // return the pointed value
  // deref shall _not_ be called more than
  // once between two incrementations
  virtual T
  deref() const = 0;

  // return NULL if empty
  // return non-NULL (maybe this) for next iterator
  virtual iterator_base*
  incr() = 0;

  // default implementation :
  // assuming incr() returns NULL if empty
  // calls optimized out by meta_iterator :
  // if empty() returns true, meta_iterator deletes the iterator
  virtual bool
  empty() const
  { return false; }
};

// enveloppe class
// need of special handling of copy/move semantics
template<class T>
struct meta_iterator
: public boost::iterator_facade<
    meta_iterator<T>
  , const T
  , boost::incrementable_traversal_tag
  , const T // Reference
  >
, public operators::testable<meta_iterator<T> > {

  // [impl] contains the actual iterator
  typedef iterator_base<T> impl_t;

  // mutable for increment() promotion
  mutable boost::intrusive_ptr<impl_t> impl;

public:
  inline
  meta_iterator()
  : impl() {}

  inline explicit
  meta_iterator(impl_t* ptr)
  : impl(ptr) {}

  inline
  ~meta_iterator() {}

public: /// copy semantic
  inline explicit
  meta_iterator(const meta_iterator &o)
  : impl(o.impl->clone()) {}
  inline meta_iterator &
  operator=(const meta_iterator &o) {
    if(this != &o)
      impl=o.impl->clone();
    return *this;
  }
  inline void
  swap(meta_iterator &o) {
    std::swap(impl, o.impl);
  }

public: /// move semantic
  typedef util::move_ptr<impl_t> proxy;

  inline
  operator proxy() { // non-const
    impl_t* p = impl.get();
    intrusive_ptr_add_ref(p);
    impl.reset();
    return proxy(p);
  }
  inline
  meta_iterator(const proxy &o) {
    impl=o.release();
    intrusive_ptr_release(impl.get());
  }
  inline meta_iterator &
  operator=(const proxy &o) {
    impl=o.release();
    intrusive_ptr_release(impl.get());
    return *this;
  }

public:
  inline impl_t*
  ptr() const
  { return impl.get(); }

public:
  inline bool
  valid() const
  { return !empty(); }

private: /// forwarding methods to impl
  friend class boost::iterator_core_access;

  inline void
  increment() {
    assert(! empty());
    impl_t* p = impl->incr();
    if(p != impl.get())
      impl.reset(p);
  }
  inline T
  dereference() const {
    assert(! empty());
    return impl->deref();
  }
  inline bool
  empty() const {
    if(!impl) return true;
    if(impl->empty()) {
      impl.reset();
      return true;
    }
    return false;
  }
};

namespace detail {

template<class It>
struct adapter_type {
  typedef typename boost::iterator_value<It>::type value_type;
  typedef iterator_base<value_type> type;
};

template<class It>
struct adapter
: adapter_type<It>::type {

  typedef typename adapter_type<It>::type super_t;
  typedef typename super_t::value_type T;

  It impl;

protected:
  inline  adapter() {}
  inline ~adapter() {}

public:
  inline explicit
  adapter(const It &it)
  : impl(it) {}

public:
  super_t* clone() const
  { return new adapter(impl); }
  super_t* incr()
  { return ++impl ? this : 0; }
  T    deref() const
  { return *impl; }
  bool empty() const
  { return !impl; }
};

template<class It>
struct do_adapt {
  static inline typename adapter_type<It>::type*
  do_it(const It &it) {
    return new adapter<It>(it);
  }
};

template<class T>
struct do_adapt<meta_iterator<T> > {
  static inline iterator_base<T>*
  do_it(const meta_iterator<T> &it) {
    return it.ptr();
  }
};

template<class It>
inline typename adapter_type<It>::type*
adapt(const It &it) {
  return do_adapt<It>::do_it(it);
}

} // namespace detail

} // namespace streams

#endif /* ITERATOR_HPP_ */
