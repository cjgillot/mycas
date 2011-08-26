#ifndef EXPAIRSEQ_IPP_
#define EXPAIRSEQ_IPP_

#include "analysis/numeric.hpp"
#include "analysis/expairseq.hpp"
#include "analysis/expairseq/operation.hpp"

#include <boost/range.hpp>

#include "util/null.hpp"
#include "util/foreach.hpp"

namespace analysis {

// ***** simple cdtor ***** //

template<class I, class M>
inline expairseq<I,M>::
expairseq(const number &n)
: m_coef( n )
, m_poly( /* null */ )
, m_hash( 0ul )
{}

template<class I, class M>
inline expairseq<I,M>::
~expairseq() {}

// ***** named constructors ***** //

template<class I, class M>
inline void expairseq<I,M>::
construct_monomial( const M* p )
{
  const epair &ep = p;
  m_poly.reset( new poly_t(1, ep) );
  m_hash = ep.hash();
}

// operation constructors
template<class I, class M>
inline void expairseq<I,M>::
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
inline void expairseq<I,M>::
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
inline void expairseq<I,M>::
construct_neg( const expairseq &a )
{
  if(a.m_poly)
    m_poly.reset( epseq::do_neg( *a.m_poly, m_hash ) );
}

template<class I, class M>
inline void expairseq<I,M>::
construct_sca( const number &n, const expairseq &a )
{
  if(a.m_poly)
    m_poly.reset( epseq::do_sca( *a.m_poly, n, m_hash ) );
}

template<class I, class M>
template<class Iter>
inline void expairseq<I,M>::
construct_mono_range( const Iter &beg, const Iter &en )
{
  CONCEPT_ASSERT(( boost::RandomAccessIteratorConcept<Iter> ));
  if( beg != en )
    m_poly.reset( epseq::do_range_const<epair>( beg, en, m_hash ) );
}

template<class I, class M>
template<class Iter>
inline void expairseq<I,M>::
construct_mutable_mono_range( const Iter &beg, const Iter &en )
{
  CONCEPT_ASSERT(( boost::Mutable_RandomAccessIteratorConcept<Iter> ));
  if( beg != en )
    m_poly.reset( epseq::do_range_mutable<epair>( beg, en, m_hash ) );
}

template<class I, class M>
template<class Iter, class EM, class NA>
inline void expairseq<I,M>::
construct_expr_range( Iter beg, const Iter &en, EM emono, NA nadd )
{
  CONCEPT_ASSERT(( boost::RandomAccessIteratorConcept<Iter> ));

  if( beg == en )
    return;

  std::vector<epair> seq;
  // quite a good start :
  //   exact if no numeric and no nested
  seq.reserve( std::distance( beg, en ) );

  for(; beg != en; ++beg)
  {
    const expr &ex = *beg;
    ex.eval();

    if( ex.is_numeric() )
      nadd( m_coef, ex.as_a<numeric>() );

    else if( ex.is_a<I>() )
    { // needs flattening
      const expairseq &eps = *ex.as_a<I>();

      // ex has been evaluated
      ASSERT( ! eps.empty() );

      raw_const_iterator nest_it = eps.raw_begin();
      const raw_const_iterator &nest_en = eps.raw_end();

      for( ; nest_it != nest_en; ++nest_it )
        seq.push_back( *nest_it );
    }

    else
      seq.push_back( emono( ex ) );
  }

  construct_mutable_mono_range( seq.begin(), seq.end() );
}

// ***** comparison ***** //
template<class I, class M>
inline std::size_t expairseq<I,M>::
hash() const
{
  std::size_t seed = 0;
  boost::hash_combine(seed, m_coef.hash());
  boost::hash_combine(seed, m_hash);
  return seed;
}

template<class I, class M>
inline util::cmp_t expairseq<I,M>::
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
inline util::cmp_t expairseq<I,M>::
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
  inline void
  operator()(const T &x)
  { x.print(os << ' '); }
};

}

template<class I, class M>
inline void expairseq<I,M>::
print(std::basic_ostream<char> &os) const
{
  os << '(';
  static_cast<const I*>( this )->print_base(os);
  m_coef.print(os << ' ');
  if(m_poly) boost::for_each(*m_poly, detail::printer(os));
  os << ')';
}

template<class I, class M>
inline bool expairseq<I,M>::
has(const symbol &s) const
{
  if(! m_poly) return false;

  foreach(const epair &e, *m_poly)
    if( e.ptr()->has(s) )
      return true;

  return false;
}

template<class I, class M>
template<class F>
inline std::vector<expr>* expairseq<I,M>::
map_children( F f ) const
{
  if( ! m_poly )
    return nullptr;

  raw_const_iterator
    it = m_poly->begin(),
    en = m_poly->end();

  for( ; it != en; ++it )
  {
    const expr &r = f( it->ptr() );

    if( r.get() != it->ptr() )
    {
      // something has changed
      // create a new vector
      typedef std::vector< expr > vec_t;
      util::move_ptr< vec_t > retp ( new vec_t );
      vec_t &ret = *retp;
      ret.reserve( m_poly->size() );

      // unchanged terms
      for( raw_const_iterator i2 = m_poly->begin(); i2 != it; ++i2 )
        ret.push_back( expr( i2->ptr() ) );

      // current term
      ret.push_back( r );
      ++it;

      // remaining terms
      for( ; it != en; ++it )
        ret.push_back( f( it->ptr() ) );

      return retp.release();
    }
  }

  // nothing has changed
  return nullptr;
}


} // namespace analysis

#endif // EXPAIRSEQ_IPP
