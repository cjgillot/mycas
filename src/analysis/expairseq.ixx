#ifndef EXPAIRSEQ_IXX_
#define EXPAIRSEQ_IXX_

#include "stdlib.hxx"

#include "analysis/numeric.hxx"
#include "analysis/expairseq.hxx"

#include "polynomial/sparse/poly.hxx"

#include "analysis/expair.ixx"

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
    if(! r2) return;
    if(! r1) {
      r1 = r2;
      return;
    }
    *r1.cow() += *r2;
  }
  static inline void
  div_rest(rest_type &r1, const rest_type &r2) {
    // the division of rest
    // is a polynomial subtraction
    if(! r2) return;
    if(! r1) {
      r1 = r2;
      inv_rest(r1);
      return;
    }
    *r1.cow() -= *r2;
  }
  static inline void
  inv_rest(rest_type &r1) {
    // inversion of rest
    // is a polynomial negation
    if(! r1) return;
    r1.cow()->ineg();
  }

  // expair::print
  template<class S>
  static void
  print_pair(S &os, const coef_type &e, const rest_type &b);
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
  (S &os, const coef_type &c, const rest_type &r) {

  os << '(';
  super::print_base(os);
  os << ' ' << c;
  if(r)
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
template<class T, class MT>
inline
expairseq<T,MT>::expairseq() {}

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
: m_impl(n, 0) {}

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
  const typename traits::poly_t* rp = m_impl.m_rest.get();
  if(rp && rp->null())
    m_impl.m_rest.reset();
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
{ return !m_impl.m_rest; }
template<class T, class MT>
inline bool
expairseq<T,MT>::is_mono() const
{ return m_impl.m_rest && m_impl.m_rest->monome(); }

template<class T, class MT>
inline typename expairseq<T, MT>::epair const&
expairseq<T,MT>::mono() const
{ assert(is_mono()); return *m_impl.m_rest->begin(); }

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

/*

namespace {

template<class EP>
struct eval_push
: std::unary_function<const EP &, void> {
  std::deque<const basic*> &cont;
  unsigned lv;

  eval_push(std::deque<const basic*> &c, unsigned l)
  : cont(c), lv(l) {}

  void
  operator()(const EP &x)
  { cont.push_back(x.eval(lv)); }
};

}

template<class Impl>
std::vector<const basic*>*
expairseq<Impl>::eval_children(unsigned lv) const {
  assert(m_impl.m_rest);

  if(lv == 0)
    return 0;

  --lv;

  const poly_t &m_poly = *m_impl.m_rest;

  typedef typename poly_t::const_iterator cit;
  cit b = m_poly.begin(), e = m_poly.end();
  for(; b != e; ++b) {
    const EP &ep = *b;

    // something has changed
    if(ep.eval(lv)) {
      std::vector<const basic*>* ret = new std::vector<const basic*>;
      ret->reserve(m_poly.size());

      std::transform(m_poly.begin(), b, std::identity(), std::back_inserter(*ret));
      ret->push_back(ep);
      std::for_each(++b, e, eval_push<EP>(*ret, lv));
      return ret;
    }
  }

  return 0;
}



template<class EP>
const basic*
expairseq<EP>::eval(unsigned lv) const {

  if(! m_impl.m_rest) return m_coef.eval(lv);

  const poly_t &m_poly = *m_impl.m_rest;

  assert(! m_poly.null());

  if(m_coef.unit() && m_poly.monome())
    return

  if(lv == 0) return this;

  std::auto_ptr<std::deque<const basic*> >
    children = eval_children(lv);

  if(!children.get())
    // nothing has changed
    return this;

  std::auto_ptr<expairseq> p2 ( create() );

  foreach(const basic* b, *children)
    p2->imul(b->as_mul());

  if(!p2->pimpl || p2->pimpl->empty())
    return p2->m_coef.eval(lv);

  if(p2->m_coef.unit() && p2->m_poly.monome())
    return p2->m_poly().eval(lv-1);

  return p2.release();
}

*/

}


#endif // EXPAIRSEQ_IXX_
