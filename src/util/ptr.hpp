#ifndef UTILS_PTR_HPP_
#define UTILS_PTR_HPP_

#include <boost/intrusive_ptr.hpp>

namespace util {

template<typename T>
using ptr = boost::intrusive_ptr<T>;

} // namespace util

#endif /* UTILS_PTR_HPP_ */
