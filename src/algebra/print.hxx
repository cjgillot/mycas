/*
 * print.hxx
 *
 *  Created on: 12 juin 2011
 *      Author: k1000
 */

#ifndef PRINT_HXX_
#define PRINT_HXX_

#include "stdlib.hxx"

namespace algebra {

namespace detail {

/*!
 * \struct print_one
 * \brief helper functor for print_range
 */
template<class S, class T>
class print_one {
  S* ios;

public:
  /*!
   * \brief Constructor from ostream
   */
  print_one(S &s)
  : ios(&s) {}
  /*!
   * \brief Copy constructor
   */
  inline
  print_one(const print_one &o)
  : ios(o.ios) {}
  /*!
   * \brief Assignment operator
   */
  inline print_one &
  operator=(const print_one &o) {
    ios=o.ios;
    return *this;
  }
  /*!
   * \brief Non-throwing swap
   */
  inline void
  swap(print_one &o) {
    std::swap(ios, o.ios);
  }
  /*!
   * \brief Destructor
   */
  inline
  ~print_one() {}

  /*!
   * \brief Call operator
   */
  void operator()(const T &x) const {
    *ios << x << " ; ";
  }
};

}

/*!
 * \fn void print_range (const Range &r, S &ios)
 *
 * \brief Prints the range r in ios
 *
 * \param r : a range
 * \param ios : an output stream
 *
 * The formatting is : typeid(Range)"[ r0 ; r1 ; ... ; rn ; ]".
 */
template<class Range, class S>
inline void
print_range(const Range &r, S &ios) {
  ios // << typeid(Range).name()
      << "[ ";
  typedef typename boost::range_value<const Range>::type T;
  boost::for_each(r, detail::print_one<S, T>(ios));
  ios << " ] ";
}

}

#endif /* PRINT_HXX_ */
