#ifndef EXPAIRSEQ_OPERATION_HPP
#define EXPAIRSEQ_OPERATION_HPP

#include "analysis/expairseq/poly.hpp"

#include <memory> // auto_ptr
#include <functional>

namespace analysis {
namespace epseq {
namespace detail {

template<class T>
struct neg_hash
: std::unary_function<T, T> {
  std::size_t &h;

  neg_hash(std::size_t &h_)
  : h(h_) {}

  T operator()(const T &o) {
    const T &ret = - o;
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

  T operator()(const T &o) {
    const T &ret = o.sca(n);
    h ^= ret.hash();
    return ret;
  }
};

template<class epair>
unsafe_vector<epair>*
do_neg(
    const unsafe_vector<epair> &a
  , std::size_t &hash
) {
  typedef unsafe_vector<epair> vector_type;
  std::auto_ptr<vector_type>
    ret ( new vector_type( a.size() ) );

  std::transform(
    a.begin(), a.end()
  , std::back_inserter( *ret )
  , neg_hash<epair>( hash )
  );

  // negating never cancels
  return ret.release();
}

template<class epair, class Sca>
unsafe_vector<epair>*
do_sca(
    const unsafe_vector<epair> &a
  , const Sca &n
  , std::size_t &hash
) {
  typedef unsafe_vector<epair> vector_type;
  std::auto_ptr<vector_type>
    ret ( new vector_type( a.size() ) );

  std::transform(
    a.begin(), a.end()
  , std::back_inserter( *ret )
  , sca_hash<epair,Sca>( hash, n )
  );

  // non-null scalar never cancels
  return ret.release();
}

/*!
 * \brief Polynomial addition function
 *
 * This function takes two \c vector s and merge them
 */
template<class epair>
unsafe_vector<epair>*
do_add(
    const unsafe_vector<epair> &a
  , const unsafe_vector<epair> &b
  , std::size_t &hash
) {
  //! \invariant \c hash already contains <tt>hash_value(a) ^ hash_value(b)</tt>

  typedef unsafe_vector<epair> vector_type;
  std::auto_ptr<vector_type>
    retp ( new vector_type( a.size() + b.size() ) );
  vector_type &ret = *retp;

  typename vector_type::const_iterator
    i1 = a.begin(), e1 = a.end(),
    i2 = b.begin(), e2 = b.end();

  std::size_t d1 = a.size(), d2 = b.size();

  //! bottleneck loop :
  while( (d1 != 0) & (d2 != 0) )
    // integer null testing is faster than iterator::operator!=
    // and simple & only since short-circuit is useless
  {
    //! hot point : comparison
    util::cmp_t c = epair::compare( *i1, *i2 );

    //! *i1 < *i2  then push *i1
    if( c < 0 )
    {
      ret.push_back( *i1 );
      // hashing already done

      ++i1; --d1;
      continue;
    }

    //! *i1 > *i2  then push *i2
    if( c != 0 ) // ie : c > 0
    {
      ret.push_back( *i2 );
      // hashing already done

      ++i2; --d2;
      continue;
    }

    //! collision case : *i1 >< *i2

    // cancel hash
    hash ^= i1->hash() ^ i2->hash();

    ret.push_back( *i1 + *i2 );
    const epair &e = ret.back();

    if( e.null() )
      ret.pop_back();
    else
      hash ^= e.hash();

    ++i1; --d1;
    ++i2; --d2;
  }

  std::copy( i1, e1, std::back_inserter(ret) );
  std::copy( i2, e2, std::back_inserter(ret) );

  return ret.empty() ? 0 : retp.release();
}

template<class epair>
unsafe_vector<epair>*
do_sub(
    const unsafe_vector<epair> &a
  , const unsafe_vector<epair> &b
  , std::size_t &hash
) {
  //! \invariant \c hash already contains <tt>hash_value(a)</tt>

  typedef unsafe_vector<epair> vector_type;
  std::auto_ptr<vector_type>
    retp ( new vector_type( a.size() + b.size() ) );
  vector_type &ret = *retp;

  typename vector_type::const_iterator
    i1 = a.begin(), e1 = a.end(),
    i2 = b.begin(), e2 = b.end();

  std::size_t d1 = a.size(), d2 = b.size();

  //! bottleneck loop :
  while( (d1 != 0) & (d2 != 0) )
    // integer null testing is faster than iterator::operator!=
    // and simple & only since short-circuit is useless
  {
    //! hot point : comparison
    util::cmp_t c = epair::compare( *i1, *i2 );

    //! *i1 < *i2  then push *i1
    if( c < 0 )
    {
      ret.push_back( *i1 );
      // hashing already done

      ++i1; --d1;
      continue;
    }

    //! *i1 > *i2  then push *i2
    if( c != 0 ) // ie : c > 0
    {
      ret.push_back( - *i2 );
      hash ^= ret.back().hash();

      ++i2; --d2;
      continue;
    }

    //! collision case : *i1 >< *i2

    // cancel hash
    hash ^= i1->hash();

    ret.push_back( *i1 - *i2 );
    const epair &e = ret.back();

    if( e.null() )
      ret.pop_back();
    else
      hash ^= e.hash();

    ++i1; --d1;
    ++i2; --d2;
  }

  std::copy(i1, e1, std::back_inserter(ret));
  std::transform(i2, e2, std::back_inserter(ret), neg_hash<epair>(hash));

  return ret.empty() ? 0 : retp.release();
}

}}} // namespace analysis::epseq::detail

#endif
