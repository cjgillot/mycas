/*
 * multiply.hpp
 *
 *  Created on: 21 mai 2011
 *      Author: k1000
 *
 * This implementation follows [VDH97], see issac97.pdf
 */

#ifndef SERIES_DENSE_MULTIPLY_HXX
#define SERIES_DENSE_MULTIPLY_HXX

#include "stdlib.hpp"
#include "utils.hpp"

#include "stream.hpp"

#include "polynomial/dense/multiply.hpp"
#include "series/dense/series.hpp"

namespace series {
namespace dense {
namespace multiply {

namespace detail {

// TODO care for endianness
inline bool
is_power(unsigned n) {
  const unsigned first_bit = n & -n;
  return n == first_bit;
}

// given : int n = k * p, p a power of 2
// transform to next (k,p) pair verifying n = k * p
inline bool
get_kp(unsigned int &k, unsigned int &p) {
  if((k & 1) == 0) {
    k >>= 1; p <<= 1;
    return true;
  }
  return false;
}

template<class Range>
inline boost::sub_range<const Range>
sub_range(const Range &r, unsigned begin, unsigned end) {
  typedef boost::sub_range<const Range> range_t;
  typedef typename boost::range_iterator<const Range>::type iter_t;
  iter_t b = boost::begin(r), e = b, e0 = boost::end(r);
  b += begin; e += end;
  if(e - b < 0 || e0 - b < 0)
    return range_t();
  if(e0 - e < 0)
    return range_t(b,e0);
  return range_t(b,e);
}

template<class Range>
inline boost::sub_range<const Range>
sub_range(const Range &r, unsigned begin) {
  typedef boost::sub_range<const Range> range_t;
  typedef typename boost::range_iterator<const Range>::type iter_t;
  iter_t b = boost::begin(r), e = boost::end(r);
  b += begin;
  return b < e ? range_t(b,e) : range_t();
}

} // namespace detail

using poly::dense::multiply::mul;

template<class K, class Mem>
class multiplier
: public streams::stream_base<K, Mem> {

  typedef Mem list_t;

  typedef streams::stream_base<K, list_t> super;
  using super::m_val;
  using super::m_end;

  typedef series<K> serie_t;
  typedef typename serie_t::iterator iter_t;

  const serie_t a, b;
  iter_t ait, bit;

  // the current degree
  unsigned long n;

  // [ait] and [bit] point to the [n]th
  // coefficient of [a] and [b]

private:
  multiplier();

public:
  inline
  multiplier(const serie_t &a_, const serie_t &b_)
  : a(a_), b(b_)
  , ait(a.begin()), bit(b.begin())
  , n(0) {
    m_val.resize(1);
    m_val.front() = *ait * *bit;
    m_end=m_val.end();
  }

  inline
  ~multiplier() {}

private:
  friend class boost::iterator_core_access;

  inline typename list_t::iterator
  incr() {
    ++n;

    if(ait) ++ait;
    if(bit) ++bit;

    if(empty()) {
      // finalize calculations
      unsigned total = a.values().size() + b.values().size() - 1;

      if(total <= n)
        return m_end == m_val.end() ? m_end : m_end++;

      m_val.resize(total);

      //std::cerr << "finalization " << total << std::endl;

      for(; n < total; ++n) {
        //std::cerr << "final round : " << n << std::endl;
        //algebra::print_range(m_val, std::cerr); std::cerr << std::endl;

        diagonal();
        maincase();
      }

      return m_end == m_val.end() ? m_end : m_end++;
    }

    m_val.resize(2*n);

    if(ait) m_val[n] += *ait * b.values().front();
    if(bit) m_val[n] += a.values().front() * *bit;

    //std::cerr << "round : " << n << std::endl;
    //algebra::print_range(m_val, std::cerr); std::cerr << std::endl;

    diagonal();
    maincase();

    m_end=m_val.begin()+n;
    return m_end++;
  }

private:
  inline void
  diagonal() {
    if(n < 2 || !detail::is_power(n)) return;
    // now, exists p verifying n == 2^(p+1)

    // add a[n/2 : n) * b[n/2 : n) to ret[n : 2*n-1)
    mul(
      detail::sub_range(a.values(), (n>>1), n)
    , detail::sub_range(b.values(), (n>>1), n)
    , m_val.begin() + n
    );

    //std::cerr << "diagonal" << std::endl;
    //algebra::print_range(detail::sub_range(a.values(), (n>>1), n), std::cerr); std::cerr << std::endl;
    //algebra::print_range(detail::sub_range(b.values(), (n>>1), n), std::cerr); std::cerr << std::endl;
    //algebra::print_range(m_val, std::cerr); std::cerr << std::endl;
  }

  inline void
  maincase() {
    if(n < 3) return;

    // invariant : n = k * p, p a power of 2
    unsigned int k = n, p = 1;

    //std::cerr << "maincase" << std::endl;

    do {
      const unsigned long kp = n - p;

      if(p < a.values().size() && kp < b.values().size()) {

        //algebra::print_range(detail::sub_range(a.values(), ep, ep << 1), std::cerr); std::cerr << std::endl;
        //algebra::print_range(detail::sub_range(b.values(), kp, n), std::cerr); std::cerr << std::endl;

        // add a[p : 2*p) * b[n-p : n) to ret[n : 2*n-1)
        mul(
          detail::sub_range(a.values(), p, p << 1)
        , detail::sub_range(b.values(), kp, n)
        , m_val.begin() + n
        );

        //algebra::print_range(m_val, std::cerr); std::cerr << std::endl;

      }

      if(p < b.values().size() && kp < a.values().size()) {

        //algebra::print_range(detail::sub_range(a.values(), kp, n), std::cerr); std::cerr << std::endl;
        //algebra::print_range(detail::sub_range(b.values(), ep, ep << 1), std::cerr); std::cerr << std::endl;

        // add a[n-p : n) * b[p : 2*p) to ret[n : 2*n-1)
        mul(
          detail::sub_range(a.values(), kp, n)
        , detail::sub_range(b.values(), p, p << 1)
        , m_val.begin() + n
        );

        //algebra::print_range(m_val, std::cerr); std::cerr << std::endl;

      }

    } while(detail::get_kp(k, p) && k >= 3);
  }

private:
  inline bool
  empty() const {
    return !ait && !bit;
  }
};

template<class K, class Mem>
inline stream_ptr<K, Mem>
do_mul(const series<K> &a, const series<K> &b) {
  return stream_ptr<K, Mem>::from_stream_base(
    new multiplier<K, Mem>(a,b)
  );
}

}}} // namespace series::dense::multiply

#endif // SERIES_DENSE_MULTIPLY_HXX
