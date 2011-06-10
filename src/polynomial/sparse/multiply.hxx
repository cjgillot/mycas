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

  M cur;

public:
  explicit inline
  heap(size_t n)
  : super(n), cur()
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

  inline const M &
  get() const
  { return cur; }
  inline void
  next() {
    {
      std::list<HO*> ml = super::find_mins();
      super::delete_mins();
      static_cast<Impl*>(this)->update(ml);
    }

    sync();
  }

private:
  friend class Impl;

  using super::insert;

  // must be called after external insertions
  inline void
  sync() {
    const std::list<HO*> &mins = super::find_mins();
    cur = algebra::zero<M>();
    foreach(HO* m, mins)
      cur += m->value();
  }
};

namespace poly {

template<class P>
struct hobj
: algebra::ordered<hobj<P> >
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
  // super needs update(ml)
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

} // namespace poly::sparse::multiply::poly

namespace series {

// could we get it without vtable ?

// abstract base class for obj0 and obj1
template<class M>
struct hobj
: boost::noncopyable {
protected:
  M cur;

private:
  hobj();

protected:
  explicit inline
  hobj(const M &c)
  : cur(c) {}

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

template<class M, class P1, class P2>
struct obj1
: public hobj<M> {

  typedef hobj<mono> super;

private:
  P1 f;
  P2 g;

  const M &g0;

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

    return new obj0<M, P2>(f0, g+1);
  }
};

template<class P1, class P2>
class series
: private heap
    < typename P1::mono
    , hobj<typename P1::mono>
    , series<P1, P2>
    >
, boost::noncopyable {

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

private:
  series();

public:
  inline
  series(const P1 &a, const P2 &b)
  : super(1) // initial size = 1 -> profiling ?
  {
    assert(!a.empty() && !b.empty());
    super::insert(new obj1<mono, P1, P2>(a, b));
  }
  inline
  ~series() {}

public:
  using super::empty;
  using super::get;
  using super::next;
};

} // namespace poly::sparse::multiply::series

}}} // poly::sparse::multiply

#endif // SPARSE_MULTIPLY_HXX

template<class M>
struct iterator_base {
  inline
  iterator_base() {}

  inline virtual
  ~iterator_base() {}

  virtual const M &
  dereference() = 0;

  // returns non-null if mutation
  virtual iterator_base*
  increment() = 0;
};

template<class M>
struct meta_iterator {
  typedef iterator<M> impl_t;
  std::auto_ptr<impl_t> impl;

public:
  inline
  meta_iterator()
  : impl() {}

  inline
  meta_iterator(impl_t* ptr)
  : impl(ptr) {}

  inline
  ~meta_iterator()
  {}

public:
  inline void
  increment() {
    assert(impl);
    impl_t* i2 = impl->increment();
    if(i2) impl = i2;
  }
  inline const M &
  dereference() const {
    assert(impl);
    return impl->dereference();
  }
};

template<class K, class M>
struct series_iterator
: boost::iterator_facade<
    series_iterator<K, M>
  , M const
  , boost::forward_traversal_tag
  > {

  typedef typename series<K>::list_t::const_iterator iter_t;

  const series<K> *serie;
  iter_t it;
  const iter_t end;

private:
  series_iterator();

public:
  inline
  series_iterator(const series_iterator &si)
  : serie(si.serie), it(si.it), end(si.end)
  {}
  inline series_iterator &
  operator=(const series_iterator &si) {
    serie = si.serie; it = si.it; end = si.end;
    return *this;
  }

public:
  inline
  series_iterator(const series<K> &s)
  : serie(&s)
  , it (boost::const_begin(s.values))
  , end(boost::const_end(s.values))
  {}

  inline
  ~series_iterator()
  {}

  inline void
  increment() {
    if(it == end) s->incr();
    ++it;
  }
  inline const M &
  dereference() const
  { return *it; }
};


template<class K>
class series {
  typedef monomial<K> mono;
  typedef std::list<mono> list_t;

  friend class series_iterator<K,mono>;

  mutable meta_iterator<mono> it;
  mutable list_t values;

private:
  inline const mono &
  incr() const {
    const mono &val = *it;
    values.push_back(val);
    return values.back();
  }

public:

};










