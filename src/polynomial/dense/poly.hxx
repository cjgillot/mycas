/*
 * poly.hxx
 *
 *  Created on: 9 juin 2011
 *      Author: k1000
 */

#ifndef POLY_HXX_
#define POLY_HXX_

#include "operators.hxx"

#include "algebra/integer.hxx"
#include "algebra/compare.hxx"
#include "utils.hxx"

#include "polynomial/dense/multiply.hxx"

namespace poly {
namespace dense {

/*!
 * \class poly
 * \brief Main class for dense polynomial representation
 */
template<class K>
class poly
: boost::additive1<poly<K>
, boost::multiplicative2<poly<K>, K
, operators::ordered<poly<K>
, operators::printable<poly<K>
> > > > {
  typedef std::vector<K> impl_t;

  // least exponent first -> ease shrinking
  impl_t impl;

public:
  /*!
   * \brief Integer ring type
   */
  typedef algebra::integer Z;


private:
  /*!
   * \brief Invariant guard function
   *
   * Call after any modification
   * modifying the degree.
   * This function ensures that the representation
   * contains has a non-null highest coefficient.
   */
  inline void
  reduce() {
    while(! impl.empty())
      if(algebra::null(impl.back()))
        impl.pop_back();
      else
        return;
  }

public:
  /*!
   * \brief Default constructor
   * Creates a null polynomial
   */
  inline
  poly() {}

  /*!
   * \brief Copy constructor
   */
  inline
  poly(const poly &o)
  : impl(o.impl) {}
  /*!
   * \brief Assignement operator
   */
  inline poly &
  operator=(const poly &o) {
    impl = o.impl;
    return *this;
  }
  /*!
   * \brief Non-throwing swap
   * @param o
   */
  inline void
  swap(poly &o) {
    std::swap(impl, o.impl);
  }

public:
  /*!
   * \brief Constant polynomial constructor
   * \param c : the constant
   */
  inline explicit
  poly(const K &c)
  : impl(1,c)
  { if(algebra::null(c)) impl.clear(); }

  /*!
   * \brief Destructor
   */
  inline
  ~poly() {}

public:
  //! \brief Iterator traversing the underlying container
  typedef typename impl_t::iterator iterator;
  //! \brief Const iterator traversing the underlying container
  typedef typename impl_t::const_iterator const_iterator;

private:
  // mutable
  inline iterator
  begin()
  { return impl.begin(); }
  inline iterator
  end()
  { return impl.end(); }

public:
  /*!
   * \brief Const begin iterator
   *
   * constant time
   */
  inline const_iterator
  begin() const
  { return impl.begin(); }
  /*!
   * \brief Const end iterator
   *
   * constant time
   */
  inline const_iterator
  end() const
  { return impl.end(); }

public:
  /*!
   * \brief static zero polynomial
   */
  static const poly zero;
  /*!
   * \brief static unity polynomial
   */
  static const poly one;

  /*!
   * \brief nullity test
   *
   * constant time
   */
  inline bool
  null() const
  { return impl.empty(); }
  /*!
   * \brief unity test
   *
   * constant time
   */
  inline bool
  unit() const {
    return impl.size() == 1
        && algebra::unit(impl.front());
  }

  /*!
   * \brief Polynomial degree
   *
   * constant time
   */
  inline Z
  deg() const
  { return impl.size() - 1; }

  /*!
   * \brief Monomial test
   *
   * \return true if the polynomial has the form [a * X^p]
   *    where [a != 0].
   *
   * linear time
   */
  inline bool
  monome() const {
    reduce();
    if(impl.empty()) return false;
    const_iterator it = impl.begin(), end = impl.end();
    ++it;
    for(; it != end; ++it)
      if(! algebra::null(*it))
        return false;
    return true;
  }

private:
  /*!
   * \brief polynomial combination
   *
   * Implementation of += and -= operators
   *   requirement : m != 0 implies f1(m) != 0
   *
   * linear time
   */
  template<class Fnc1, class Fnc2>
  static inline void
  combine(poly &a, const poly &b, Fnc1 f1, Fnc2 f2) {
    a.impl.reserve(std::max(a.impl.size(), b.impl.size()));

    iterator
      i1 = a.begin(),
      e1 = a.end();
    const_iterator
      i2 = b.begin(),
      e2 = b.end();

    for(; i1 != e1 && i2 != e2; ++i1, ++i2)
      f2(*i1, *i2);

    /* if terms remain in o */
    for(; i2 != e2; ++i2)
      a.impl.push_back(f1(*i2));

    a.reduce();
  }

public:
  /*!
   * \brief in-place addition operator
   *
   * linear time
   */
  inline poly &
  operator+=(const poly &o) {
    combine(*this, o,
        functor::identity<K>(),
        functor::plus_eq<K>()
    );
    return *this;
  }
  /*!
   * \brief in-place subtraction operator
   *
   * linear time
   */
  inline poly &
  operator-=(const poly &o) {
    combine(*this, o,
        functor::negate<K>(),
        functor::minus_eq<K>()
    );
    return *this;
  }

  /*!
   * \brief in-place negation
   *
   * linear time
   */
  inline poly &
  ineg() {
    boost::for_each(*this, algebra::ineg<K>);
    return *this;
  }

  /*!
   * \brief in-place scalar multiplication
   *
   * linear time
   */
  inline poly &
  operator*=(const K &o) {
    if(algebra::null(o)) {
      impl.clear();
      return *this;
    }
    std::for_each(begin(), end(), functor::multiplies_by<K>(o));
    return *this;
  }
  /*!
   * \brief in-place scalar division
   *
   * linear time
   */
  inline poly &
  operator/=(const K &o) {
    assert(! algebra::null(o));
    std::for_each(begin(), end(), functor::divides_by<K>(o));
    return *this;
  }

private:
  static inline void
  do_mul(const impl_t &a, const impl_t &b, impl_t &r) {
    if(a.empty() || b.empty()) {
      r.clear();
      return;
    }
    r.resize(a.size() + b.size() - 1, algebra::zero<K>());
    multiply::mul(a,b,r.begin());
  }

public:
  /*!
   * \brief multiplication operator
   *
   * time : see multiply.hxx
   *
   * \param a,b : polynomials
   * \return their product [a*b]
   */
  friend inline poly
  operator*(const poly &a, const poly &b) {
    poly ret;
    poly::do_mul(a.impl, b.impl, ret.impl);
    ret.reduce();
    return ret;
  }
  /*!
   * \brief in-place multiplication operator
   *
   * time : see multiply.hxx
   */
  inline poly &
  operator*=(const poly &o) {
    (*this * o).swap(*this);
    return *this;
  }

public:
  /*!
   * \brief Named constructor from coefficients range
   *
   * linear time
   *
   * \param r : a coefficient range
   * \return : the polynomial whose coefficients are r's
   *    exponents in increasing order
   */
  template<class Range>
  static inline poly
  from_coefs(const Range &r) {
    poly ret;
    int sz = boost::size(r);
    ret.impl.reserve(sz);
    boost::copy(r, std::back_inserter(ret.impl));
    return ret;
  }

public:
  /*!
   * \brief Printing function
   *
   * linear time
   *
   * \param ios : an output stream
   */
  template<class S>
  inline void
  print(S &ios) const {
    algebra::print_range(impl, ios);
  }

public:
  /*!
   * \brief Comparison function
   *
   * worst case linear time
   *
   * \return the lexicographical comparison value
   */
  inline static int
  compare(const poly &a, const poly &b) {
    return algebra::range_compare(
        a.impl, b.impl,
        algebra::compare<K>
    );
  }
};

template<class K>
const poly<K> poly<K>::zero;

template<class K>
const poly<K> poly<K>::one(algebra::one<K>());

}} // namespace poly::dense

#endif /* POLY_HXX_ */
