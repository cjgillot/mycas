/*
 * heap.hpp
 *
 *  Created on: 18 juin 2011
 *      Author: k1000
 */

#ifndef SPARSE_MULTIPLY_HEAP_HPP_
#define SPARSE_MULTIPLY_HEAP_HPP_

#include "stdlib.hpp"
#include "imperative/heap.hpp"

namespace poly {
namespace sparse {
namespace multiply {

/*!
 * \class heap
 * \brief Multiplication heap used by both
 *    polynomial and series multiplication
 *    algorithms.
 */
template<class M, class HO, class Impl>
class heap
: private imperative::heap::chain<HO>
{
  typedef imperative::heap::chain<HO> super;

  M cur;

  typedef typename super::list_t list_t;

private:
  heap();

public:
  /*!
   * \brief Sized constructor
   */
  explicit inline
  heap(size_t n)
  : super(n), cur()
  {}
  /*!
   * \brief Destructor
   *
   * This dtor HO::del() all
   * HO objects left in the heap.
   */
  inline
  ~heap() {
    while(!empty()) {
      boost::for_each(
        super::find_maxs()
      , std::mem_fun_ref(&HO::del)
      );
      super::delete_maxs();
    }
  }

public:
  /*!
   * \brief Copy constructor
   */
  inline
  heap(const heap &o)
  : super(o), cur(o.cur) {}
  /*!
   * \brief Assignement operator
   */
  inline heap &
  operator=(const heap &o) {
    super::operator=(o);
    cur=o.cur;
    return *this;
  }
  /*!
   * \brief Non-throwing swap
   */
  inline void
  swap(heap &o) {
    super::swap(o);
    std::swap(cur, o.cur);
  }

public:
  /*!
   * \brief Emptiness test
   */
  using super::empty;

  /*!
   * \brief Current monomial getter
   * \return the last computed monomial
   */
  inline const M &
  get() const
  { return cur; }

  /*!
   * \brief Next monomial computer
   *
   * This function calls the derived
   * Impl::do_update() function.
   */
  inline void
  next() {
    {
      list_t ml = super::find_maxs();
      super::delete_maxs();
      static_cast<Impl*>(this)->do_update(ml);
    }

    sync();
  }

protected:
  /*!
   * \brief Element insertion into the heap
   */
  using super::insert;

  /*!
   * \brief Current monomial recalculation.
   *
   * This function must be called after
   * any insertion/deletion in the heap,
   * in order to update the value of cur.
   */
  inline void
  sync() {
    if(super::empty()) return;
    cur = algebra::zero<M>();
    foreach(const HO &m, super::find_maxs())
      cur += m->value();
  }
};

}}} // namespace poly::sparse::multiply

#endif /* SPARSE_MULTIPLY_HEAP_HPP_ */
