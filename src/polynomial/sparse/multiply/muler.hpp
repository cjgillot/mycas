/*
 * muler.hpp
 *
 *  Created on: 18 juin 2011
 *      Author: k1000
 */

#ifndef SPARSE_MULTIPLY_MULER_HPP_
#define SPARSE_MULTIPLY_MULER_HPP_

#include "stdlib.hpp"

#include "polynomial/sparse/multiply/heap.hpp"
#include "polynomial/sparse/multiply/forward.hpp"
#include "polynomial/sparse/multiply/hobj.hpp"

namespace poly {
namespace sparse {
namespace multiply {

/*!
 * \class muler
 * \brief Multiplication class for sparse polynomials.
 *
 * Given {a} and {b} two polynomials, this class
 * constructs a heap containing {hobj}s, ie.
 * objects computing online the scalar products
 * [ {a[i]} * {b} ].
 */
template<class P, class Mul>
class muler
: private detail::muler_type<P, Mul>::type {

  typedef detail::muler_type<P, Mul> types;
  typedef typename types::type super;

  typedef typename types::mono mono;
  typedef hobj<P, Mul> ho_t;

private:
  /*!
   * \brief update class supplied to super
   *
   * This function handles the updating of
   * the {hobj}s, and proper re-inserting.
   */
  friend class detail::muler_type<P, Mul>::type;
  template<class Range>
  inline void
  do_update(const Range &ml) {
    foreach(util::ptr<ho_t> m, ml) {
      if(m->update()) super::insert(m);
      else delete m.get();
    }
  }

private:
  muler();

public:
  /*!
   * \brief Constructor
   *
   * Constructs a muler representing {a * b}.
   * time and heap size : linear in {a.size()}
   *
   * \param a,b : two polynomials
   */
  inline
  muler(const P &a, const P &b)
  : super(a.size()) {
    assert(!a.empty() && !b.empty());

    // shall we prefer allocate a ho_t array of size a.size() ?
    // for now, poly::update and heap::~heap do delete
    foreach(const mono &m, a)
      super::insert(new ho_t(m, b));

    super::sync();
  }

  /*!
   * \brief Destructor
   *
   * All memory handling is made by super.
   */
  inline
  ~muler() {}

  //! \brief Emptiness test
  using super::empty;

  //! \brief Current monomial access
  using super::get;

  //! \brief Next monomial computing
  using super::next;
};

}}} // namespace poly::sparse::muler

#endif /* SPARSE_MULTIPLY_MULER_HPP_ */
