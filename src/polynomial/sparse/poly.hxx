/*
 * poly.hxx
 *
 *  Created on: 22 mai 2011
 *      Author: k1000
 */

// TODO:
// - polynomial division

#ifndef POLY_HXX_
#define POLY_HXX_

#include "stdlib.hxx"
#include "utils.hxx"

#include "algebra/compare.hxx"
#include "algebra/print.hxx"

#include "polynomial/sparse/monomial.hxx"
#include "polynomial/sparse/multiply.hxx"

namespace poly {
namespace sparse {

/*!
 * \class poly
 * \brief Main class for sparse polynomials representation
 *
 * These are created on top of M, a monomial group (see monomial.hxx)
 */
template<class M>
class poly
: boost::additive1<poly<M>
, boost::multiplicative2<poly<M>, M
, operators::ordered<poly<M>
, operators::printable<poly<M>
> > > >
{
  // monomials are sorted
  // biggest exponent first
  typedef std::list<M> impl_t;
  impl_t impl;

public:
  /*!
   * \brief Integer ring type
   */
  typedef typename M::Z Z;

public:
  /*!
   * \brief Default constructor
   *
   * Creates a null polynomial
   */
  inline
  poly() {}

  /*!
   * \brief Copy constructor
   */
  inline
  poly(const poly &m)
  : impl(m.impl) {}
  /*!
   * \brief Assignement operator
   */
  inline poly &
  operator=(const poly &o) {
    impl=o.impl;
    return *this;
  }
  /*!
   * \brief Non-throwing swap
   */
  inline void
  swap(poly &o) {
    std::swap(impl, o.impl);
  }

  /*!
   * \brief Monomial polynomial constructor
   */
  inline explicit
  poly(const M &m)
  : impl(1,m) {}

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
  //! \brief Reverse iterator traversing the underlying container
  typedef typename impl_t::reverse_iterator reverse_iterator;
  //! \brief Const reverse iterator traversing the underlying container
  typedef typename impl_t::const_reverse_iterator const_reverse_iterator;

private:
  inline iterator
  begin()
  { return impl.begin(); }
  inline iterator
  end()
  { return impl.end(); }
  inline reverse_iterator
  rbegin()
  { return impl.rbegin(); }
  inline reverse_iterator
  rend()
  { return impl.rend(); }


public:
  /*!
   * \brief Const begin
   *
   * constant time
   */
  inline const_iterator
  begin() const
  { return impl.begin(); }
  /*!
   * \brief Const end
   *
   * constant time
   */
  inline const_iterator
  end() const
  { return impl.end(); }
  /*!
   * \brief Const reverse begin
   *
   * constant time
   */
  inline const_reverse_iterator
  rbegin() const
  { return impl.rbegin(); }
  /*!
   * \brief Const reverse end
   *
   * constant time
   */
  inline const_reverse_iterator
  rend() const
  { return impl.rend(); }

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
  inline const Z &
  deg() const {
    return impl.front().deg();
  }

  /*!
   * \brief Monomial test
   *
   * \return true if the polynomial has the form [a * X^p]
   *    where [a != 0].
   *
   * constant time
   */
  inline bool
  monome() const
  { return impl.size() == 1; }

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
  combine(impl_t &a, const impl_t &b, const Fnc1 &f1, const Fnc2 &f2) {
    iterator
      i1 = a.begin(),
      e1 = a.end();
    const_iterator
      i2 = b.begin(),
      e2 = b.end();

    while(i1 != e1 && i2 != e2) {
      int cmp = algebra::compare(*i1, *i2);
      if(cmp < 0) { ++i1; continue; }
      if(cmp > 0) { a.insert(i1, f1(*i2)); ++i2; continue; }
      f2(*i1, *i2);

      ++i2;

      if(algebra::null(*i1))
        i1 = a.erase(i1);
      else
        ++i1;
    }

    /* if terms remain in o */
    while(i2 != e2) {
      a.push_back(f1(*i2));
      ++i2;
    }
  }

public:
  /*!
   * \brief in-place addition operator
   *
   * linear time
   */
  inline poly &
  operator+=(const poly &o) {
    combine(
        impl, o.impl,
        functor::identity<M>(),
        functor::plus_eq<M>()
    );
    return *this;
  }
  /*!
   * \brief in-place addition operator
   *
   * linear time
   */
  inline poly &
  operator-=(const poly &o) {
    combine(
        impl, o.impl,
        functor::negate<M>(),
        functor::minus_eq<M>()
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
    boost::for_each(impl, algebra::ineg<M>);
    return *this;
  }

  /*!
   * \brief in-place scalar multiplication
   *
   * linear time
   */
  inline poly &
  operator*=(const M &o) {
    if(algebra::null(o)) {
      impl.clear();
      return *this;
    }
    boost::for_each(
        impl,
        functor::multiplies_by<M>(o)
    );
    return *this;
  }
  /*!
   * \brief in-place scalar division
   *
   * linear time
   */
  inline poly &
  operator/=(const M &o) {
    assert(! algebra::null(o));
    boost::for_each(
        impl,
        functor::divides_by<M>(o)
    );
    return *this;
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
    if(a.impl.empty() || b.impl.empty()) {
      return zero;
    }

    poly ret;
    multiply::do_mul<M, impl_t, impl_t>(impl, o.impl, ret.impl);
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
  from_monos(const Range &r) {
    poly ret;
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
  static int
  compare(const poly &a, const poly &b) {
    return algebra::range_compare(
        a.impl, b.impl,
        std::mem_fun_ref(&M::deep_compare)
    );
  }
};

template<class M>
const poly<M> poly<M>::zero;

template<class M>
const poly<M> poly<M>::one(algebra::one<M>());

}} // poly::sparse

#endif /* POLY_HXX_ */
