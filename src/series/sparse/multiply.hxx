/*
 * multiply.hxx
 *
 *  Created on: 21 mai 2011
 *      Author: k1000
 */

#ifndef SERIES_SPARSE_MULTIPLY_HXX
#define SERIES_SPARSE_MULTIPLY_HXX

#include "stdlib.hxx"
#include "utils.hxx"

#include "stream.hxx"

#include "polynomial/sparse/multiply.hxx"

namespace series {
namespace sparse {
namespace multiply {

using streams::iterator_base;
using poly::sparse::multiply::heap;

// abstract base class for obj0 and obj1
template<class M>
struct hobj
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

  virtual hobj*
  update() = 0;

public:
  /* the comparison function is reversed :
   * we want a min-heap on the exponent */
  static inline int
  compare(const hobj &a, const hobj &b) {
    return algebra::compare(b.cur, a.cur);
  }
};

template<class M>
struct obj0
: public hobj<M> {

  typedef hobj<M> super;

private:
  const M &fm;
  boost::intrusive_ptr<
    iterator_base<M>
  > g;

private:
  inline
  obj0(const M &f0, iterator_base<M> *git)
  : fm(f0), g(git) {
    super::cur = f0 * git->deref();
  }

public:
  inline virtual
  ~obj0() {}

public:
  inline super*
  update() {
    g->incr();
    super::cur = fm * g->deref();
    return 0;
  }

public:
  static inline obj0<M>*
  make(const M &f0, iterator_base<M> *git) {
    if(algebra::null(f0)) return 0;
    return new obj0<M>(f0, git);
  }
};

template<class M>
struct obj1
: public hobj<M> {

  typedef hobj<M> super;

private:
  boost::intrusive_ptr<
    iterator_base<M>
  > f, g;

  const M &g0;
  M f0;

public:
  inline
  obj1(iterator_base<M> *fit, iterator_base<M> *git)
  : f(fit), g(git)
  , f0(fit->deref()), g0(git->deref()) {
    super::cur = f0 * g0;
  }
  inline
  ~obj1() {}

public:
  virtual super*
  update() {
    f->incr();

    M f02 = f->deref();

    std::swap(f0, f02);

    super::cur = f0 * g0;

    iterator_base<M> *g2 = g->clone();
    g2->incr();

    return obj0<M>::make(f02, g2);
  }
};

template<class M>
class muler
: private heap<
    M
  , util::ptr<hobj<M> >
  , muler<M>
  >
, public util::refcounted {

  typedef hobj<M> ho_t;
  typedef heap<M, util::ptr<ho_t>, muler> super;

protected:
  friend class heap<M, util::ptr<ho_t>, muler>;
  inline void
  do_update(const std::list<util::ptr<ho_t> > &ml) {
    foreach(util::ptr<ho_t> m, ml) {
      ho_t* o = m->update();
      super::insert(m);
      if(o) super::insert(o);
    }
  }

private:
  muler();

public:
  inline
  muler(iterator_base<M> *a, iterator_base<M> *b)
  : super(1) // initial size = 1 -> profiling ?
  {
    assert(!a->empty() && !b->empty());
    super::insert(new obj1<M>(a, b));
    super::sync();
  }
  inline
  ~muler() {}

public:
  using super::empty;
  using super::get;
  using super::next;
};

} // namespace multiply

namespace detail {

using namespace streams;

// due to flow requirements,
// null coefficients are allowed
template<class M>
class mul_iterator
: public iterator_base<M>
{
  boost::intrusive_ptr<
    multiply::muler<M>
  > impl;

public:
  inline
  mul_iterator()
  : impl() {}

private:
  inline
  mul_iterator(const mul_iterator &o)
  : impl(o.impl)
  {}

public:
  inline
  mul_iterator(iterator_base<M>* l, iterator_base<M>* r)
  : impl(new multiply::series<M>(l,r))
  {}

  inline
  ~mul_iterator() {}

public:
  inline iterator_base<M>*
  clone() const {
    return new mul_iterator(*this);
  }

  inline iterator_base<M>*
  incr() {
    assert(impl && !impl->empty());
    impl->next();
    return impl->empty() ? 0 : this;
  }

  inline M
  deref() const {
    assert(impl && !impl->empty());
    return impl->get();
  }
};

template<class M>
inline iterator_base<M>*
mul(iterator_base<M>* a, iterator_base<M>* b) {
  if(!a || !b) return 0;
  if(a->empty() || b->empty()) return 0;
  return new mul_iterator<M>(a,b);
}


} // namespace detail

}} // namespace series::sparse

#endif // SERIES_SPARSE_MULTIPLY_HXX
