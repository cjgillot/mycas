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

template<class P, class S>
struct common: boost::noncopyable {
  typedef typename P::mono mono;
  S so;

  mutable bool ready;
  mutable mono cur;

  common(size_t l);

  bool empty() const;
  const mono &get() const;
  void next();
};

template<class P>
struct heap_obj {
  typedef typename P::mono mono;

private:
  struct mono_muler {
    typedef mono result_type;

    const mono *f0;

    explicit
    mono_muler(const mono &f)
    : f0(&f)
    {}

    mono operator()(const mono &g0) const
    { return *f0 * g0; }
  };

  boost::transformed_range
    < mono_muler
    , const P
    > gen;

  mono cur;

public:
  heap_obj(const mono &f0, const P &g);

  const mono &
  value() const;

  bool
  update();

  /* the comparison function is reversed :
   * we want a max-heap on the exponent */
  static int compare(const heap_obj &a, const heap_obj &b);
};

template<class P>
struct heap
: public imperative::heap::chain<heap_obj<P>*>
, private boost::noncopyable
{
  typedef heap_obj<P> ho;
  typedef imperative::heap::chain<heap_obj<P>*> super;

  heap(size_t);
  ~heap();

  void update(std::list<ho*> work);
};

template<class P>
struct poly: common<P, heap<P> > {
  typedef heap<P> str_t;
  typedef heap_obj<P> ho_t;
  typedef common<P, heap<P> > super;

  typedef typename P::mono mono;

  poly(const P &a, const P &b);
};

}}} // poly::sparse::multiply

#include "multiply.inl"

#endif // SPARSE_MULTIPLY_HXX
