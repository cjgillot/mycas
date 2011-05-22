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

/*
namespace algebra {

template<class O, class A>
int compare<std::list<O, A> >(const std::list<O, A> &a, const std::list<O, A> &b) {
  assert(a && b);

  return algebra::compare(a.front(), b.front());
}

}
*/

namespace imperative {
namespace heap {
using namespace imperative::heap;


// ********************************* heap ****************************** //
template<class O>
inline
heap<O>::heap()
: data() {}

template<class O>
inline
heap<O>::heap(size_t n)
: data() { data.reserve(n); }

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
  std::push_heap(data.begin(), data.end(), algebra::compare<O>);
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

  std::list<e*> ret(1, &x0);

  foreach(e *x, data)
    if(algebra::compare(x0, x) == 0)
      ret.push_front(x);

  return ret;
}

template<class O>
inline void heap<O>::deleteMin() {
  assert(data.size() > 0);

  std::pop_heap(data.begin(), data.end(), algebra::compare<O>);
  data.pop_back();
}

template<class O>
inline void heap<O>::deleteMins() {
  size_t sz = data.size();

  assert(sz > 0);

  e* m = data.front();

  typename std::vector<e*>::iterator
    b=data.begin(),
    e=data.end();

  do {
    std::pop_heap(b,e, algebra::compare<O>);
    --e; --sz;
  } while(sz >= 0
      && O::compare(m, data.front()) == 0);

  data.resize(sz);
}

// ***************************** chain ********************************** //
template<class O>
inline
chain<O>::chain()
: impl() {}

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
  for(typename std::vector<le>::iterator
        it = impl::data.begin(),
        end= impl::data.end();

      it != end;

      ++it)

    if(algebra::compare(x, it->front()) == 0)
    { (*it).push_front(x); return; }

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
