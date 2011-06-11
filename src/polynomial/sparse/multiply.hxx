/*
 * multiply.hxx
 *
 *  Created on: 21 mai 2011
 *      Author: k1000
 */

#ifndef SPARSE_MULTIPLY_HXX
#define SPARSE_MULTIPLY_HXX

#include "stdlib.hxx"
#include "imperative/heap.hxx"

namespace poly {
namespace sparse {
namespace multiply {

template<class M, class HO, class Impl>
struct heap
: private imperative::heap::chain<HO>
, private boost::noncopyable
{
  typedef imperative::heap::chain<HO> super;

  M cur;

private:
  heap();

public:
  explicit inline
  heap(size_t n)
  : super(n), cur()
  {}
  inline
  ~heap() {
    while(!empty()) {
      std::list<HO> max = super::find_maxs();
      foreach(HO m, max) delete m.get();
      super::delete_maxs();
    }
  }

public:
  using super::empty;

  inline const M &
  get() const
  { return cur; }
  inline void
  next() {
    {
      std::list<HO> ml = super::find_maxs();
      super::delete_maxs();
      static_cast<Impl*>(this)->do_update(ml);
    }

    sync();
  }

protected:
  using super::insert;

  // must be called after insertions/deletions
  inline void
  sync() {
    if(super::empty()) return;
    const std::list<HO> &maxs = super::find_maxs();
    cur = algebra::zero<M>();
    foreach(const HO &m, maxs)
      cur += m->value();
  }
};

template<class P>
struct hobj
: operators::ordered<hobj<P>
, operators::printable<hobj<P>
> >
, boost::noncopyable {
  typedef typename P::mono mono;

private:
  const mono &fm;
  typename P::const_iterator gbeg, gend;

  mono cur;

private:
  hobj();

public:
  inline
  hobj(const mono &f0, const P &g)
  : fm(f0)
  , gbeg(boost::const_begin(g)), gend(boost::const_end(g))
  , cur(*gbeg * fm)
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

    cur = fm * *gbeg;

    return true;
  }

  template<class S>
  inline void
  print(S &ios) const {
    ios << "hobj[ ";
    ios << value();
    ios << " ] ";
  }

  /* the comparison function is right :
   * we want a max-heap on the exponent */
  static inline int
  compare(const hobj &a, const hobj &b) {
    return algebra::compare(a.value(), b.value());
  }
};

template<class P>
class poly
: private heap<
    typename P::mono
  , util::ptr<hobj<P> >
  , poly<P>
> {

  typedef typename P::mono mono;
  typedef hobj<P> ho_t;
  typedef heap<mono, util::ptr<ho_t>, poly> super;

protected:
  // super needs update(ml)
  friend class heap<mono, util::ptr<ho_t>, poly>;
  inline void
  do_update(const std::list<util::ptr<ho_t> > &ml) {
    foreach(util::ptr<ho_t> m, ml) {
      if(m->update()) super::insert(m);
      else delete m.get();
    }
  }

public:
  inline
  poly(const P &a, const P &b)
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
  ~poly() {}

  using super::empty;
  using super::get;
  using super::next;
};

}}} // poly::sparse::multiply

#endif // SPARSE_MULTIPLY_HXX
