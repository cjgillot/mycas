/*
 * stdlib.hxx
 *
 *  Created on: 21 mai 2011
 *      Author: k1000
 */

#ifndef STDLIB_HXX_
#define STDLIB_HXX_

#include<iostream>
#include<algorithm>
#include<numeric>
#include<cassert>
#include<vector>
#include<string>
#include<deque>
#include<list>

#include<ext/slist>

namespace std_ext = __gnu_cxx;

#include<boost/foreach.hpp>
#define foreach BOOST_FOREACH
#define reverse_foreach BOOST_REVERSE_FOREACH

#include<boost/range.hpp>
#include<boost/range/combine.hpp>
#include<boost/range/adaptors.hpp>
#include<boost/range/algorithm.hpp>
#include<boost/range/counting_range.hpp>
#include<boost/range/algorithm_ext/push_back.hpp>

#include<boost/iterator.hpp>
#include<boost/iterator/zip_iterator.hpp>
#include<boost/iterator/iterator_facade.hpp>
#include<boost/iterator/transform_iterator.hpp>

#include<boost/operators.hpp>

#include<boost/optional.hpp>

#include<boost/type_traits.hpp>

#include<boost/bind.hpp>

/*
#include<boost/lambda/bind.hpp>
#include<boost/lambda/lambda.hpp>

using boost::lambda::_1;
using boost::lambda::_2;
using boost::lambda::_3;
*/

#include<boost/intrusive_ptr.hpp>

#endif /* STDLIB_HXX_ */
