/*
 * combine.hxx
 *
 *  Created on: 13 juin 2011
 *      Author: k1000
 */

#ifndef COMBINE_HXX_
#define COMBINE_HXX_

#include "stdlib.hxx"

namespace poly {
namespace dense {
namespace multiply {

template<class Range1, class Range2>
struct add_range
: boost::range_adaptor<Range1> {

};

}}} // namespace poly::dense::multiply

#endif /* COMBINE_HXX_ */
