#ifndef PTR_IXX_
#define PTR_IXX_

#include "stdlib.hpp"

#include "analysis/ptr.hpp"

namespace analysis {

template<class T>
inline
ptr<T>::ptr(const ptr &o)
: m_ptr(o.m_ptr) {}

template<class T>
inline ptr<T> &
ptr<T>::operator=(const ptr &o) {
  m_ptr=o.m_ptr;
  return *this;
}

template<class T>
inline void
ptr<T>::swap(ptr &o)
{ m_ptr.swap(o.m_ptr); }

template<class T>
inline
ptr<T>::~ptr() {}

template<class T>
inline
ptr<T>::ptr(const T* p)
: m_ptr(p) {
  assert(p != 0);
}

template<class T>
inline T*
ptr<T>::cow() {
  if(!util::unique(m_ptr.get()))
    m_ptr = m_ptr->clone();
  return const_cast<T*>(m_ptr.get());
}

template<class T>
inline bool
ptr<T>::null() const {
  return m_ptr->null();
}

template<class T>
inline bool
ptr<T>::unit() const {
  return m_ptr->unit();
}

template<class T>
inline const T*
ptr<T>::eval(unsigned lv) const {
  expr ev = m_ptr->eval(lv);
  const T* p = ev.as_a<T>();

  if(p && m_ptr.get() != p)
    m_ptr.reset(p);
  return p;
}

template<class T>
inline void
ptr<T>::print(std::basic_ostream<char> &os) const
{ m_ptr->print(os); }

template<class T>
template<class Compare>
inline int
ptr<T>::compare(const ptr &a, const ptr &b, Compare cmp) {
  if(a.m_ptr == b.m_ptr)
    return 0;
/*
  if(! b.m_ptr)
    return (bool)a.m_ptr;

  if(! a.m_ptr)
    return -1;
*/
  int c = cmp(*a.m_ptr, *b.m_ptr);

  if(c) return c;

  // here, a == b
  util::unify_ptr(a.m_ptr, b.m_ptr);

  return 0;
}

}


#endif /* PTR_HXX_ */
