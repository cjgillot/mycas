/*
 * expair.hpp
 *
 *  Created on: 20 juin 2011
 *      Author: k1000
 */

#ifndef EXPAIR_HXX_
#define EXPAIR_HXX_

#include "stdlib.hpp"

namespace analysis {

/*!
 * \brief Traits structure for expair
 *
 * This structure needs to be specialized
 * for each class instanciating expair.
 *
 * \li Member types : \c coef_type and \c rest_type,
 * the types of the \c expair data.
 * \li Member structure : \c ep,
 * providing the operations.
 *
 * \c ep member functions : (if they make sense)
 * \li null_pair(const coef_type &, const rest_type &)
 * \li compare_coef(const coer_type &, const coef_type &)
 * \li compare_rest(const rest_type &, const rest_type &)
 * \li add/sub/mul/div _coef(coef_type &, const coef_type &)
 * \li neg/inv _coef(coef_type &)
 * \li mul/div _rest(rest_type &, const rest_type &)
 * \li neg/inv _rest(rest_type &)
 * \li print_pair(const coef_type &, const rest_type &)
 *
 * \see expair
 */
template<class Impl>
struct expair_traits;

/*!
 * \brief The \c expair representation class
 *
 * This CRTP class provides a mean to consider
 * a \c basic-inherited class as an \c expair for use
 * with \c expairseq.
 *
 * All operations names are given considering
 * the expair as a multiplication { \c coef * \c rest}.
 *
 * This class has value semantic and is not meant
 * to be derived.
 *
 * Each using class has to provide needed types and functions
 * via the \a Traits template parameter.
 *
 * The \a Impl class shall define coercion from and to \c expair.
 *
 * \see expair_traits
 */
template<class Traits>
struct expair
: boost::field_operators1<expair<Traits>
, boost::multiplicative2<expair<Traits>, typename Traits::coef_type
> > {

private:
  //! \brief Default constructor (disabled)
  expair();

  typedef typename Traits::ep ep;

public:
  //! \brief Coefficient type
  typedef typename Traits::coef_type K;

  //! \brief Rest type
  typedef typename Traits::rest_type Z;

  //! \brief Coefficient data
  K m_coef;

  //! \brief Rest data
  Z m_rest;

public:
  //! \brief Pair constructor
  expair(const K &c, const Z &r);

  //! \brief Copy constructor
  expair(const expair &o);

  //! \brief Assignement operator
  expair&
  operator=(const expair &o);

  //! \brief Non-throwing swap
  void
  swap(expair &o);

  //! \brief Destructor
  ~expair();

public:
  //! \brief Null testing
  bool null() const;

public:
  //! \brief Addition
  expair &operator+=(const expair &o);
  //! \brief Subtraction
  expair &operator-=(const expair &o);

  //! \brief Inplace negation
  expair &ineg();
  //! \brief Negation
  expair  operator-() const;

  //! \brief Scalar multiplication
  expair &operator*=(const number &o);
  //! \brief Scalar division
  expair &operator/=(const number &o);

  //! \brief Multiplication
  expair &operator*=(const expair &o);
  //! \brief Division
  expair &operator/=(const expair &o);
  //! \brief Inplace inversion
  expair &iinv();

public:
  //! \brief Partial hash
  std::size_t
  calc_hash() const;

  //! \brief Partial rest-ordering
  static int
  compare(const expair &a, const expair &b);

  //! \brief Real deep hash
  std::size_t
  deep_hash() const;

  //! \brief Real deep ordering
  static int
  deep_compare(const expair &a, const expair &b);

  //! \brief Printing
  void print(std::basic_ostream<char> &os) const;
};

}

#include "expair.ipp"

#endif /* EXPAIR_HXX_ */
