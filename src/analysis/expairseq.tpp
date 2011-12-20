#ifndef EXPAIRSEQ_TPP_
#define EXPAIRSEQ_TPP_

#include "analysis/numeric.hpp"
#include "analysis/expairseq.hpp"

#include "analysis/expairseq.ipp"

#include "analysis/expairseq/operation.tpp"

#include <boost/range.hpp>

#include "util/null.hpp"
#include "util/foreach.hpp"

namespace analysis {

// ***** simple cdtor ***** //

// -> .ipp

// ***** named constructors ***** //

// -> .ipp

// operation constructors
/*!\name Operation constructors
  *
  * Given two \c expairseq, these constructors build
  * a merge depending on the call tag.
  *
  * Complexity : linear
  *   (both in \ref epair copies and comparisons)
  *
  * \{
  */
//@{
template<class I, class M>
void expairseq<I,M>::
construct_add( const expairseq &a, const expairseq &b )
{
  m_hash = a.m_hash ^ b.m_hash;

  if( ! b.m_poly )
    m_poly = a.m_poly;

  else if( ! a.m_poly )
    m_poly = b.m_poly;

  else
    m_poly.reset( epseq::do_add( *a.m_poly, *b.m_poly, m_hash ) );
}

template<class I, class M>
void expairseq<I,M>::
construct_sub( const expairseq &a, const expairseq &b )
{
  m_hash = a.m_hash;

  if( ! b.m_poly )
    m_poly = a.m_poly;

  else if( ! a.m_poly )
    m_poly.reset( epseq::do_neg( *b.m_poly, m_hash ) );

  else
    m_poly.reset( epseq::do_sub( *a.m_poly, *b.m_poly, m_hash ) );
}

template<class I, class M>
void expairseq<I,M>::
construct_neg( const expairseq &a )
{
  if(a.m_poly)
    m_poly.reset( epseq::do_neg( *a.m_poly, m_hash ) );
}

template<class I, class M>
void expairseq<I,M>::
construct_sca( const number &n, const expairseq &a )
{
  if(a.m_poly)
    m_poly.reset( epseq::do_sca( *a.m_poly, n, m_hash ) );
}
//@}
/*!\}*/

// range -> .ipp

// ***** comparison ***** //
template<class I, class M>
std::size_t expairseq<I,M>::
hash() const
{
  std::size_t seed = 0;
  boost::hash_combine(seed, m_coef.hash());
  boost::hash_combine(seed, m_hash);
  return seed;
}

template<class I, class M>
util::cmp_t expairseq<I,M>::
partial_compare(const expairseq &o) const
{
  // trivial case
  if( m_poly.get() == o.m_poly.get() )
    return 0;

  if( ! m_poly )
    return -1; // since m_poly != o.m_poly
  else if( ! o.m_poly )
    return +1;

  util::cmp_t c
    = util::compare( m_hash, o.m_hash );
  if(c) return c;

  const poly_t
    &a = *  m_poly
  , &b = *o.m_poly;

  // compare sizes
  // doing like this saves loops,
  // since size differences are very common
  std::size_t
    d1 = a.size()
  , d2 = b.size();

  c = util::compare( d1, d2 );
  if(c) return c;

  // lexicographical comparison now
  typename poly_t::const_iterator
    i1 = a.begin(), i2 = b.begin();

  for(; d1 != 0; --d1) {
    c = epair::deep_compare( *i1, *i2 );
    if(c)
      return c;
  }

  // now, we know we have equality -> unify
  util::unify_ptr(
    const_cast<expairseq&>(*this).m_poly
  , const_cast<expairseq&>(o)    .m_poly
  );
  return 0;
}

template<class I, class M>
util::cmp_t expairseq<I,M>::
compare_same_type(const basic &o_) const
{
  const expairseq &o = static_cast<const expairseq&>(o_);
  util::cmp_t c
    = number::compare(m_coef, o.m_coef);
  if(c) return c;
  return partial_compare(o);
}

namespace detail
{

struct printer
{
  std::basic_ostream<char> &os;

  printer(std::basic_ostream<char> &s)
  : os(s) {}

  printer(const printer &o)
  : os(o.os) {}

  template<class T>
   void
  operator()(const T &x)
  { expr(x).print(os << ' '); }
};

}

template<class I, class M>
void expairseq<I,M>::
print(std::basic_ostream<char> &os) const
{
  os << '(';
  static_cast<const I*>( this )->print_base(os);
  m_coef.print(os << ' ');
  if(m_poly) boost::for_each(*m_poly, detail::printer(os));
  os << ')';
}

template<class I, class M>
bool expairseq<I,M>::
has(const symbol &s) const
{
  if(! m_poly) return false;

  foreach(const epair &e, *m_poly)
    if( e.get()->has(s) )
      return true;

  return false;
}


} // namespace analysis

#endif // EXPAIRSEQ_IPP
