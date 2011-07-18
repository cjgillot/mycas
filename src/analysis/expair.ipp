#ifndef EXPAIR_IXX_
#define EXPAIR_IXX_

#include "stdlib.hpp"
#include "numeric.hpp"

#include "algebra/utils.hpp"
#include "algebra/compare.hpp"

#include "analysis/expair.hpp"

namespace analysis {

// ctors / dtors
template<class Traits>
inline
expair<Traits>::expair() {}

template<class Traits>
inline
expair<Traits>::expair(const K &c, const Z &r)
: m_coef(c), m_rest(r) {}

template<class Traits>
inline
expair<Traits>::expair(const expair &o)
: m_coef(o.m_coef)
, m_rest(o.m_rest) {}

template<class Traits>
inline expair<Traits>&
expair<Traits>::operator=(const expair &o) {
  m_coef=o.m_coef;
  m_rest=o.m_rest;
  return *this;
}

template<class Traits>
inline void
expair<Traits>::swap(expair &o) {
  m_coef.swap(o.m_coef);
  m_rest.swap(o.m_rest);
}

template<class Traits>
inline
expair<Traits>::~expair() {}


// test
template<class Traits>
inline bool
expair<Traits>::null() const {
  return ep::null_pair(m_coef, m_rest);
}

// operations
template<class Traits>
inline expair<Traits>&
expair<Traits>::operator+=(const expair &o) {
  assert(compare(*this, o) == 0);
  ep::add_coef(m_coef, o.m_coef);
  return *this;
}
template<class Traits>
inline expair<Traits>&
expair<Traits>::operator-=(const expair &o) {
  assert(compare(*this, o) == 0);
  ep::sub_coef(m_coef, o.m_coef);
  return *this;
}

template<class Traits>
inline expair<Traits>&
expair<Traits>::ineg() {
  ep::neg_coef(m_coef);
  return *this;
}
template<class Traits>
inline expair<Traits>
expair<Traits>::operator-() const {
  return expair(*this).ineg();
}

// scalar mul/div
template<class Traits>
inline expair<Traits>&
expair<Traits>::operator*=(const number &o) {
  ep::mul_coef(m_coef, o);
  return *this;
}
template<class Traits>
inline expair<Traits>&
expair<Traits>::operator/=(const number &o) {
  ep::div_coef(m_coef, o);
  return *this;
}

// homogeneous mul/div
template<class Traits>
inline expair<Traits>&
expair<Traits>::operator*=(const expair &o) {
  ep::mul_rest(m_rest, o.m_rest);
  return *this *= o.m_coef;
}
template<class Traits>
inline expair<Traits>&
expair<Traits>::operator/=(const expair &o) {
  ep::div_rest(m_rest, o.m_rest);
  return *this /= o.m_coef;
}
template<class Traits>
inline expair<Traits>&
expair<Traits>::iinv() {
  ep::inv_coef(m_coef);
  ep::inv_rest(m_rest);
  return *this;
}

// comparison
template<class Traits>
inline int
expair<Traits>::compare(const expair &a, const expair &b)
{ return ep::compare_rest(a.m_rest, b.m_rest); }

template<class Traits>
int
expair<Traits>::deep_compare(const expair &a, const expair &b) {
  int c = compare(a,b);
  return c ? c
  : ep::compare_coef(a.m_coef, b.m_coef);
}

// hashing
template<class Traits>
std::size_t
expair<Traits>::calc_hash() const {
  return ep::hash_rest(m_rest);
}

template<class Traits>
std::size_t
expair<Traits>::deep_hash() const {
  std::size_t seed = calc_hash();
  boost::hash_combine(seed, ep::hash_coef(m_coef));
  return seed;
}

// printing
template<class Traits>
inline void
expair<Traits>::print(std::basic_ostream<char> &os) const
{ ep::print_pair(os, m_coef, m_rest); }

}


#endif // EXPAIR_IXX_
