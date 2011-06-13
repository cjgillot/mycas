/*
 * iterator.hxx
 *
 *  Created on: 12 juin 2011
 *      Author: k1000
 */

/*
 * definition of several iterator classes
 * for use with the stream structure
 *
 * these are based on the abstract iterator_base class
 * and handled by the meta_iterator class
 *
 * this design allows objects deriving
 * from iterator_base to promote when incremented
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
template<class T>
struct meta_iterator
: boost::iterator_facade<
  meta_iterator<T>
, const T
, boost::incrementable_traversal_tag
> {

  // [impl] contains the actual iterator
  typedef iterator_base<T> impl_t;

  // mutable for empty() optimization
  mutable boost::intrusive_ptr<impl_t> impl;

public:
  inline
  meta_iterator()
  : impl() {}
  inline
  meta_iterator(const meta_iterator &o)
  : impl(o.impl) {}
  inline meta_iterator &
  operator=(const meta_iterator &o) {
    impl=o.impl;
    return *this;
  }
  inline void
  swap(meta_iterator &o) {
    std::swap(impl, o.impl);
  }

  inline
  meta_iterator(impl_t* ptr)
  : impl(ptr) {}

  inline
  ~meta_iterator()
  {}

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
    // impl = impl->incr();
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
struct adapter
: iterator_base<
    typename boost::iterator_value<It>::type
  > {

  typedef typename boost::iterator_value<It>::type T;
  It impl;

public:
  inline
  adapter()
  {}
  inline
  adapter(const adapter &o)
  : impl(o.impl) {}
  inline adapter &
  operator=(const adapter &o) {
    impl=o.impl;
    return *this;
  }

  inline void
  swap(adapter &o) {
    std::swap(impl, o.impl);
  }
  inline
  ~adapter()
  {}

  inline explicit
  adapter(const It &it)
  : impl(it) {}

public:
  iterator_base<T>* clone() const
  { return new adapter(impl); }
  iterator_base<T>* incr()
  { return ++impl ? this : 0; }
  T    deref() const
  { return *impl; }
  bool empty() const
  { return !impl; }
};

template<class It>
inline adapter<It>*
adapt(const It &it) {
  return new adapter<It>(it);
}

} // namespace detail

} // namespace streams



#endif /* ITERATOR_HXX_ */
