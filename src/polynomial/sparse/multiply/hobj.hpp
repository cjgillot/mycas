/*
 * hobj.hpp
 *
 *  Created on: 18 juin 2011
 *      Author: k1000
 */

#ifndef SPARSE_MULTIPLY_HOBJ_HPP_
#define SPARSE_MULTIPLY_HOBJ_HPP_

#include "stdlib.hpp"

#include "polynomial/sparse/multiply/heap.hpp"
#include "polynomial/sparse/multiply/forward.hpp"

namespace poly {
namespace sparse {
namespace multiply {

/*!
 * \struct hobj
 *
 * \brief Heap objects in polynomial
 *    multiplication implementation.
 *
 * This object represents an online
 * monomial-polynomial product {fm * g}.
 * {g} is the range [gbeg : gend).
 * {cur} is the first monomial of this product.
 */
template<class P, class Mul>
struct hobj
: operators::ordered<hobj<P, Mul> >
, boost::noncopyable
, private Mul {

private:
  typedef detail::muler_type<P, Mul> types;
  typedef typename types::iter iter;

public:
  //! \brief The monomial type
  typedef typename types::mono mono;

private:
  const mono &fm;
  iter gbeg;
  const iter gend;

  mono cur;

private:
  hobj();

public:
  /*!
   * \brief The scalar product constructor
   *
   * \param f0 : a monomial
   * \param g : a monomial range representing a polynomial
   */
  inline
  hobj(const mono &f0, const P &g)
  : fm(f0)
  , gbeg(boost::const_begin(g)), gend(boost::const_end(g))
  , cur(Mul::operator()(fm, *gbeg))
  {}
  /*!
   * \brief Destructor
   */
  inline
  ~hobj() {}

  /*!
   * \brief Current monomial value
   * \return a const reference to {cur}
   */
  inline const mono &
  value() const
  { return cur; }

  /*!
   * \brief Updating function
   *
   * This method transforms {this} from
   * {fm * [gbeg : gend)} into {fm * [gbeg++ : gend)}.
   *
   * \return true if there is still terms, false if empty
   */
  inline bool
  update() {
    assert(gbeg != gend);

    ++gbeg;

    if(gbeg == gend) return false;

    cur = Mul::operator()(fm, *gbeg);

    return true;
  }

  /*!
   * \brief Comparison function for the heap
   *
   * The comparison function is right :
   * we want a max-heap on the exponent.
   */
  static inline int
  compare(const hobj &a, const hobj &b) {
    return algebra::compare(a.value(), b.value());
  }
};

}}} // namespace poly::sparse::multiply

#endif /* SPARSE_MULTIPLY_HOBJ_HPP_ */
