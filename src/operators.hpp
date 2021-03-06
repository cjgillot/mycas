#ifndef OPERATORS_HPP_
#define OPERATORS_HPP_

#include<boost/operators.hpp>

/*!
 * \namespace operators
 *
 * This namespace provides several operator-like functions
 * (ie. compare and print),
 * and some operator facilities
 * (ie. ordered and printable).
 */
namespace operators {

//! \brief Base case comparison
template<class S>
inline int
compare(const S &a, const S &b)
{ return S::compare(a,b); }

//! \brief Pointer to const comparison : compare pointees
template<class S>
inline int
compare(const S *a, const S *b)
{ return operators::compare(*a, *b); }

//! \brief Pointer comparison : compare pointees
template<class S>
inline int
compare(S *a, S *b)
{ return operators::compare(*a, *b); }

/*!\brief Ordering operator facility
 *
 * Provided a static member \c compare() function,
 * this class creates all the ordering operators.
 */
template<class T, class B = ::boost::detail::empty_base<T> >
struct ordered
: boost::totally_ordered1<T, B> {
  //! \brief Less-than operator
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

/*!\brief Stream operator facility
 *
 * Provided a member \c print() function,
 * this class creates the ostream \c operator<<.
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

/*!\brief Safe bool operator facility
 *
 * Provided a member \c valid() function,
 * this class creates the safe bool operator
 * and the unary not \c operator!.
 */
template<class T, class B = ::boost::detail::empty_base<T> >
struct testable
: public B {
private:
  //! \brief Detail structure
  struct safe_bool {
    B* fnc(T* &);

    typedef B* (safe_bool::*bool_type)(T* &);

    static bool_type to_bool(bool pred)
    { return pred ? &safe_bool::fnc : 0; }
  };

  const T& self() const
  { return static_cast< const T& >( *this ); }

public:
  //! Safe bool type
  typedef typename safe_bool::bool_type bool_t;

  //! \brief Safe bool operator
  inline
  operator bool_t() const
  { return safe_bool::to_bool( self().valid() ); }
  //! \brief Negation operator
  inline bool
  operator!() const
  { return !self().valid(); }
};

} // namespace operators

#endif /* OPERATORS_HPP_ */
