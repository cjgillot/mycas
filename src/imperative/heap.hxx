/*
 * heap.hxx
 *
 *  Created on: 21 mai 2011
 *      Author: k1000
 */

#ifndef HEAP_HXX_
#define HEAP_HXX_

#include "stdlib.hxx"

namespace imperative {
namespace heap {


// standard _max_-heap class
// implemented on top of std::*_heap

// O must be less_than_compare'able,
// assignable and copyable

// all references returned by find_min* functions
// are invalidated by insert and detete_min* calls

template<class O>
struct heap {
  typedef O e;

protected:
  typedef std::vector<e> vect_t;
  vect_t data;

public:
  inline heap() {}
  inline heap(const heap &o)
  : data(o.data) {}
  inline heap &
  operator=(const heap &o) {
    data = o.data;
    return *this;
  }

  // construct empty heap and reserve size
  explicit inline
  heap(size_t n)
  : data(n) {}

  inline
  ~heap() {}

  inline void
  swap(heap &o) {
    std::swap(data, o.data);
  }

public: // access
  /// C = 1
  inline bool
  empty() const {
    return data.empty();
  }

  /// C = 1
  inline e &
  find_min() {
    assert(! empty());
    return data.front();
  }
  inline const e &
  find_min() const {
    assert(! empty());
    return data.front();
  }

  /// C = n
  inline std::list<e>
  find_mins() const {
    assert(! empty());

    e x0 = data.front();

    std::list<e> ret(0);

    foreach(const e &x, data)
      if(x0 == x)
        ret.push_front(x);

    return ret;
  }

public: // modification algorithms
  /// C = lg n
  inline void
  insert(const e &x) {
    data.push_back(x);
    boost::push_heap(data);
  }

  /// C = lg n
  inline void
  delete_min() {
    assert(data.size() > 0);

    boost::pop_heap(data);
    data.pop_back();
  }

  /// C = n * lg n
  inline void
  delete_mins() {
    assert(! empty());

    size_t sz = data.size();

    const e &m = data.front();

    typename vect_t::iterator
      b=data.begin(),
      e=data.end();

    do {
      std::pop_heap(b, e);
      --e; --sz;
    }
    while(sz && m == *b);

    data.resize(sz);
  }
};


// chained heap structure
// elements comparing equal are packed in lists
//
// speeds up :
// -> find_mins   from (n) to (1)
// -> delete_mins from (n * lg n) to (lg n)
//
// drawbacks :
// -> insert from (lg n) to (n)
//
// memory management :
// -> all heap allocated lists in impl,
//      no null pointer nor empty list
// -> new in insert
// -> delete in delete_* and dtor
template<class O>
struct chain
: protected heap<std::list<O>*> {
  typedef O e;
  typedef std::list<O> le;

  // the underlying heap contains
  //  k [adapt<O>] objects
  // for a total of n [O] objects

private:
  typedef heap<le*> impl;

public:
  inline
  chain()
  : impl() {}
  inline
  chain(const chain &o)
  : impl(o) {}
  inline chain &
  operator=(const chain &o) {
    impl::operator=(o);
    return *this;
  }

  // construct empty heap and reserve size
  explicit inline
  chain(size_t n)
  : impl(n) {}

  inline
  ~chain() {
    // properly free all allocated lists
    foreach(le* l, impl::data)
      delete l;
    impl::data.clear();
  }

  inline void
  swap(chain &o)
  { impl::swap(o); }

public:
  /// C = 1
  using impl::empty;

  /// C = 1
  inline e &
  find_min()
  { return impl::find_min()->front(); }
  inline const e &
  find_min() const
  { return impl::find_min()->front(); }

  /// C = 1
  inline const std::list<e> &
  find_mins() const
  { return *impl::find_min(); }

public:
  /// C = k
  inline void
  insert(const e &x) {
    // seek for existing list
    foreach(le *l, impl::data)
      if(x == l->front()) {
        // got one
        l->push_front(x);
        return;
      }

    // no existing list -> insert new le(x)
    impl::insert(new le(1, x));
  }

  /// C = lg k
  inline void
  delete_min() {
    assert(! empty());

    le* lmin = impl::find_min();

    assert(! lmin->empty());

    lmin->pop_front();

    if(lmin->empty()) {
      delete lmin;
      impl::delete_min();
    }
  }

  /// C = lg k
  inline void
  delete_mins() {
    assert(! empty());
    delete impl::find_min();
    impl::delete_min();
  }
};


}} // namespace imperative::heap

#endif /* HEAP_HXX_ */
