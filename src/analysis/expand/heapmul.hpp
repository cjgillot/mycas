#ifndef EXPAND_HEAPMUL_HPP
#define EXPAND_HEAPMUL_HPP

#include <util/foreach.hpp>
#include <algorithm>

namespace analysis {
namespace expand_detail {

struct heap_obj_base
{
  std::size_t chash;
  std::size_t shash;
};

template< class epair, class Iter >
struct heap_obj
: public heap_obj_base
{
  typedef heap_obj_base super;

public:
  heap_obj( const epair &f_, const Iter &git_, const Iter &gen_ )
  : super()
  , f( f_ ), git( git_ ), gen( gen_ )
  , cur( f_ * *git_ )
  {
    ++git;
    chash = cur.coef_hash();
    shash = cur.value_hash();
  }

  heap_obj( const heap_obj &o )
  : super( o )
  , f( o.f ), git( o.git ), gen( o.gen )
  , cur( o.cur )
  {}

  heap_obj &operator=(heap_obj o)
  { o.swap( *this ); return *this; }

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

  const epair &get() const
  { return cur; }

  void next()
  {
    ASSERT( git != gen );
    cur = f * *git;
    hash = cur.sort_hash();
    ++git;
  }
  
  // reverse predicate to have a min-heap
  friend inline bool
  operator<( const heap_obj &a, const heap_obj &b )
  {
    return ( a.chash  > b.chash )
        || ( a.chash == b.chash && a.shash > b.shash );
  }

private:
  epair f;

  Iter git, gen;

  epair cur;
};

/*!\brief Heap multiplication
 *
 * Complexity ( n < m )
 * - Multiplication : n*m
 * - Comparison : n*m*lg(n)
 * - Space : n
 */
template<class handle, class RRange, class ARange>
void expand_heap(
    RRange &ret,
    const handle &f0, const ARange &F,
    const handle &g0, const ARange &G
) {
  if( F.size() > G.size() )
  {
    expand_heap( ret, g0, G, f0, F );
    return;
  }

  typedef typename ARange::const_iterator iter_t;
  typedef heap_obj_base hob_t;
  typedef heap_obj<handle, iter_t> ho_t;

  std::vector<ho_t  > objs;  objs.reserve( F.size() + 2 );
  std::vector<hob_t*> heap;  heap.reserve( F.size() + 2 );

  {
    objs.push_back( ho_t( f0, G.begin(), G.end() ) );
    heap.push_back( &objs.back() );

    objs.push_back( ho_t( g0, F.begin(), F.end() ) );
    heap.push_back( &objs.back() );
  }
  foreach( const handle &f, F )
  {
    objs.push_back( ho_t( f, G.begin(), G.end() ) );
    heap.push_back( &objs.back() );
  }

  std::make_heap( heap.begin(), heap.end() );

  ASSERT( ! heap.empty() );

  // TODO help coefficient cancelling
  while( ! heap.empty() )
  {
    const ho_t* ho = static_cast< const ho_t* >( heap.front() );
    ret.push_back( ho->get().get() );

    if( ho.valid() )
    {
      std:: pop_heap( heap.begin(), heap.end() );
      static_cast<ho_t*>( heap.back() )->next() ;
      std::push_heap( heap.begin(), heap.end() );
    }
    else
    {
      std:: pop_heap( heap.begin(), heap.end() );
      heap. pop_back();
    }
  }
}

}} // namespace analysis::expand_detail

#endif // EXPAND_HEAPMUL_HPP
