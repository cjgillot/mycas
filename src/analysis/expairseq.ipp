#ifndef EXPAIRSEQ_IXX_
#define EXPAIRSEQ_IXX_

#include "stdlib.hpp"

#include "analysis/numeric.hpp"
#include "analysis/expairseq.hpp"

#include "polynomial/sparse/poly.hpp"

#include "analysis/expair.ipp"

namespace analysis {

template<class Traits, class MonoTraits>
struct expairseq_traits<Traits, MonoTraits>::ep
: public Traits::ep {

  typedef typename Traits::ep super;

  // expair::null testing
  static inline bool
  null_pair(const coef_type &c, const rest_type &)
  { return super::null_coef(c); }

  // expair::compare and expair::deep_compare
  static inline int
  compare_coef(const coef_type &c1, const coef_type &c2)
  { return coef_type::compare(c1, c2); }
  static inline int
  compare_rest(const rest_type &r1, const rest_type &r2)
  { return poly_t::compare(*r1.get(), *r2.get()); }

// all coef operations :

// using importation doesn't work
// when a particuliar operation is not defined in super
#define IMPORT_BIN_OP(name)               \
  static void name##_coef                 \
    (coef_type &c1, const coef_type &c2)  \
  { super::name##_coef(c1,c2); }
#define IMPORT_UN_OP(name)                \
  static void name##_coef                 \
    (coef_type &c1)                       \
  { super::name##_coef(c1); }

  IMPORT_BIN_OP(add)
  IMPORT_BIN_OP(sub)
  IMPORT_BIN_OP(mul)
  IMPORT_BIN_OP(div)

  IMPORT_UN_OP(neg)
  IMPORT_UN_OP(inv)

#undef IMPORT_BIN_OP
#undef IMPORT_UN_OP

  // all rest operations :
  static inline void
  mul_rest(rest_type &r1, const rest_type &r2) {
    // the multiplication of rest
    // is a polynomial addition
    *r1.cow() += *r2;
  }
  static inline void
  div_rest(rest_type &r1, const rest_type &r2) {
    // the division of rest
    // is a polynomial subtraction
    *r1.cow() -= *r2;
  }
  static inline void
  inv_rest(rest_type &r1) {
    // inversion of rest
    // is a polynomial negation
    r1.cow()->ineg();
  }

  // expair::print
  template<class S>
  static void
  print_pair(S &os, const coef_type &e, const rest_type &b);

  // expair::*_hash
  static std::size_t hash_coef(const coef_type &c)
  { return c.get_hash(); }
  static std::size_t hash_rest(const rest_type &r) {
    std::size_t seed = 0;
    foreach(const epair &ep, *r) {
      seed ^= ep.deep_hash();
    }
    return seed;
  }
};

namespace {

struct printer {
  std::basic_ostream<char> &os;

  printer(std::basic_ostream<char> &s)
  : os(s) {}

  printer(const printer &o)
  : os(o.os) {}

  template<class T>
  inline void
  operator()(const T &x)
  { os << " "; x.print(os); }

private:
  printer();
};

}

template<class Impl, class Mono>
template<class S>
void
expairseq_traits<Impl, Mono>::ep::print_pair
  (S &os, const coef_type &c, const rest_type &r)
{
  os << '(';
  super::print_base(os);
  os << ' ' << c;
  r->transform(printer(os));
  os << ')';
}


// expair coercion
template<class T, class MT>
inline
expairseq<T,MT>::expairseq(const impl_t &i)
: m_impl(i) {}
template<class T, class MT>
inline
expairseq<T,MT>::operator expairseq<T,MT>::impl_t() const
{ return m_impl; }

// ctor
// template<class T, class MT>
// inline
// expairseq<T,MT>::expairseq() {}

template<class T, class MT>
inline
expairseq<T,MT>::expairseq(const expairseq &o)
: basic(o), m_impl(o.m_impl) {}

template<class T, class MT>
inline void
expairseq<T,MT>::swap(expairseq &o)
{ m_impl.swap(o.m_impl); }


template<class T, class MT>
inline
expairseq<T,MT>::expairseq(const coef_type &n)
: m_impl(n, new poly_t) {}

template<class T, class MT>
inline
expairseq<T,MT>::expairseq(const coef_type &n, const epair &p)
: m_impl(n, new poly_t(p)) {}

template<class T, class MT>
inline
expairseq<T,MT>::expairseq(const coef_type &n, const poly_t* i)
: m_impl(n, i) {}

template<class T, class MT>
inline
expairseq<T,MT>::~expairseq() {}


// canonicalize
template<class T, class MT>
inline void
expairseq<T,MT>::canonicalize() {
  // nothing to do
}

// operations
template<class T, class MT>
inline expairseq<T,MT> &
expairseq<T,MT>::iadd(const expairseq &o) {
  // an expairseq addition is an expair multiplication
  m_impl *= o.m_impl;
  canonicalize();
  return *this;
}

template<class T, class MT>
inline expairseq<T,MT> &
expairseq<T,MT>::isub(const expairseq &o) {
  m_impl /= o.m_impl;
  canonicalize();
  return *this;
}

template<class T, class MT>
inline expairseq<T,MT> &
expairseq<T,MT>::ineg() {
  m_impl.iinv();
  canonicalize();
  return *this;
}

template<class T, class MT>
inline expairseq<T,MT> &
expairseq<T,MT>::imul(const coef_type &n) {
  m_impl *= n;
  canonicalize();
  return *this;
}

// access
template<class T, class MT>
inline typename expairseq<T, MT>::coef_type const &
expairseq<T,MT>::coef() const
{ return m_impl.m_coef; }

template<class T, class MT>
inline bool
expairseq<T,MT>::is_empty() const
{ return m_impl.m_rest->null(); }
template<class T, class MT>
inline bool
expairseq<T,MT>::is_mono() const
{ return m_impl.m_rest->monome(); }

template<class T, class MT>
inline typename expairseq<T, MT>::epair const&
expairseq<T,MT>::mono() const
{ assert(is_mono()); return *m_impl.m_rest->begin(); }

template<class T, class MT>
inline std::size_t
expairseq<T,MT>::calc_hash() const
{ return m_impl.deep_hash(); }

template<class T, class MT>
inline int
expairseq<T,MT>::compare_same_type(const basic &o) const {
  return impl_t::deep_compare(
    m_impl
  , static_cast<const expairseq &>(o).m_impl
  );
}

template<class T, class MT>
inline void
expairseq<T,MT>::print(std::basic_ostream<char> &os) const
{ m_impl.print(os); }

}


#endif // EXPAIRSEQ_IXX_
