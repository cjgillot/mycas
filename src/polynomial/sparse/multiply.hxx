/*
 * multiply.hxx
 *
 *  Created on: 21 mai 2011
 *      Author: k1000
 */

#ifndef SPARSE_MULTIPLY_HXX
#define SPARSE_MULTIPLY_HXX

#include "stdlib.hxx"

namespace poly {
namespace sparse {
namespace multiply {

template<class M, class HO, class Impl>
struct heap
: private imperative::heap::chain<HO*>
, private boost::noncopyable
{
  typedef imperative::heap::chain<HO*> super;

  mutable bool ready;
  mutable M cur;

public:
  explicit inline
  heap(size_t n)
  : super(n)
  , ready(false)
  , cur()
  {}
  inline
  ~heap() {
    while(!empty()) {
      std::list<HO*> min = super::find_mins();
      foreach(HO* m, min) delete m;
      super::delete_mins();
    }
  }

public:
  using super::empty;
  using super::insert;

  inline const M &
  get() const {
    if(ready) return cur;

    const std::list<HO*> &mins = super::find_mins();

    cur = algebra::zero<M>();
    boost::for_each(mins,
       boost::lambda::var(cur)
    += boost::lambda::bind(&HO::value, *_1));

    ready = true;
    return cur;
  }
  inline void
  next() {
    std::list<HO*> ml = super::find_mins();

    super::delete_mins();

    ready = false;

    static_cast<Impl*>(this)->update(ml);
  }
};

namespace poly {

template<class P>
struct hobj
: algebra::ordered<hobj<P>
> {
  typedef typename P::mono mono;

private:

  const mono &fm;
  typename P::const_iterator gbeg, gend;

  mono cur;

private:
  hobj();
  hobj(const hobj &);
  hobj &operator=(const hobj &);

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

  /* the comparison function is right :
   * we want a max-heap on the exponent */
  static inline int
  compare(const hobj &a, const hobj &b) {
    return algebra::compare(a.value(), b.value());
  }
};

template<class P>
class poly
: private heap<typename P::mono, hobj<P>, poly<P> > {

  typedef typename P::mono mono;
  typedef hobj<P> ho_t;
  typedef heap<mono, ho_t, poly> super;

protected:
  friend class heap<mono, ho_t, poly>;

  inline void
  update(const std::list<ho_t*> &ml) {
    foreach(ho_t* m, ml) {
      if(m->update()) super::insert(m);
      else delete m;
    }
  }

public:
  inline
  poly(const P &a, const P &b)
  : super(a.size())
  {
    assert(!a.empty() && !b.empty());

    foreach(const mono &m, a)
      super::insert(new ho_t(m, b));
  }
  inline
  ~poly() {}

  using super::empty;
  using super::get;
  using super::next;
};

}

namespace series {

// base abstract class for obj0 and obj1
template<class M>
struct hobj {
protected:
  M cur;

private:
  hobj();
  hobj(const hobj &);
  hobj &operator=(const hobj &);

public:
  explicit inline
  hobj(const M &c)
  : cur(c) {}

  virtual inline
  ~hobj() {}

public:
  inline const M &
  value() const
  { return cur; }

  virtual hobj*
  update() = 0;

  /* the comparison function is reversed :
   * we want a min-heap on the exponent */
  static inline int
  compare(const hobj &a, const hobj &b) {
    return algebra::compare(b.cur, a.cur);
  }
};

template<class M, class P2>
struct obj0
: public hobj<M> {
  typedef hobj<M> super;

private:
  const M &fm;
  P2 g;

public:
  inline
  obj0(const M &f0, const P2 &git)
  : super(f0 * *git)
  , fm(f0), g(git)
  {}
  inline virtual
  ~obj0() {}

public:
  inline super*
  update() {
    ++g;
    super::cur = fm * *g;
    return 0;
  }
};

template<class P1, class P2>
struct obj1
: public hobj<typename P1::mono> {
  typedef typename P1::mono mono;

  typedef hobj<mono> super;

private:
  P1 f;
  P2 g;

  const mono &g0;

public:
  inline
  obj1(const P1 &fit, const P2 &git)
  : super(*f * *g)
  , f(fit), g(git), g0(*g)
  {}
  inline
  ~obj1() {}

public:
  virtual super*
  update() {
    mono f0 = *f; ++f;

    super::cur = *f * g0;

    return new obj0<mono, P2>(f0, g+1);
  }
};

template<class P1, class P2>
class series
: private heap
    < typename P1::mono
    , hobj<typename P1::mono>
    , series<P1, P2>
    > {

  typedef typename P1::mono mono;
  typedef hobj<mono> ho_t;
  typedef heap<mono, ho_t, series> super;

protected:
  friend class heap<mono, ho_t, series>;

  inline void
  update(const std::list<ho_t*> &ml) {
    foreach(ho_t* m, ml) {
      ho_t* o = m->update();
      super::insert(m);
      if(o) super::insert(o);
    }
  }

public:
  inline
  series(const P1 &a, const P2 &b)
  : super(1)
  {
    assert(!a.empty() && !b.empty());
    super::insert(new obj1<P1, P2>(a, b));
  }
  inline
  ~series() {}

  using super::empty;
  using super::get;
  using super::next;
};


}

}}} // poly::sparse::multiply

#endif // SPARSE_MULTIPLY_HXX
