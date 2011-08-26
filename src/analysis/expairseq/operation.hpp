#ifndef EXPAIRSEQ_OPERATION_HPP
#define EXPAIRSEQ_OPERATION_HPP

#include "container/unsafe_vector.hpp"
#include "analysis/expairseq/poly.hpp"

#include <functional>
#include <boost/range.hpp>

#include "util/foreach.hpp"
#include "util/assert.hpp"
#include "util/move.hpp" // move_ptr

namespace analysis {
namespace epseq {

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
poly<epair>*
do_neg(
    const poly<epair> &a
  , std::size_t &hash
) {
  typedef poly<epair> vector_type;
  util::move_ptr<vector_type>
    ret ( new vector_type( a.size() ) );

  std::transform(
    a.begin(), a.end()
  , std::back_inserter( *ret )
  , neg_hash<epair>( hash )
  );

  ret->shrink();

  // negating never cancels
  return ret.release();
}

template<class epair, class Sca>
poly<epair>*
do_sca(
    const poly<epair> &a
  , const Sca &n
  , std::size_t &hash
) {
  typedef poly<epair> vector_type;
  util::move_ptr<vector_type>
    ret ( new vector_type( a.size() ) );

  std::transform(
    a.begin(), a.end()
  , std::back_inserter( *ret )
  , sca_hash<epair,Sca>( hash, n )
  );

  ret->shrink();

  // non-null scalar never cancels
  return ret.release();
}

template<class epair>
poly<epair>*
do_add(
    const poly<epair> &a
  , const poly<epair> &b
  , std::size_t &hash
) {
  //! \invariant \c hash already contains <tt>hash_value(a) ^ hash_value(b)</tt>

  typedef poly<epair> vector_type;
  util::move_ptr<vector_type>
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

  ret.shrink();

  return ret.empty() ? nullptr : retp.release();
}

template<class epair>
poly<epair>*
do_sub(
    const poly<epair> &a
  , const poly<epair> &b
  , std::size_t &hash
) {
  //! \invariant \c hash already contains <tt>hash_value(a)</tt>

  typedef poly<epair> vector_type;
  util::move_ptr<vector_type>
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

  ret.shrink();

  return ret.empty() ? nullptr : retp.release();
}

template<class epair>
struct sort_pred
: std::binary_function<bool, const epair&, const epair&>
{
  inline bool operator()( const epair &a, const epair &b ) const
  { return epair::compare( a, b ) < 0; }
};

struct do_range_impl_inplace {
  template<class epair, class Iter, class Cont>
  static void
  unique(
    Iter beg
  , const Iter &end
  , Cont &ret
  , std::size_t &hash
  ) {
    STATIC_ASSERT(( boost::is_same< epair, typename boost::iterator_value<Iter>::type >::value ));

    // unique_copy and hash

    // use the range itself as buffer
    epair &buf = *beg;

    //! \invariant
    //!   buf aliases *begin()
    //!   only the subrange [begin(), it) has been modified
    //!   so the range [it, end) has not
    while( ++beg != end )
    {
      util::cmp_t c = epair::compare( buf, *beg );

      // collision : accumulate in buf
      if( c == 0 )
      {
        buf = buf + *beg;
        continue;
      }

      // no collision, commit and update
      if( ! buf.null() )
      {
        hash ^= buf.hash();
        ret.push_back( buf );
      }

      // load new value in buf
      buf.swap( *beg );
    }
  }
};

struct do_range_impl_copy {
  template<class epair, class Iter, class Cont>
  static void
  unique(
    Iter beg
  , const Iter &end
  , Cont &ret
  , std::size_t &hash
  ) {
    // unique_copy and hash

    epair buf = *beg;

    //! \invariant
    //!   buf aliases *begin()
    //!   only the subrange [begin(), it) has been modified
    //!   so the range [it, end) has not
    while( ++beg != end )
    {
      util::cmp_t c = epair::compare( buf, *beg );

      // collision : accumulate in buf
      if( c == 0 )
      {
        buf = buf + *beg;
        continue;
      }

      // no collision, commit and update
      if( ! buf.null() )
      {
        hash ^= buf.hash();
        ret.push_back( buf );
      }

      // load new value in buf
      buf = *beg;
    }
  }
};

//! \brief Construct a vector from an unsorted range of \c epair
template<class epair, class Iter>
poly<epair>*
do_range_mutable(
    const Iter &beg
  , const Iter &end
  , std::size_t &hash
) {
  CONCEPT_ASSERT(( boost::Mutable_RandomAccessIteratorConcept<Iter> ));

  typedef poly<epair> vec_t;

  { // get canonical ordering
    sort_pred< epair > p;
    std::sort( beg, end, p );
  }

  typename std::iterator_traits< Iter >::difference_type
    dist = std::distance( beg, end );

  ASSERT( dist >= 0 );

  util::move_ptr<vec_t> retp( new vec_t( std::size_t( dist ) ) );
  vec_t &ret = *retp;

  { // unique_copy and hash
    typedef typename boost::iterator_value<Iter>::type value_type;

    typedef typename boost::mpl::if_<
      boost::is_same<epair, value_type>
    , do_range_impl_inplace
    , do_range_impl_copy
    >::type impl;

    impl::template unique<epair, Iter>( beg, end, ret, hash );
  }

  ret.shrink();

  return ret.empty() ? nullptr : retp.release();
}

//! \brief Construct a vector from an unsorted range of \c epair
template<class epair, class Iter>
poly<epair>*
do_range_const(
    const Iter &beg
  , const Iter &end
  , std::size_t &hash
) {
  CONCEPT_ASSERT(( boost::RandomAccessIteratorConcept<Iter> ));

  typename std::iterator_traits< Iter >::difference_type
    dist = std::distance( beg, end );

  ASSERT( dist >= 0 );

  typedef container::unsafe_vector<epair> vec_t;
  vec_t tmp ( std::size_t( dist ), beg, end );

  return do_range_mutable<epair>( tmp.begin(), tmp.end(), hash );
}

}} // namespace analysis::epseq

#endif
