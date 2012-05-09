#ifndef EXPAIRSEQ_OPERATION_TPP
#define EXPAIRSEQ_OPERATION_TPP

#include "container/unsafe_vector.hpp"
#include "analysis/vectorseq/poly.hpp"
#include "analysis/vectorseq/iterator.hpp"

#include <boost/range.hpp>
#include <boost/bind.hpp>

#include "util/assert.hpp"

namespace analysis {
namespace vseq {

template<class EP, class Poly>
Poly* do_neg(const Poly &a)
{
  std::unique_ptr< Poly >
    ret ( new Poly( a.size() ) );

  typedef typename EP::monomial_type const M;

  std::transform(
    a.begin(), a.end()
  , std::back_inserter( *ret )
  , [](const basic* p) { return EP::neg( static_cast<M*>(p) ); }
  );

  ret->shrink();

  // negating never cancels
  return ret.release();
}

template<class EP, class Poly>
Poly* do_sca(const Poly &a, const number &n)
{
  std::unique_ptr< Poly >
    ret ( new Poly( a.size() ) );

  typedef typename EP::monomial_type const M;

  std::transform(
    a.begin(), a.end()
  , std::back_inserter( *ret )
  , [&n](const basic* p) { return EP::sca( static_cast<M*>(p), n ); }
  );

  ret->shrink();

  // non-null scalar never cancels
  return ret.release();
}

template<class EP, class Poly>
Poly*
do_add(const Poly &a, const Poly &b)
{
  typedef typename EP::monomial_type M;

  std::unique_ptr<Poly> retp ( new Poly( a.size() + b.size() ) );
  Poly &ret = *retp;

  typedef eps_iterator<M, typename Poly::const_iterator> iterator;
  iterator
    i1 = a.begin(), e1 = a.end(),
    i2 = b.begin(), e2 = b.end();

  std::size_t d1 = a.size(), d2 = b.size();

  //! bottleneck loop :
  while( (d1 != 0) & (d2 != 0) )
    // integer null testing is faster than iterator::operator!=
  {
    //! hot point : comparison
    util::cmp_t c = EP::compare( *i1, *i2 );

    //! *i1 < *i2  then push *i1
    if( c < 0 )
    {
      ret.push_back( *i1 );
      ++i1; --d1;
      continue;
    }

    //! *i1 > *i2  then push *i2
    if( c != 0 ) // ie : c > 0
    {
      ret.push_back( *i2 );
      ++i2; --d2;
      continue;
    }

    //! collision case : *i1 >< *i2

    ret.push_back( EP::add( *i1, *i2 ) );
    const M* e = static_cast<const M*>( ret.back() );

    if( EP::null(e) )
      ret.pop_back();

    ++i1; --d1;
    ++i2; --d2;
  }

  std::copy( i1, e1, std::back_inserter(ret) );
  std::copy( i2, e2, std::back_inserter(ret) );

  ret.shrink();

  return ret.empty() ? nullptr : retp.release();
}

template<class EP, class Poly>
Poly*
do_sub(const Poly &a, const Poly &b)
{
  typedef typename EP::monomial_type M;

  std::unique_ptr<Poly> retp { new Poly( a.size() + b.size() ) };
  Poly &ret = *retp;

  typedef eps_iterator< M, typename Poly::const_iterator > iterator;
  iterator
    i1 = a.begin(), e1 = a.end(),
    i2 = b.begin(), e2 = b.end();

  std::size_t d1 = a.size(), d2 = b.size();

  //! bottleneck loop :
  while( (d1 != 0) & (d2 != 0) )
    // integer null testing is faster than iterator::operator!=
  {
    //! hot point : comparison
    util::cmp_t c = EP::compare( *i1, *i2 );

    //! *i1 < *i2  then push *i1
    if( c < 0 )
    {
      ret.push_back( *i1 );
      ++i1; --d1;
      continue;
    }

    //! *i1 > *i2  then push *i2
    if( c != 0 ) // ie : c > 0
    {
      ret.push_back( EP::neg( *i2 ) );
      ++i2; --d2;
      continue;
    }

    //! collision case : *i1 >< *i2

    ret.push_back( EP::sub( *i1, *i2 ) );
    auto e = static_cast<const M*>( ret.back() );

    if( EP::null(e) )
      ret.pop_back();

    ++i1; --d1;
    ++i2; --d2;
  }

  std::copy( i1, e1, std::back_inserter(ret) );
  std::transform( i2, e2, std::back_inserter(ret), EP::neg );

  ret.shrink();

  return ret.empty() ? nullptr : retp.release();
}

}} // namespace analysis::epseq

#endif
