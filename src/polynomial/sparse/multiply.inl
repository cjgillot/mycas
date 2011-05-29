/*
 * multiply.inl
 *
 *  Created on: 21 mai 2011
 *      Author: k1000
 */

#ifndef SPARSE_MULTIPLY_INL
#define SPARSE_MULTIPLY_INL

#include "stdlib.hxx"

namespace poly {
namespace sparse {
namespace multiply {

template<class P, class S>
inline
common<P,S>::common(size_t l)
: so(l), ready(false), cur(0)
{}

template<class P, class S>
inline bool
common<P,S>::empty() const
{ return so.empty(); }

template<class P, class S>
inline const typename P::mono &
common<P,S>::get() const {
  if(ready) return cur;
  typedef typename P::mono mono;
  const std::list<heap_obj<P>*> &mins = so.findMins();
  cur = algebra::zero<mono>();
  foreach(heap_obj<P>* ho, mins)
    cur += ho->value();
  ready = true;
  return cur;
}

template<class P, class S>
inline void
common<P,S>::next() {
  std::list<typename S::ho*> ml = so.findMins();

  ready = false;

  so.deleteMins();

  so.update(ml);
}

template<class P>
inline
heap_obj<P>::heap_obj(const mono &f0, const P &g)
: gen((assert(!g.empty()),
    g | boost::adaptors::transformed(mono_muler(f0))
  ))
, cur(gen.front())
{}


/* comparison is reversed to get a max-heap */
template<class P>
inline int
heap_obj<P>::compare(const heap_obj &a, const heap_obj &b) {
  return algebra::compare(b.cur, a.cur);
}

template<class P>
inline const typename P::mono &
heap_obj<P>::value() const
{ return cur; }

template<class P>
inline bool
heap_obj<P>::update() {
  assert(gen);

  gen.pop_front();

  if(gen.empty()) return false;

  cur = gen.front();

  return true;
}

template<class P>
inline
heap<P>::heap(size_t l)
: super(l)
{}
template<class P>
inline
heap<P>::~heap()
{
  while(!super::empty()) {
    std::list<ho*> min = super::findMins();
    foreach(ho* m, min) delete m;
    super::deleteMins();
  }
}

template<class P>
inline void
heap<P>::update(std::list<ho*> work) {
  foreach(ho* m, work)
    if(m->update()) insert(m);
    else delete m;
}

template<class P>
inline
poly<P>::poly(const P &a, const P &b)
: super(a.size())
{
  assert(!a.empty() && !b.empty());

  foreach(const mono &m, a)
    super::so.insert(new ho_t(m, b));
}

}}} // poly::sparse::multiply

#endif // SPARSE_MULTIPLY_HXX
