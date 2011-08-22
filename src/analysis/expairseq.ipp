#ifndef EXPAIRSEQ_IPP_
#define EXPAIRSEQ_IPP_

#include "util/null.hpp"

#include "algebra/compare.hpp"

#include "analysis/numeric.hpp"
#include "analysis/expairseq.hpp"

#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

#include "analysis/expairseq/operation.hpp"

namespace analysis {

template<class I, class M>
inline
expairseq<I,M>::expairseq(const number &n)
: m_coef( n )
, m_poly( /* null */ )
, m_hash( 0ul )
{}

template<class I, class M>
expairseq<I,M>::expairseq(const number &n, const epair &p)
: m_coef( n )
, m_poly( new poly_t(1, p) )
, m_hash( p.hash() )
{}

template<class I, class M>
expairseq<I,M>::~expairseq() {}

// operation constructors
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

  else
    m_poly.reset( epseq::detail::do_add( *a.m_poly, *b.m_poly, m_hash ) );

}

template<class I, class M>
inline
expairseq<I,M>::expairseq(const expairseq &a, const expairseq &b, sub_t)
: m_coef(I::ep::sub(a.m_coef, b.m_coef))
, m_poly( /* null */ )
, m_hash(a.m_hash) {

  if( ! b.m_poly )
      m_poly = a.m_poly;

  else {
    if( ! a.m_poly )
      m_poly.reset( epseq::detail::do_neg( *b.m_poly, m_hash ) );

    else
      m_poly.reset( epseq::detail::do_sub( *a.m_poly, *b.m_poly, m_hash ) );
  }

}

template<class I, class M>
inline
expairseq<I,M>::expairseq(const expairseq &a, neg_t)
: m_coef( I::ep::neg( a.m_coef ) )
, m_poly( /* null */ )
, m_hash(0) {

  if(a.m_poly)
    m_poly.reset( epseq::detail::do_neg(*a.m_poly, m_hash) );

}

template<class I, class M>
inline
expairseq<I,M>::expairseq(const expairseq &a, const number &n, sca_t)
: m_coef(I::ep::mul(a.m_coef, n))
, m_poly( /* null */ )
, m_hash(0) {

  if(a.m_poly)
    m_poly.reset( epseq::detail::do_sca(*a.m_poly, n, m_hash) );

}

// access
template<class I, class M>
inline const number&
expairseq<I,M>::coef() const
{ return m_coef; }

template<class I, class M>
inline bool
expairseq<I,M>::is_empty() const
{ return ! m_poly; }
template<class I, class M>
inline bool
expairseq<I,M>::is_mono() const
{ return m_poly && m_poly->size() == 1; }

template<class I, class M>
inline typename expairseq<I,M>::epair const&
expairseq<I,M>::mono() const
{ assert(is_mono()); return *m_poly->begin(); }

template<class I, class M>
inline std::size_t
expairseq<I,M>::hash() const {
  std::size_t seed = 0;
  boost::hash_combine(seed, m_coef.hash());
  boost::hash_combine(seed, m_hash);
  return seed;
}

template<class I, class M>
inline util::cmp_t
expairseq<I,M>::partial_compare(const expairseq &o) const {
  if( m_poly.get() == o.m_poly.get() ) return 0;

  if( ! m_poly )
    return o.m_poly ? -1 : 0;
  else if( ! o.m_poly )
    return 1;

  util::cmp_t c
    = util::compare( m_hash, o.m_hash );
  if(c) return c;

  const poly_t
    &a = *  m_poly
  , &b = *o.m_poly;

  std::size_t
    d1 = a.size()
  , d2 = b.size();

  c = util::compare( d1, d2 );
  if(c) return c;

  typename poly_t::const_iterator
    i1 = a.begin(), i2 = b.begin();

  for(; d1 != 0; --d1) {
    c = epair::deep_compare( *i1, *i2 );
    if(c)
      return c;
  }

  util::unify_ptr(
    const_cast<expairseq&>(*this).m_poly
  , const_cast<expairseq&>(o)    .m_poly
  );
  return 0;
}

template<class I, class M>
inline util::cmp_t
expairseq<I,M>::compare_same_type(const basic &o_) const {
  const expairseq &o = static_cast<const expairseq&>(o_);
  util::cmp_t c
    = number::compare(m_coef, o.m_coef);
  if(c) return c;
  return partial_compare(o);
}

namespace detail {

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
  static_cast<const I*>( this )->print_base(os);
  m_coef.print(os << ' ');
  if(m_poly) boost::for_each(*m_poly, detail::printer(os));
  os << ')';
}

template<class I, class M>
inline bool
expairseq<I,M>::has(const symbol &s) const {
  if(! m_poly) return false;

  foreach(const epair &e, *m_poly)
    if( e.ptr()->has(s) )
      return true;

  return false;
}

}


#endif // EXPAIRSEQ_IPP_
