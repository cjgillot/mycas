/*
 * iterator.hxx
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

#ifndef ITERATOR_HXX_
#define ITERATOR_HXX_

#include "stdlib.hxx"
#include "utils.hxx"

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
: boost::iterator_facade<
  meta_iterator<T>
, const T
, boost::incrementable_traversal_tag
, const T // Reference
> {
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
    return proxy(impl);
  }
  inline
  meta_iterator(const proxy &o) {
    impl=o.release();
  }
  inline meta_iterator &
  operator=(const proxy &o) {
    impl=o.release();
    return *this;
  }

public:
  inline impl_t*
  ptr() const
  { return impl.get(); }

private:
  typedef util::safe_bool<void(meta_iterator::*)(meta_iterator&)> safe_bool;
  typedef typename safe_bool::unspecified_bool_type bool_t;
public:
  inline
  operator bool_t() const {
    return safe_bool::to_unspecified_bool(
        !empty(),
        &meta_iterator::swap
    );
  }
  inline bool
  operator!() const {
    return empty();
  }

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

  typedef adapter_type<It>::type super_t;
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
  inline iterator_base<It>*
  do_it(const It &it) {
    return new adapter<It>(it);
  }
};

template<class T>
struct do_adapt<meta_iterator<T> > {
  inline iterator_base<It>*
  do_it(const meta_iterator<It> &it) {
    return it.ptr();
  }
};

template<class It>
inline boost::intrusive_ptr<iterator_base<It> >
adapt(const It &it) {
  return do_adapt<It>::do_it(it);
}

} // namespace detail

} // namespace streams

#endif /* ITERATOR_HXX_ */
