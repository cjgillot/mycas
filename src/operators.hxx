/*
 * operators.hxx
 *
 *  Created on: 11 juin 2011
 *      Author: k1000
 */

#ifndef OPERATORS_HXX_
#define OPERATORS_HXX_

#include "stdlib.hxx"

/*!
 * \namespace operators
 *
 * This namespace provides several operator-like functions
 * (ie. compare and print),
 * and some operator facilities
 * (ie. oedered and printable).
 */
namespace operators {

/*!
 * \brief Base case comparison
 */
template<class S>
inline int
compare(const S &a, const S &b)
{ return S::compare(a,b); }

/*!
 * \brief Pointer to const comparison : compare pointees
 */
template<class S>
inline int
compare(const S *a, const S *b)
{ return operators::compare(*a, *b); }

/*!
 * \brief Pointer comparison : compare pointees
 */
template<class S>
inline int
compare(S *a, S *b)
{ return operators::compare(*a, *b); }

/*!
 * \struct ordered
 * \brief Ordering operator facility
 *
 * Provided a static member {compare()} function,
 * this class creates all the ordering operators.
 */
template<class T, class B = ::boost::detail::empty_base<T> >
struct ordered
: boost::totally_ordered1<T, B> {
  //! \brief less_than operator
  friend inline bool
  operator< (const T &a, const T &b) {
    return T::compare(a,b) < 0;
  }
  //! \brief Equality operator
  friend inline bool
  operator==(const T &a, const T &b) {
    return T::compare(a,b) == 0;
  }
};

/*!
 * \struct printable
 * \brief Stream operator facility
 *
 * Provided a member {print()} function,
 * this class creates the ostream {<<} operator.
 */
template<class T, class B = ::boost::detail::empty_base<T> >
struct printable
: B {
  //! \brief Print operator
  template<class Char, class Traits>
  friend inline std::basic_ostream<Char, Traits> &
  operator<<(std::basic_ostream<Char, Traits> &ios, const T &a) {
    a.print(ios);
    return ios;
  }
};

/*!
 * \struct testable
 * \brief Safe bool operator facility
 *
 * Provided a member {valid()} function,
 * this class creates the safe bool operator
 * and the unary not (!) operator.
 */
template<class T, class B = ::boost::detail::empty_base<T> >
class testable
: public B {
  typedef boost::range_detail::safe_bool<bool(T::*)()const> safe_bool;

public:
  //! Safe bool type
  typedef typename safe_bool::unspecified_bool_type bool_t;

  //! \brief Safe bool operator
  inline
  operator bool_t() const {
    return safe_bool::to_unspecified_bool(
      static_cast<const T*>(this)->valid()
    , &T::valid
    );
  }
  //! \brief Negation operator
  inline bool
  operator!() const {
    return !static_cast<const T*>(this)->valid();
  }
};

} // namespace operators

//! \brief operator<< specialization for pointers

template<class T, class Char, class Traits>
inline std::basic_ostream<Char, Traits> &
operator<<(std::basic_ostream<Char, Traits> &ios, T* a) {
  return ios << *a;
}

#endif /* OPERATORS_HXX_ */
