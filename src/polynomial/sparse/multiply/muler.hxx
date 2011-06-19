/*
 * muler.hxx
 *
 *  Created on: 18 juin 2011
 *      Author: k1000
 */

#ifndef SPARSE_MULTIPLY_MULER_HXX_
#define SPARSE_MULTIPLY_MULER_HXX_

#include "stdlib.hxx"

#include "polynomial/sparse/multiply/heap.hxx"
#include "polynomial/sparse/multiply/forward.hxx"
#include "polynomial/sparse/multiply/hobj.hxx"

namespace poly {
namespace sparse {
namespace multiply {

template<class P, class Mul>
class muler
: private detail::muler_type<P, Mul>::type {

  typedef detail::muler_type<P, Mul> types;
  typedef typename types::type super;

  typedef typename types::mono mono;
  typedef hobj<P, Mul> ho_t;

protected:
  // super needs update(ml)
  friend class detail::muler_type<P, Mul>::type;
  template<class Range>
  inline void
  do_update(const Range &ml) {
    foreach(util::ptr<ho_t> m, ml) {
      if(m->update()) super::insert(m);
      else delete m.get();
    }
  }

public:
  inline
  muler(const P &a, const P &b)
  : super(a.size())
  {
    assert(!a.empty() && !b.empty());

    // shall we prefer allocate a ho_t array of size a.size() ?
    // for now, poly::update and heap::~heap do delete
    foreach(const mono &m, a)
      super::insert(new ho_t(m, b));

    super::sync();
  }
  inline
  ~muler() {}

  using super::empty;
  using super::get;
  using super::next;
};

}}} // namespace poly::sparse::muler

#endif /* SPARSE_MULTIPLY_MULER_HXX_ */
