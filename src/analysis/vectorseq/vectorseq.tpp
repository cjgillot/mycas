#ifndef EXPAIRSEQ_TPP_
#define EXPAIRSEQ_TPP_

#include "analysis/expr.hpp"
#include "analysis/vectorseq/vectorseq.hpp"

#include "analysis/vectorseq/vectorseq.ipp"

#include "analysis/vectorseq/operation.tpp"

#include <boost/range/algorithm/for_each.hpp>

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
  * Given two \c vectorseq, these constructors build
  * a merge depending on the call tag.
  *
  * Complexity : linear
  *   (both in \ref epair copies and comparisons)
  *
  * \{
  */
//@{
template<class I, class M>
void vectorseq<I,M>::
construct_add( const vectorseq &a, const vectorseq &b )
{
  vectorseq_base::construct_add( a, b );

  if( ! b.m_poly )
    m_poly = a.m_poly;

  else if( ! a.m_poly )
    m_poly = b.m_poly;

  else
    m_poly.reset( epseq::do_add( *a.m_poly, *b.m_poly ) );
}

template<class I, class M>
void vectorseq<I,M>::
construct_sub( const vectorseq &a, const vectorseq &b )
{
  vectorseq_base::construct_sub( a, b );

  if( ! b.m_poly )
    m_poly = a.m_poly;

  else if( ! a.m_poly )
    m_poly.reset( epseq::do_neg( *b.m_poly ) );

  else
    m_poly.reset( epseq::do_sub( *a.m_poly, *b.m_poly ) );
}

template<class I, class M>
void vectorseq<I,M>::
construct_neg( const vectorseq &a )
{
  vectorseq_base::construct_neg( a );

  if(a.m_poly)
    m_poly.reset( epseq::do_neg( *a.m_poly ) );
}

template<class I, class M>
void vectorseq<I,M>::
construct_sca( const number &n, const vectorseq &a )
{
  vectorseq_base::construct_sca( n, a );

  if(a.m_poly)
    m_poly.reset( epseq::do_sca( *a.m_poly, n ) );
}
//@}
/*!\}*/

// range -> .ipp

// ***** comparison ***** //
template<class I, class M>
util::cmp_t vectorseq<I,M>::
partial_compare(const vectorseq &o) const
{
  util::cmp_t c = vectorseq_base::partial_compare( o );
  if( c ) return c;

  // trivial case
  if( m_poly.get() == o.m_poly.get() )
    return 0;

  if( ! m_poly )
    return -1; // since m_poly != o.m_poly
  else if( ! o.m_poly )
    return +1;

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

  for(; d1 != 0; --d1)
  {
    c = epair::deep_compare( *i1, *i2 );
    if(c) return c;
  }

  // now, we know we have equality -> unify
  util::unify_ptr(
    const_cast<vectorseq&>(*this).m_poly
  , const_cast<vectorseq&>(o)    .m_poly
  );
  return 0;
}

template<class I, class M>
util::cmp_t vectorseq<I,M>::
compare_same_type(const basic &o_) const
{
  const vectorseq &o = static_cast<const vectorseq&>( o_ );
  util::cmp_t c = vectorseq_base::compare_same_type( o );
  if(c) return c;
  return partial_compare(o);
}

namespace detail {

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
void vectorseq<I,M>::
print(std::basic_ostream<char> &os) const
{
  os << '(';
  static_cast<const I*>( this )->print_base(os);
  coef().print(os << ' ');
  if(m_poly) boost::for_each(*m_poly, detail::printer(os));
  os << ')';
}

template<class I, class M>
bool vectorseq<I,M>::
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
