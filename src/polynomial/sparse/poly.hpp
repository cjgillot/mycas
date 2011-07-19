/*
 * poly.hpp
 *
 *  Created on: 22 mai 2011
 *      Author: k1000
 */

// TODO:
// - polynomial division

#ifndef SPARSE_POLY_HPP_
#define SPARSE_POLY_HPP_

#include "stdlib.hpp"
#include "utils.hpp"

#include "algebra/compare.hpp"
#include "algebra/print.hpp"

#include "polynomial/sparse/monomial.hpp"
#include "polynomial/sparse/multiply.hpp"

namespace poly {
namespace sparse {

/*!
 * \class poly
 * \brief Main class for sparse polynomials representation
 *
 * These are created on top of M, a monomial group (see monomial.hpp)
 * M must be :
 * - CopyConstructible
 * - Destructible
 * - plus_eq and minus_eq'able
 * - ineg'able and negate'able
 * - multiply'able and divide'able
 * - compare'able and deep_compare'able
 * - null testable and zero'able
 */
template<class M>
class poly
: boost::additive1<poly<M>
, boost::multiplicative2<poly<M>, M
, operators::ordered<poly<M>
, operators::printable<poly<M>
> > > > {

  // monomials are sorted
  // biggest exponent first
  typedef std::list<M> impl_t;
  impl_t impl;

public:
  //! \brief Integer ring type
  typedef typename M::Z Z;

public:
  /*!
   * \brief Default constructor
   *
   * Creates a null polynomial
   */
  poly() {}

  //! \brief Copy constructor
  poly(const poly &m)
  : impl(m.impl) {}
  //! \brief Assignement operator
  poly &operator=(const poly &o) {
    impl=o.impl;
    return *this;
  }
  //! \brief Non-throwing swap
  void swap(poly &o) {
    std::swap(impl, o.impl);
  }

  //! \brief Monomial polynomial constructor
  explicit
  poly(const M &m)
  : impl(1,m) {}

  //! \brief Destructor
  ~poly() {}

public:
  /*!
   * \defgroup Range operations
   * \{
   */

  //! \brief Iterator traversing the underlying container
  typedef typename impl_t::iterator iterator;
  //! \brief Const iterator traversing the underlying container
  typedef typename impl_t::const_iterator const_iterator;
  //! \brief Reverse iterator traversing the underlying container
  typedef typename impl_t::reverse_iterator reverse_iterator;
  //! \brief Const reverse iterator traversing the underlying container
  typedef typename impl_t::const_reverse_iterator const_reverse_iterator;

private:
  iterator
  begin()
  { return impl.begin(); }
  iterator
  end()
  { return impl.end(); }
  reverse_iterator
  rbegin()
  { return impl.rbegin(); }
  reverse_iterator
  rend()
  { return impl.rend(); }


public:
  //! \brief Const begi
  const_iterator
  begin() const
  { return impl.begin(); }
  //! \brief Const end
  const_iterator
  end() const
  { return impl.end(); }
  //! \brief Const reverse begin
  const_reverse_iterator
  rbegin() const
  { return impl.rbegin(); }
  //! \brief Const reverse end
  const_reverse_iterator
  rend() const
  { return impl.rend(); }

  //! \brief Stored size
  std::size_t
  size() const
  { return impl.size(); }

  //! \}

public:
  //! \brief static zero polynomial
  static const poly zero;
  //! \brief static unity polynomial
  static const poly one;

  /*!
   * \brief nullity test
   *
   * constant time
   */
  bool
  null() const
  { return impl.empty(); }
  /*!
   * \brief unity test
   *
   * constant time
   */
  bool
  unit() const {
    return impl.size() == 1
        && algebra::unit(impl.front());
  }

  /*!
   * \brief Polynomial degree
   *
   * constant time
   */
  const Z &
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
  bool
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
  static void
  combine(
    impl_t &a
  , const impl_t &b
  , Fnc1 f1
  , Fnc2 f2
  );

public:
  /*!
   * \defgroup Linear operations
   * \{
   */

  //! \brief Transformation function
  template<class F>
  void
  transform(F f)
  { boost::for_each(impl, f); }
  //! \brief Transformation function, const version
  template<class F>
  void
  transform(F f) const
  { boost::for_each(impl, f); }


  template<class M2, class F>
  friend poly poly<M2>::map(F f);

  //! \brief Map function
  template<class F>
  poly<
    typename boost::result_of<F(const M &)>::type
  >
  map(F f) const {
    typedef typename boost::result_of<F(const M &)>::type M2;
    poly<M2> ret;
    boost::transform(impl, std::back_inserter(ret.impl), f);
    return ret;
  }

  /*!
   * \brief Printing function
   * \param os : an output stream
   */
  template<class S>
  void
  print(S &os) const {
    algebra::print_range(impl, os);
  }


public:
  //! \brief Inplace addition
  poly &
  operator+=(const poly &o) {
    combine(
        impl, o.impl,
        functor::identity<M>(),
        functor::plus_eq<M>()
    );
    return *this;
  }
  //! \brief Inplace subtraction
  poly &
  operator-=(const poly &o) {
    combine(
        impl, o.impl,
        functor::negate<M>(),
        functor::minus_eq<M>()
    );
    return *this;
  }

  //! \brief Inplace negation
  poly &
  ineg() {
    transform(algebra::ineg<M>);
    return *this;
  }

  //! \brief Inplace scalar multiplication
  poly &
  operator*=(const M &o) {
    if(algebra::null(o)) {
      impl.clear();
      return *this;
    }
    transform(functor::multiplies_by<M>(o));
    return *this;
  }
  //! \brief Inplace scalar division
  poly &
  operator/=(const M &o) {
    assert(! algebra::null(o));
    transform(functor::divides_by<M>(o));
    return *this;
  }

  //! \} (linear operations)

public:
  /*!
   * \brief multiplication operator
   *
   * time : see multiply.hpp
   *
   * \param a,b : polynomials
   * \return their product [a*b]
   */
  friend poly
  operator*(const poly &a, const poly &b) {
    if(a.impl.empty() || b.impl.empty()) {
      return zero;
    }

    poly ret;
    multiply::do_mul<M, impl_t, impl_t>(a.impl, b.impl, ret.impl);
    return ret;
  }
  /*!
   * \brief in-place multiplication operator
   *
   * time : see multiply.hpp
   */
  poly &
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
  static poly
  from_monos(const Range &r) {
    poly ret;
    boost::copy(r, std::back_inserter(ret.impl));
    return ret;
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
        M::deep_compare
    );
  }
};


template<class M>
template<class Fnc1, class Fnc2>
void poly<M>::combine(
  impl_t &a
, const impl_t &b
, Fnc1 f1
, Fnc2 f2
) {
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


template<class M>
const poly<M> poly<M>::zero;

template<class M>
const poly<M> poly<M>::one(algebra::one<M>());

}} // poly::sparse

#endif /* SPARSE_POLY_HPP_ */
