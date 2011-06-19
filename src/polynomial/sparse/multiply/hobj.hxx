/*
 * hobj.hxx
 *
 *  Created on: 18 juin 2011
 *      Author: k1000
 */

#ifndef SPARSE_MULTIPLY_HOBJ_HXX_
#define SPARSE_MULTIPLY_HOBJ_HXX_

#include "stdlib.hxx"

#include "polynomial/sparse/multiply/heap.hxx"
#include "polynomial/sparse/multiply/forward.hxx"

namespace poly {
namespace sparse {
namespace multiply {

template<class P, class Mul>
struct hobj
: operators::ordered<hobj<P, Mul> >
, boost::noncopyable
, private Mul {

  typedef detail::muler_type<P, Mul> types;
  typedef typename types::mono mono;
  typedef typename types::iter iter;

private:
  const mono &fm;
  iter gbeg;
  const iter gend;

  mono cur;

private:
  hobj();

public:
  inline
  hobj(const mono &f0, const P &g)
  : fm(f0)
  , gbeg(boost::const_begin(g)), gend(boost::const_end(g))
  , cur(Mul::operator()(fm, *gbeg))
  {}
  inline
  ~hobj() {}

  inline const mono &
  value() const
  { return cur; }
  inline bool
  update() {
    assert(gbeg != gend);

    ++gbeg;

    if(gbeg == gend) return false;

    cur = Mul::operator()(fm, *gbeg);

    return true;
  }

  /* the comparison function is right :
   * we want a max-heap on the exponent */
  static inline int
  compare(const hobj &a, const hobj &b) {
    return algebra::compare(a.value(), b.value());
  }
};

}}} // namespace poly::sparse::multiply

#endif /* SPARSE_MULTIPLY_HOBJ_HXX_ */
