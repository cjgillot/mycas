#ifndef EXPAIRSEQ_IPP_
#define EXPAIRSEQ_IPP_

#include "stdlib.hpp"
#include "algebra/compare.hpp"

#include "analysis/numeric.hpp"
#include "analysis/expairseq.hpp"

namespace analysis {

template<class I, class M>
inline
expairseq<I,M>::expairseq(const expairseq &o)
: basic(o), m_coef(o.m_coef), m_poly(o.m_poly) {}

template<class I, class M>
inline void
expairseq<I,M>::swap(expairseq &o) {
  basic::swap(o);
  m_coef.swap(o.m_coef);
  m_poly.swap(o.m_poly);
}


template<class I, class M>
inline
expairseq<I,M>::expairseq(const number &n)
: m_coef(n), m_poly( /* null */ ), m_hash(0) {}

template<class I, class M>
expairseq<I,M>::expairseq(const number &n, const epair &p)
: m_coef(n), m_poly(new poly_t(1, p)), m_hash(p.hash()) {}

template<class I, class M>
expairseq<I,M>::~expairseq() {}


// operations
namespace {

template<class T>
struct neg_hash
: std::unary_function<T, T> {
  std::size_t &h;

  neg_hash(std::size_t &h_)
  : h(h_) {}

  inline T operator()(const T &o) {
    T ret = - o;
    h ^= ret.hash();
    return ret;
  }
};

template<class T, class Sca>
struct sca_hash
: std::unary_function<T, T> {
  std::size_t &h;
  const Sca &n;

  sca_hash(std::size_t &h_, const Sca &n_)
  : h(h_), n(n_) {}

  inline T operator()(const T &o) {
    T ret = o.sca(n);
    h ^= ret.hash();
    return ret;
  }
};

template<class Range>
void do_neg(Range &ret, const Range &a, std::size_t &hash) {
  ret.reserve( a.size() );
  typedef typename boost::range_value<const Range>::type epair;
  boost::transform(a, std::back_inserter(ret), neg_hash<epair>(hash));
}

template<class Range, class Sca>
void do_sca(Range &ret, const Range &a, const Sca &n, std::size_t &hash) {
  ret.reserve( a.size() );
  typedef typename boost::range_value<const Range>::type epair;
  boost::transform(a, std::back_inserter(ret), sca_hash<epair, Sca>(hash, n));
}

template<class Range>
void do_add(Range &ret, const Range &a, const Range &b, std::size_t &hash) {

  ret.reserve(a.size() + b.size());

  typedef typename boost::range_value<const Range>::type epair;

  typename boost::range_iterator<const Range>::type
    i1 = a.begin(), e1 = a.end(),
    i2 = b.begin(), e2 = b.end();

  while(i1 != e1 && i2 != e2) {
    int c = epair::compare( *i1, *i2 );

    if(c  < 0) {
      ret.push_back( *i1 );
      // hashing already done
      ++i1;
      continue;
    }
    if(c != 0) {
      ret.push_back( *i2 );
      // hashing already done
      ++i2;
      continue;
    }

    // cancel hash
    hash ^= i1->hash() ^ i2->hash();

    ret.push_back( *i1 + *i2 );
    const epair &e = ret.back();

    if( e.null() )
      ret.pop_back();
    else
      hash ^= e.hash();
  }

  std::copy(i1, e1, std::back_inserter(ret));
  std::copy(i2, e2, std::back_inserter(ret));
}

template<class Range>
void do_sub(Range &ret, const Range &a, const Range &b, std::size_t &hash) {

  ret.reserve(a.size() + b.size());

  typedef typename boost::range_value<const Range>::type epair;

  typename boost::range_iterator<const Range>::type
    i1 = a.begin(), e1 = a.end(),
    i2 = b.begin(), e2 = b.end();

  while(i1 != e1 && i2 != e2) {
    int c = epair::compare( *i1, *i2 );

    if(c  < 0) {
      ret.push_back( *i1 );
      // hashing already done
      ++i1;
      continue;
    }
    if(c != 0) {
      ret.push_back( - *i2 );
      hash ^= ret.back().hash();
      ++i2;
      continue;
    }

    // cancel hash
    hash ^= i1->hash();

    ret.push_back( *i1 + *i2 );
    epair &e = ret.back();

    if( e.null() )
      ret.pop_back();
    else
      hash ^= e.hash();
  }

  std::copy(i1, e1, std::back_inserter(ret));
  std::transform(i2, e2, std::back_inserter(ret), neg_hash<epair>(hash));
}

}

template<class I, class M>
inline
expairseq<I,M>::expairseq(const expairseq &a, const expairseq &b, add_t)
: m_coef(I::ep::add(a.m_coef, b.m_coef))
, m_poly( /* null */ )
, m_hash(a.m_hash ^ b.m_hash) {

  if( ! a.m_poly )
    m_poly = b.m_poly;

  else if( ! b.m_poly )
    m_poly = a.m_poly;

  else {
    m_poly.reset( new poly_t );
    do_add(*m_poly, *a.m_poly, *b.m_poly, m_hash);
  }

}

template<class I, class M>
inline
expairseq<I,M>::expairseq(const expairseq &a, const expairseq &b, sub_t)
: m_coef(I::ep::sub(a.m_coef, b.m_coef))
, m_poly(new poly_t)
, m_hash(a.m_hash) {

  if( ! b.m_poly )
      m_poly = a.m_poly;

  else {
    m_poly.reset( new poly_t );

    if( ! a.m_poly )
      do_neg(*m_poly, *b.m_poly, m_hash);

    else
      do_add(*m_poly, *a.m_poly, *b.m_poly, m_hash);
  }

}

template<class I, class M>
inline
expairseq<I,M>::expairseq(const expairseq &a, neg_t)
: m_coef( I::ep::neg( a.m_coef ) )
, m_poly( /* null */ )
, m_hash(0) {

  if(a.m_poly) {
    m_poly.reset( new poly_t );
    do_neg(*m_poly, *a.m_poly, m_hash);
  }
}

template<class I, class M>
inline
expairseq<I,M>::expairseq(const expairseq &a, const number &n, sca_t)
: m_coef(I::ep::mul(a.m_coef, n))
, m_poly( /* null */ )
, m_hash(0) {

  if(a.m_poly) {
    m_poly.reset( new poly_t );
    do_sca(*m_poly, *a.m_poly, n, m_hash);
  }
}

// access
template<class I, class M>
inline const number&
expairseq<I,M>::coef() const
{ return m_coef; }

template<class I, class M>
inline bool
expairseq<I,M>::is_empty() const
{ return m_poly->empty(); }
template<class I, class M>
inline bool
expairseq<I,M>::is_mono() const
{ return m_poly->size() == 1; }

template<class I, class M>
inline typename expairseq<I,M>::epair const&
expairseq<I,M>::mono() const
{ assert(is_mono()); return *m_poly->begin(); }

template<class I, class M>
inline std::size_t
expairseq<I,M>::calc_hash() const {
  std::size_t seed = basic::calc_hash();
  boost::hash_combine(seed, m_coef.get_hash());
  boost::hash_combine(seed, m_hash);
  return seed;
}

template<class I, class M>
inline int
expairseq<I,M>::partial_compare(const expairseq &o) const {
  if(m_poly.get() == o.m_poly.get()) return 0;

  int c = m_hash - o.m_hash;
  if(c) return c;

  c = algebra::range_compare(*m_poly, *o.m_poly, epair::deep_compare);

  // if(c == 0) util::unify_ptr(m_poly, o.m_poly);
  return c;
}

template<class I, class M>
inline int
expairseq<I,M>::compare_same_type(const basic &o_) const {
  const expairseq &o = static_cast<const expairseq&>(o_);
  int c = number::compare(m_coef, o.m_coef);
  if(c) return c;
  return partial_compare(o);
}

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
  { x.print(os << ' '); }

private:
  printer();
};

}

template<class I, class M>
inline void
expairseq<I,M>::print(std::basic_ostream<char> &os) const {
  os << '(';
  print_base(os);
  m_coef.print(os << ' ');
  boost::for_each(*m_poly, printer(os));
  os << ')';
}

}


#endif // EXPAIRSEQ_IPP_
