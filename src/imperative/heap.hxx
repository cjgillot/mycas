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

template<class O>
struct heap {
  typedef O e;

protected:
  std::vector<e> data;

public:
  heap();
  explicit heap(size_t n);
  ~heap();

  bool empty() const;

  /// C = lg n
  void insert(const e &x);

  /// C = 1
  e &findMin();
  const e &findMin() const;

  /// C = n
  std::list<e> findMins() const;

  /// C = lg n
  void deleteMin();

  /// C = n * lg n
  void deleteMins();
};


template<class T>
struct adapt {
  typedef std::list<T> list_t;

  boost::shared_ptr<list_t> repr;

  adapt(const T &x): repr(new list_t(1,x)) {}
  adapt(const adapt &a): repr(a.repr) {}
  const adapt &operator=(const adapt &a)
  { repr = a.repr; return *this; }
  ~adapt() {}

  template<class I>
  void insert(const I &i)
  { repr->insert(i); }

  const T &front() const
  { return repr->front(); }
  void push_front(const T &x)
  { repr->push_front(x); }

  const list_t &
  list() const
  { return *repr; }

  static int
  compare(const adapt &a, const adapt &b)
  { return algebra::compare(a.repr->front(), b.repr->front()); }
};

template<class O>
struct chain: protected heap<adapt<O> > {
  typedef O e;
  typedef adapt<O> le;

private:
  typedef heap<le> impl;

public:
  chain();
  explicit chain(size_t n);
  ~chain();

  bool empty() const;

  /// C = lb n
  void insert(const e &x);

  /// C = 1
  e &findMin();
  const e &findMin() const;

  /// C = n
  std::list<e> findMins() const;

  /// C = lg n
  void deleteMin();

  /// C = n * lg n
  void deleteMins();
};


}} // namespace imperative::heap

#include "heap.inl"

#endif /* HEAP_HXX_ */
