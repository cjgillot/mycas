/*
 * multiply.hpp
 *
 *  Created on: 21 mai 2011
 *      Author: k1000
 */

#ifndef SERIES_SPARSE_MULTIPLY_HPP
#define SERIES_SPARSE_MULTIPLY_HPP

#include "stdlib.hpp"
#include "utils.hpp"

#include "stream.hpp"

#include "polynomial/sparse/multiply/heap.hpp"

#include "series/sparse/multiply/muler.hpp"

namespace series {
namespace sparse {
namespace detail {

using namespace streams;

// due to flow requirements,
// null coefficients are allowed
template<class M>
class mul_iterator
: public iterator_base<M>
{
  multiply::muler<M> impl;

private:
  mul_iterator();

  inline // used by clone()
  mul_iterator(const mul_iterator &o)
  : impl(o.impl)
  {}

public:
  inline
  mul_iterator(iterator_base<M>* l, iterator_base<M>* r)
  : impl(l,r) {}

  inline
  ~mul_iterator() {}

public:
  inline iterator_base<M>*
  clone() const {
    return new mul_iterator(*this);
  }

  inline iterator_base<M>*
  incr() {
    assert(!impl.empty());
    impl.next();
    return impl.empty() ? 0 : this;
  }

  inline M
  deref() const {
    assert(!impl.empty());
    return impl.get();
  }
};

template<class M>
inline iterator_base<M>*
prod(iterator_base<M>* a, iterator_base<M>* b) {
  if(!a || !b) return 0;
  if(a->empty() || b->empty()) return 0;
  return new mul_iterator<M>(a,b);
}


}}} // namespace series::sparse::detail

#endif // SERIES_SPARSE_MULTIPLY_HPP
