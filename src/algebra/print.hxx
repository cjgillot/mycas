/*
 * print.hxx
 *
 *  Created on: 12 juin 2011
 *      Author: k1000
 */

#ifndef PRINT_HXX_
#define PRINT_HXX_

namespace algebra {

template<class Range, class S>
inline void
print_range(const Range &r, S &ios) {
  ios << typeid(Range).name() << "[ ";
  typedef typename boost::range_reference<const Range>::type cref_t;
  foreach(cref_t x, r)
    ios << x << " ; ";
  ios << " ] ";
}

}

#endif /* PRINT_HXX_ */
