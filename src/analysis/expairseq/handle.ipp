#ifndef EXPAIRSEQ_HANDLE_IPP
#define EXPAIRSEQ_HANDLE_IPP

#include "analysis/expairseq/handle.hpp"

namespace analysis {
namespace epseq {

// cdtor
template<class I, class M>
inline
handle<I,M>::handle(const eps_t* p)
: m_ptr(p) { assert(p); }

template<class I, class M>
inline
handle<I,M>::handle(const handle &o)
: m_ptr(o.m_ptr) {}

template<class I, class M>
inline handle<I,M> &
handle<I,M>::operator=(const handle &o) {
  m_ptr = o.m_ptr;
  return *this;
}

template<class I, class M>
inline
handle<I,M>::~handle() throw() {}

// coercion
template<class I, class M>
inline
handle<I,M>::operator expr() const
{ return expr(m_ptr.get()); }

// operations
template<class I, class M>
inline handle<I,M>
handle<I,M>::operator+(const handle &o) const {
  assert(compare(*this, o) == 0);
  return chg_coef(m_ptr->m_coef + o.m_ptr->m_coef);
}

template<class I, class M>
inline handle<I,M>
handle<I,M>::operator-(const handle &o) const {
  assert(compare(*this, o) == 0);
  return chg_coef(m_ptr->m_coef - o.m_ptr->m_coef);
}
template<class I, class M>
inline handle<I,M>
handle<I,M>::operator-() const {
  return chg_coef(- m_ptr->m_coef);
}
template<class I, class M>
inline handle<I,M>
handle<I,M>::sca(const number &n) const {
  return chg_coef(m_ptr->m_coef * n);
}

// tests & comparison
template<class I, class M>
inline bool
handle<I,M>::null() const
{ return m_ptr->m_coef.null(); }

template<class I, class M>
inline util::cmp_t
handle<I,M>::compare(const handle &a, const handle &b)
{ return a.m_ptr->partial_compare(*b.m_ptr); }

template<class I, class M>
inline util::cmp_t
handle<I,M>::deep_compare(const handle &a, const handle &b)
{ return basic::compare(*a.m_ptr, *b.m_ptr); }

template<class I, class M>
inline std::size_t
handle<I,M>::hash() const
{ return m_ptr->hash(); }

// printing
template<class I, class M>
template<class S>
inline void
handle<I,M>::print(S &os) const
{ m_ptr->print(os); }

template<class I, class M>
inline const typename handle<I,M>::eps_t*
handle<I,M>::ptr() const throw()
{ return m_ptr.get(); }

// operations implementation
template<class I, class M>
inline typename handle<I,M>::eps_t*
handle<I,M>::chg_coef(const number &n) const throw() {
  eps_t* ret = m_ptr->clone();
  ret->m_coef = n;
  return ret;
}

}} // namespace analysis::epseq


#endif
