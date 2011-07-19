/*
 * hobj.hpp
 *
 *  Created on: 18 juin 2011
 *      Author: k1000
 */

#ifndef SERIES_SPARSE_HOBJ_HPP_
#define SERIES_SPARSE_HOBJ_HPP_

#include "stdlib.hpp"
#include "utils.hpp"

#include "stream.hpp"

namespace series {
namespace sparse {
namespace multiply {

using streams::iterator_base;
using poly::sparse::multiply::heap;

// abstract base class for obj0 and obj1
template<class M>
class hobj
: boost::noncopyable {
protected:
  M cur;

protected:
  inline
  hobj() {}

public:
  virtual inline
  ~hobj() {}

public:
  inline const M &
  value() const
  { return cur; }

  virtual bool
  update(hobj* &) = 0;

public:
  /* the comparison function is reversed :
   * we want a min-heap on the exponent */
  static inline int
  compare(const hobj &a, const hobj &b) {
    return algebra::compare(b.cur, a.cur);
  }
};

}}} // namespace series::sparse::multiply

#endif /* SERIES_SPARSE_HOBJ_HPP_ */
