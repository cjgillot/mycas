/*
 * stdlib.hxx
 *
 *  Created on: 21 mai 2011
 *      Author: k1000
 */

#ifndef STDLIB_HXX_
#define STDLIB_HXX_

#include<algorithm>
#include<cassert>
#include<vector>
#include<list>

#include<boost/foreach.hpp>
#define foreach BOOST_FOREACH

#include<boost/range.hpp>
#include<boost/range/adaptors.hpp>
#include<boost/range/algorithm.hpp>
#include<boost/range/algorithm_ext/push_back.hpp>

#include<boost/iterator.hpp>
#include<boost/iterator/iterator_facade.hpp>

using boost::range_detail::safe_bool;

#include<boost/operators.hpp>

/*
#include<boost/lambda/bind.hpp>
#include<boost/lambda/lambda.hpp>

using boost::lambda::_1;
using boost::lambda::_2;
using boost::lambda::_3;
*/

#include<boost/intrusive_ptr.hpp>

#endif /* STDLIB_HXX_ */
