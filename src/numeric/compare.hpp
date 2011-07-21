#ifndef COMPARE_HPP
#define COMPARE_HPP

#include "number.hpp"
#include "repr.hpp"

namespace numeric {

// value_test_v
namespace {

template<unsigned N>
struct value_test_v
: boost::static_visitor<bool> {

  template<class T>
  bool operator()(const T &e) const
  { return e == N; }

};

}

inline bool
repr_t::null() const
{ return boost::apply_visitor( value_test_v<0>(), m_impl ); }

inline bool
repr_t::unit() const
{ return boost::apply_visitor( value_test_v<1>(), m_impl ); }

// compare_v
namespace {

struct compare_v
: boost::static_visitor<int> {

  template<class T, class U>
  int operator()(const T &l, const U &r) const
  { return cmp(l, r); }

};

}

inline int
repr_t::compare(const repr_t &a, const repr_t &b)
{ return boost::apply_visitor( compare_v(), a.m_impl, b.m_impl ); }

// hash_v
namespace {

struct hash_v
: boost::static_visitor<std::size_t> {

  inline std::size_t
  operator()(const mpz_class &o) const
  {
    std::size_t seed = 0;

    mpz_srcptr o2 = o.get_mpz_t();
    std::size_t i = mpz_size( o2 );

    while( i != 0 )
      boost::hash_combine( seed, mpz_getlimbn( o2, --i ) );

    return seed;
  }

  inline std::size_t
  operator()(const mpq_class &o) const
  {
    std::size_t seed = operator()( o.get_num() );
    boost::hash_combine( seed, operator()( o.get_den() ) );
    return seed;
  }

  inline std::size_t
  operator()(const mpfr_class &o) const
  {
    mpfr_srcptr o2 = o.get_mpfr_t();

    if( mpfr_zero_p( o2 ) ) return 0;
    if( mpfr_nan_p ( o2 ) ) return 1;
    if( mpfr_inf_p ( o2 ) ) return 2;

    std::size_t seed = mpfr_sgn( o2 );

    boost::hash_combine( seed, mpfr_get_prec( o2 ) );
    boost::hash_combine( seed, mpfr_get_exp ( o2 ) );

    // hash the limbs now

    const std::size_t sz = std::ceil( double( mpfr_get_prec( o2 ) ) / double(mp_bits_per_limb) );
    boost::hash_range( seed, o2->_mpfr_d , o2->_mpfr_d + sz );

    return seed;
  }

};

}

inline std::size_t
repr_t::hash() const
{ return boost::apply_visitor( hash_v(), m_impl ); }


// print_v
namespace {

template<class S>
struct print_v
: boost::static_visitor<> {

  S &os;

  print_v(S &s)
  : os(s) {}

  template<class T>
  inline void
  operator()(const T &o) const
  { os << o; }

};

}

template<class S>
inline void
repr_t::print(S &os) const
{ boost::apply_visitor( print_v<S>(os), m_impl ); }

}

#endif
