/*
 * heap.hxx
 *
 *  Created on: 21 mai 2011
 *      Author: k1000
 */

#ifndef HEAP_HXX_
#define HEAP_HXX_

#include "stdlib.hxx"
#include "operators.hxx"

namespace imperative {
namespace heap {


// standard _max_-heap class
// implemented on top of std::*_heap
//
// O must be less_than_compare'able,
// assignable and copyable
//
// all references returned by find_max* functions
// are invalidated by insert and detete_max* calls
template<class O>
struct heap {
  BOOST_STATIC_ASSERT(!boost::is_pointer<O>::value);

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
  : data()
  { data.reserve(n); }

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
  find_max() {
    assert(! empty());
    return data.front();
  }
  inline const e &
  find_max() const {
    assert(! empty());
    return data.front();
  }

  /// C = n
  inline std::list<e>
  find_maxs() const {
    assert(! empty());

    e x0 = data.front();

    std::list<e> ret;

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
  delete_max() {
    assert(data.size() > 0);

    boost::pop_heap(data);
    data.pop_back();
  }

  /// C = n * lg n
  inline void
  delete_maxs() {
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

namespace detail {

template<class O>
struct list
: operators::ordered<list<O>
, operators::printable<list<O>
> > {
  typedef std::list<O> ptr_t;
  ptr_t* ptr;

  inline
  list()
  : ptr(0) {}
  inline
  list(ptr_t* p)
  : ptr(p) {}
  inline
  list(std::size_t n, const O &x)
  : ptr(new ptr_t(n,x)) {}
  inline
  list(const list &o)
  : ptr(o.ptr) {}
  inline list &
  operator=(const list &o) {
    ptr = o.ptr;
    return *this;
  }
  inline
  ~list() {}

  inline ptr_t*
  get() const
  { return ptr; }

  inline ptr_t &
  operator*() const
  { return *ptr; }
  inline ptr_t*
  operator->() const
  { return ptr; }

  template<class S>
  inline void
  print(S &ios) {
    ios << "heap::list[ ";
    foreach(const O &e, *ptr) {
      ios << e << " ; ";
    }
    ios << " ] ";
    return ios;
  }

  static inline int
  compare(const list &a, const list &b) {
    return algebra::compare(a->front(), b->front());
  }
};

} // namespace detail

// chained heap structure
// elements comparing equal are packed in lists
//
// speeds up :
// -> find_maxs   from (n) to (1)
// -> delete_maxs from (n * lg n) to (lg n)
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
: protected heap<detail::list<O> > {
  typedef O e;
  typedef detail::list<e> le;

  // the underlying heap contains
  //  k [list<O>] objects
  // for a total of n [O] objects

private:
  typedef heap<le> impl;

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
    foreach(le l, impl::data)
      delete l.get();
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
  find_max()
  { return impl::find_max()->front(); }
  inline const e &
  find_max() const
  { return impl::find_max()->front(); }

  /// C = 1
  inline const std::list<e> &
  find_maxs() const
  { return *impl::find_max(); }

public:
  /// C = k
  inline void
  insert(const e &x) {
    // seek for existing list
    foreach(le &l, impl::data) {
      if(x == l->front()) {
        // got one
        l->push_front(x);
        return;
      }
    }

    // no existing list -> insert new le(x)
    impl::insert(le(1, x));
  }

  /// C = lg k
  inline void
  delete_max() {
    assert(! empty());

    le lmax = impl::find_max();

    assert(!lmax->empty());

    lmax->pop_front();

    if(lmax->empty()) {
      delete lmax.get();
      impl::delete_max();
    }
  }

  /// C = lg k
  inline void
  delete_maxs() {
    assert(! empty());
    delete impl::find_max().get();
    impl::delete_max();
  }
};


}} // namespace imperative::heap

#endif /* HEAP_HXX_ */
