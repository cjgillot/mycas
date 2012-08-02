#ifndef EXPAND_HEAPMUL_HPP
#define EXPAND_HEAPMUL_HPP

#include "analysis/vectorseq.hpp"

#include "container/unsafe_vector.hpp"

#include <algorithm>

namespace analysis {
namespace expand_detail {

struct heap_obj_base
{
  std::size_t chash;
  std::size_t shash;
};

template< class EP, class M, class Iter >
struct heap_obj
: public heap_obj_base
{
  typedef heap_obj_base super;

public:
  heap_obj( const M* f_, const Iter &git_, const Iter &gen_ )
  : super()
  , f( f_ ), git( git_ ), gen( gen_ )
  , cur( EP::mul( f_, *git_ ) )
  {
    ++git;
    EP::sort_hash( cur.get(), chash, shash );
  }

  heap_obj(const heap_obj&) = default;
  heap_obj(heap_obj&&) = default;

  heap_obj &operator=(const heap_obj&) = default;
  heap_obj &operator=(heap_obj&&) = default;

  ~heap_obj() {}

  void swap( heap_obj &o )
  {
    f.swap( o.f );
    std::swap( git, o.git );
    std::swap( gen, o.gen );
    cur.swap( o.cur );
    std::swap( chash, o.chash );
    std::swap( shash, o.shash );
  }

public:
  bool valid() const
  { return git != gen; }

  const M* get() const
  { return cur.get(); }

  void next()
  {
    ASSERT( git != gen );
    cur.reset( EP::mul( f.get(), *git ) );
    EP::sort_hash( cur.get(), chash, shash );
    ++git;
  }

private:
  ptr<const M> f;

  Iter git, gen;

  ptr<const M> cur;
};

struct predicate
: std::binary_function<const heap_obj_base*, const heap_obj_base*, bool>
{
  // reverse predicate to have a min-heap
  inline bool operator()( const heap_obj_base *a, const heap_obj_base *b )
  {
    return ( a->chash  > b->chash )
        || ( a->chash == b->chash && a->shash > b->shash );
  }
};

/*!\brief Heap n-ary merging
 * n = #f, m = #g
 * Complexity
 * - Multiplication : n*m
 * - Comparison : n*m*lg(n)
 * - Space : n
 */
template<class Iter, class EP = prod::handle>
struct heap_nmerge
{
private:
  typedef heap_obj_base hob_t;
  typedef heap_obj<EP, prod, Iter> ho_t;

  container::unsafe_vector<ho_t  > objs;
  container::unsafe_vector<hob_t*> heap;

public:
  explicit heap_nmerge(std::size_t len)
  : objs(len), heap(len) {}
  ~heap_nmerge() = default;

private:
  heap_nmerge(const heap_nmerge&) = delete;
  heap_nmerge& operator=(const heap_nmerge&) = delete;
  heap_nmerge(heap_nmerge&&) = delete;
  heap_nmerge& operator=(heap_nmerge&&) = delete;

public:
  void add_one(const prod* scale, const Iter& beg, const Iter& end)
  {
    objs.emplace_back( scale, beg, end );
    heap.push_back( &objs.back() );
  }
  template<class RRange>
  void main(RRange& ret)
  {
    std::make_heap( heap.begin(), heap.end(), predicate() );

    ASSERT( ! heap.empty() );

    // TODO help coefficient cancelling
    while( ! heap.empty() )
    {
      const ho_t* ho = static_cast< const ho_t* >( heap.front() );
      ret.push_back( ho->get() );

      if( ho->valid() )
      {
        std:: pop_heap( heap.begin(), heap.end(), predicate() );
        static_cast<ho_t*>( heap.back() )->next() ;
        std::push_heap( heap.begin(), heap.end(), predicate() );
      }
      else
      {
        std:: pop_heap( heap.begin(), heap.end(), predicate() );
        heap. pop_back();
      }
    }
  }
};

}} // namespace analysis::expand_detail

namespace std {

template<class EP, class M, class I>
void swap(const analysis::expand_detail::heap_obj<EP,M,I> &a,
          const analysis::expand_detail::heap_obj<EP,M,I> &b)
{ a.swap( b ); }

}

#endif // EXPAND_HEAPMUL_HPP
