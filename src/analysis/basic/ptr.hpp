#ifndef PTR_HPP_
#define PTR_HPP_

#include <boost/intrusive_ptr.hpp>

namespace analysis {

template<typename T>
using ptr = boost::intrusive_ptr<T>;

}


#endif /* PTR_HPP_ */
