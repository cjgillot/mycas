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


// standard heap class
// implemented on top of std::*_heap
//
// O must be algebra::compare'able,
// assignable and copyable
template<class O>
struct heap {
  typedef O e;

protected:
  typedef std::vector<e> vect_t;
  vect_t data;

public:
  heap();
  heap(const heap &);
  heap &operator=(const heap &);

  // construct empty heap and reserve size
  explicit heap(size_t n);
  ~heap();

  void swap(heap &);

public: // access
  /// C = 1
  bool empty() const;

  /// C = 1
  e &find_min();
  const e &find_min() const;

  /// C = n
  std::list<e> find_mins() const;

public: // modification algorithms
  /// C = lg n
  void insert(const e &x);

  /// C = lg n
  void delete_min();

  /// C = n * lg n
  void delete_mins();
};


/* pointer proxy to std::list */
template<class T>
struct adapt {
  typedef std::list<T> list_t;

private:
  boost::shared_ptr<list_t> repr;

public:
  explicit
  adapt(const T &);

  adapt();
  adapt(const adapt &);
  const adapt &
  operator=(const adapt &);

  ~adapt() {}

  template<class I>
  void
  insert(const I &i);

  const T &
  front() const;

  void
  push_front(const T &x);

  const list_t &
  list() const;

  static int
  compare(const adapt &a, const adapt &b);
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
template<class O>
struct chain: protected heap<adapt<O> > {
  typedef O e;
  typedef adapt<O> le;

  // the underlying heap contains
  //  k adapt<O>
  // for a total of n O

private:
  typedef heap<le> impl;

public:
  chain();
  chain(const chain &);
  chain &operator=(const chain &);

  // construct empty heap and reserve size
  explicit chain(size_t n);
  ~chain();

  void swap(chain &);

public:
  /// C = 1
  bool empty() const;

  /// C = 1
  e &find_min();
  const e &find_min() const;

  /// C = 1
  std::list<e> find_mins() const;

public:
  /// C = k
  void insert(const e &x);

  /// C = lg k
  void delete_min();

  /// C = lg k
  void delete_mins();
};


}} // namespace imperative::heap

#include "heap.inl"

#endif /* HEAP_HXX_ */
