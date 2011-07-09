/*
 * base.hxx
 *
 *  Created on: 30 juin 2011
 *      Author: k1000
 */

#ifndef GROUP_BASE_HXX_
#define GROUP_BASE_HXX_

#include "stdlib.hxx"
#include "operators.hxx"

#include "structures/magma/data.hxx"
#include "algebra/compare.hxx"
#include "algebra/print.hxx"
#include "algebra/utils.hxx"

namespace structures {
namespace group {

template<class K, class T>
class base
: public operators::ordered<base<K,T>
, boost::multipliable2<base<K,T>, int
> > {

  typedef std::pair<K, T> pair_t;
  typedef std::list<pair_t> impl_t;
  impl_t impl;

public:
  inline base() {}

  inline
  base(const base &o)
  : impl(o.impl) {}
  inline base&
  operator=(const base &o) {
    impl = o.impl;
    return *this;
  }
  inline void
  swap(base &o) {
    impl.swap(o.impl);
  }

public:
  inline explicit
  base(const T &l)
  : impl(1, std::make_pair(algebra::one<K>(), l)) {}

  inline
  ~base() {}

public:
  inline bool
  null() const {
    return impl.empty();
  }

  static const base zero;

public:
  typedef typename impl_t::iterator iterator;
  typedef typename impl_t::const_iterator const_iterator;

  iterator begin() { return impl.begin(); }
  iterator end  () { return impl.end(); }

  const_iterator begin() const { return impl.begin(); }
  const_iterator end  () const { return impl.end  (); }

public:
  inline bool
  empty() const
  { return impl.empty(); }

private:
  struct negator
  : std::unary_function<pair_t&, void> {
    inline void
    operator()(pair_t &p)
    { algebra::ineg(p.first); }
  };

  // C = number of collisions
  static inline void
  do_add(impl_t &a, impl_t &b) {
    for(; !a.empty() && !b.empty() ;) {
      pair_t
        &ab = a.back()
      , &bf = b.front();
      if(ab.second != bf.second) break;
      ab.first += bf.first;
      b.pop_front();
      if(algebra::null(ab.first))
        a.pop_back();
      else
        break;
    }
    a.splice(a.end(), b);
  }
  // C = |a|
  static inline void
  do_neg(impl_t &a) {
    boost::for_each(a, negator());
  }
  // C = |b|
  static inline void
  do_sub(impl_t &a, impl_t &b) {
    for(; !a.empty() && !b.empty() ;) {
      pair_t
        &ab = a.back()
      , &bf = b.front();
      if(ab.second != bf.second) break;
      ab.first -= bf.first;
      b.pop_front();
      if(algebra::null(ab.first))
        a.pop_back();
      else
        break;
    }
    do_neg(b);
    a.splice(a.end(), b);
  }

public:
  // C = |a| + |b|
  friend inline base
  operator+(base a, const base &b)
  { return a += b; }
  // C = |b|
  inline base&
  operator+=(base o) {
    do_add(impl, o.impl);
    return *this;
  }

  // C = |a|
  friend inline base
  operator-(base a) {
    return a.ineg();
  }
  inline base&
  ineg() {
    do_neg(impl);
    return *this;
  }

  // C = |a| + |b|
  friend inline base
  operator-(base a, const base &b)
  { return a -= b; }
  // C = |b|
  inline base&
  operator-=(base o) {
    do_sub(impl, o.impl);
    return *this;
  }

  // C = n * |this|
  inline base&
  operator*=(int n) {
    if(n < 0) {
      ineg();
      n = -n;
    }
    base cst;
    while(n >= 2) {
      if(n & 1) cst += *this;
      *this += *this;
      n >>= 1;
    }
    if(n == 0) return *this  = cst;
    if(n == 1) return *this += cst;
  }

public:
  static int
  compare(const base &a, const base &b)
  { return algebra::range_compare(a.impl, b.impl, std::less<pair_t>()); }

private:
  template<class F>
  struct map2_type {
    typedef typename boost::result_of<F(const T&)>::type type;
  };
  template<class F>
  struct map_type {
    typedef typename boost::result_of<F(const T&)>::type reference;
    typedef typename boost::remove_reference<reference>::type value_type;
    typedef base<K,value_type> type;
  };

public:
  template<class F>
  inline typename map2_type<F>::type
  map2(F f) const {
    typedef typename map2_type<F>::type base2;
    base2 ret;
    foreach(const T &m, impl) {
      base2 im = f(m);
      ret.impl.splice(ret.impl.end(), im);
    }
    return ret;
  }
  template<class F>
  inline typename map_type<F>::type
  map(F f) const {
    typedef typename map_type<F>::type base2;
    base2 ret;
    boost::transform(impl, std::back_inserter(ret.impl), f);
    return ret;
  }

public:
  inline base
  reverse() const {
    base ret(*this);
    ret.impl.reverse();
    return ret;
  }

  template<class F>
  inline typename map2_type<F>::type
  rev_map2(F f) const {
    typedef typename map2_type<F>::type base2;
    base2 ret;
    reverse_foreach(const T &m, impl) {
      base2 im = f(m);
      ret.impl.splice(ret.impl.end(), im);
    }
    return ret;
  }
  template<class F>
  inline typename map_type<F>::type
  rev_map(F f) const {
    typedef typename map_type<F>::type base2;
    base2 ret;
    boost::transform(impl, std::front_inserter(ret.impl), f);
    return ret;
  }

public:
  template<class S>
  void print(S &os) const {
    foreach(const pair_t &m, impl) {
      os << " , " << m.first << "'" << m.second;
    }
  }
};

template<class K, class T>
const base<K,T> base<K,T>::zero;


}} // namespace structures::group


#endif /* GROUP_BASE_HXX_ */
