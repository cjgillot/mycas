#ifndef EXPAND_HEAPMUL_HPP
#define EXPAND_HEAPMUL_HPP

#include <util/foreach.hpp>
#include <algorithm>

namespace analysis {
namespace expand_detail {

struct heap_obj_base {
  std::size_t hash;

  heap_obj_base()
  : hash( 0 ) {}

  ~heap_obj_base() {}

  void swap( heap_obj_base &o )
  { std::swap( hash, o.hash ); }
};

template<class epair, class Iter>
struct heap_obj
: heap_obj_base {

  heap_obj(const epair &f_, const Iter &git_, const Iter &gen_)
  : heap_obj_base()
  , f(f_), git(git_), gen(gen_)
  , cur(f_ * *git_)
  {
    hash = cur.sort_hash();
    ++git;
  }

  ~heap_obj() {}

  void swap( heap_obj &o )
  {
    heap_obj_base::swap( o );
    f.swap( o.f );
    cur.swap( o.cur );
    std::swap( git, o.git );
    std::swap( gen, o.gen );
  }

  bool valid() const
  { return git != gen; }

  const epair &get() const
  { return cur; }

  void next()
  {
    ASSERT( git != gen );
    cur = f + *git;
    hash = cur.sort_hash();
    ++git;
  }

private:
  epair cur;
  const epair f;

  Iter git;
  const Iter gen;
};

struct predicate
: std::binary_function<
    const heap_obj_base&
  , const heap_obj_base&
  , bool
> {
  typedef heap_obj_base hob;

  inline bool
  operator()( const hob &a, const hob &b ) const
  { return a.hash < b.hash; }
};

/*!Heap multiplication (n < m)
 *
 * Multiplication : n*m
 * Comparison : n*m*lg(n)
 * Space : n
 */
template<class handle, class RRangen class ARange>
void expand_heap(RRange &ret, const ARange &F, const ARange &G)
{
  if( F.size() > G.size() )
  {
    expand_heap( ret, G, F );
    return;
  }

  std::vector<heap_obj<epair> > heap;
  heap.reserve( F.size() );

  std::make_heap( heap.begin(), heap.end(), predicate() );

  foreach( const handle &f, F ) {
    heap.push_back( heap_obj( f, G.begin(), G.end() ) );
    std::push_heap( heap.begin(), heap.end(), predicate() );
  }

  ASSERT( ! heap.empty() );

  // TODO encourage coefficient cancelling
  while( ! heap.empty() )
  {
    const heap_obj &ho = heap.front();
    ret.push_back( ho.get().get() );

    if( ho.valid() )
    {
      std:: pop_heap( heap.begin(), heap.end(), predicate() );
      heap.back().next();
      std::push_heap( heap.begin(), heap.end(), predicate() );
    }
    else
    {
      std:: pop_heap( heap.begin(), heap.end(), predicate() );
      heap.pop_back();
    }
  }
}

}} // namespace analysis::expand_detail

namespace std {

template<class E, class I>
void swap( analysis::expand_detail::heap_obj<E,I> &a
         , analysis::expand_detail::heap_obj<E,I> &b )
{ a.swap( b ); }

}

#endif // EXPAND_HEAPMUL_HPP
