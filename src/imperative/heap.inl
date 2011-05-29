/*
 * heap.inl
 *
 *  Created on: 21 mai 2011
 *      Author: k1000
 */

#ifndef HEAP_INL_
#define HEAP_INL_

#include"../stdlib.hxx"

#include "heap.hxx"

#include "algebra/utils.hxx"

namespace imperative {
namespace heap {
/*using namespace imperative::heap;*/


// *************************** heap ************************ //
template<class O>
inline
heap<O>::heap()
: data()
{}

template<class O>
inline
heap<O>::heap(const heap &o)
: data(o.data)
{}

template<class O>
inline heap<O> &
heap<O>::operator=(const heap &o) {
  data = o.data;
  return *this;
}

template<class O>
inline void
heap<O>::swap(heap &o) {
  std::swap(data, o.data);
}

template<class O>
inline
heap<O>::heap(size_t n)
: data()
{ data.reserve(n); }

template<class O>
inline
heap<O>::~heap()
{}

template<class O>
inline bool
heap<O>::empty() const
{ return data.empty(); }

template<class O>
inline void heap<O>::insert(const e &x) {
  data.push_back(x);
  std::push_heap(
    data.begin(),
    data.end(),
    algebra::compare<O>);
}

template<class O>
inline const typename heap<O>::e &
heap<O>::findMin() const {
  assert(data.size() > 0);
  return data.front();
}

template<class O>
inline typename heap<O>::e &
heap<O>::findMin() {
  assert(data.size() > 0);
  return data.front();
}

template<class O>
inline std::list<typename heap<O>::e>
heap<O>::findMins() const {
  assert(data.size() > 0);

  e x0 = data.front();

  std::list<e> ret(0);

  foreach(const e &x, data)
    if(algebra::compare(x0, x) == 0)
      ret.push_front(x);

  return ret;
}

template<class O>
inline void heap<O>::deleteMin() {
  assert(data.size() > 0);

  std::pop_heap(
    data.begin(),
    data.end(),
    algebra::compare<O>
  );
  data.pop_back();
}

template<class O>
inline void heap<O>::deleteMins() {
  size_t sz = data.size();

  assert(sz > 0);

  e m = data.front();

  vect_t::iterator
    b=data.begin(),
    e=data.end();

  do {
    std::pop_heap(b, e, algebra::compare<O>);
    --e; --sz;
  }
  while(sz > 0
     && algebra::compare(m, *b) == 0);

  data.resize(sz);
}

// ***** list adaptor for chain ***** //

template<class T>
inline
adapt<T>::adapt(const T &x)
: repr(new list_t(1,x))
{}

template<class T>
inline
adapt<T>::adapt()
: repr()
{}
template<class T>
inline
adapt<T>::adapt(const adapt &a)
: repr(a.repr)
{}
template<class T>
inline const adapt<T> &
adapt<T>::operator=(const adapt &a) {
  repr = a.repr;
  return *this;
}

template<class T>
inline
adapt<T>::~adapt()
{}

template<class T>
template<class I>
inline void
adapt<T>::insert(const I &i)
{ repr->insert(i); }

template<class T>
inline const T &
adapt<T>::front() const
{ return repr->front(); }

template<class T>
inline void
adapt<T>::push_front(const T &x)
{ repr->push_front(x); }

template<class T>
inline const list_t &
adapt<T>::list() const
{ return *repr; }

template<class T>
inline int
adapt<T>::compare(const adapt &a, const adapt &b) {
  return algebra::compare(
    a.repr->front(),
    b.repr->front());
}

// ***************************** chain *********************** //
template<class O>
inline
chain<O>::chain()
: impl() {}

template<class O>
inline
chain<O>::chain(const chain &o)
: impl(o)
{}

template<class O>
inline chain<O> &
chain<O>::operator=(const chain &o) {
  impl::operator=(o);
  return *this;
}

template<class O>
inline void
chain<O>::swap(heap &o) {
  impl::swap(o);
}

template<class O>
inline
chain<O>::chain(size_t n)
: impl(n) {}

template<class O>
inline
chain<O>::~chain()
{}

template<class O>
inline bool
chain<O>::empty() const {
  return impl::empty();
}

template<class O>
inline void
chain<O>::insert(const e &x) {
  for(vect_t::iterator
        it = impl::data.begin(),
        end= impl::data.end();
      it != end;
      ++it)
    if(algebra::compare(x, it->front()) == 0)
    { (*it).push_front(x); return; }

  /* no existing list -> insert le(x) */
  impl::insert(le(x));
}

template<class O>
inline typename chain<O>::e &
chain<O>::findMin() {
  return impl::findMin().front();
}

template<class O>
inline const typename chain<O>::e &
chain<O>::findMin() const {
  return impl::findMin().front();
}

template<class O>
inline std::list<typename chain<O>::e>
chain<O>::findMins() const {
  return impl::findMin().list();
}

template<class O>
inline void
chain<O>::deleteMin() {
  le &lmin = impl::findMin();

  assert(! lmin.empty());

  lmin.pop_front();

  if(lmin.empty()) impl::deleteMin();
}

template<class O>
inline void chain<O>::deleteMins() {
  impl::deleteMin();
}

}} // namespace imperative::heap

#endif /* HEAP_INL_ */
