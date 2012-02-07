#ifndef FUNCTION_IPP
#define FUNCTION_IPP

#include "analysis/function/function.hpp"

#include "analysis/expr.hpp"
#include "analysis/function/exprseq.ipp"

#include <algorithm>
#include <boost/array.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/iterator/zip_iterator.hpp>

#include "container/unsafe_vector.hpp"

namespace analysis {
namespace detail {

// utility iterator class for sign_sort
template<class Iter>
class indexer_t
: public boost::iterator_adaptor<
    indexer_t<Iter> // Derived
  , Iter // Base
  , std::pair<
      typename boost::iterator_value<Iter>::type
    , unsigned
    > const // Value
  , std::input_iterator_tag // Tag
  , std::pair<
      typename boost::iterator_value<Iter>::type
    , unsigned
    > const // Reference
  > {

  typedef typename indexer_t::iterator_adaptor_ super_type;

public:
  typedef typename super_type::value_type value_type;
  typedef typename super_type::reference reference;

  explicit
  indexer_t(const Iter &i)
  : super_type(i), m_index(0) {}

private:
  friend class boost::iterator_core_access;

  void increment()
  {
    ++super_type::base_reference();
    ++m_index;
  }

  reference
  dereference() const
  { return value_type( *this->base_reference(), m_index ); }

private: // data
  unsigned m_index;
};

template<class It>
inline indexer_t<It>
indexer(const It &c)
{ return indexer_t<It>(c); }

// partial ordering on std::pair<expr, unsigned>
struct less_pair {
  typedef std::pair<expr, unsigned> arg_t;

  inline bool
  operator()(const arg_t &a, const arg_t &b)
  { return a.first < b.first; }
};

template<class Tmp, class Perm, class Self>
void do_sort( Tmp &tmp, Perm &perm, Self &self )
{
  //! sorting according to the \c expr only
  std::sort(
    tmp.begin()
  , tmp.end()
  , detail::less_pair()
  );

  //! get back sorted \c m_args and the permutation
  boost::copy(
    tmp,

    boost::make_zip_iterator(
      boost::make_tuple(
        self.begin()
      , perm.begin()
      )
    )
  );
}

template<class Cont>
bool sign_perm( std::size_t sz, Cont &perm )
{
  //! now compute the sign, via the number of even cycles in \c perm
  bool sign = false; // empty permutation ( = identity ) is even
  for( ; sz != 0; --sz )
  {
    const unsigned start = perm[ sz ];

    // if not marked
    if( start != unsigned( -1 ) )
    {
      unsigned curr = start;
      unsigned length = 0;

      // traverse and mark the cycle
      do {
        const unsigned next = perm[ curr ];
        perm[ curr ] = unsigned( -1 ); // flag traversed
        ++length;

        curr = next;

      } while(curr != start);

      if( ( length & 1 ) == 0 )
        sign = ! sign;
    }
  }
  return sign;
}

template<unsigned N>
struct do_sign_sort
{
  template<class F>
  static inline bool apply( F &self )
  {
    typedef unsigned index_t;

    typedef boost::array<index_t, N> perm_t;
    perm_t perm;

    //! sort an indexed vector
    {
      //! temporary indexed array to be sorted
      typedef std::pair< expr, index_t > pair_t;
      typedef container::unsafe_array< pair_t, N > tmp_t;

      tmp_t tmp (
        detail::indexer( self.begin() )
      , detail::indexer( self.end() )
      );

      do_sort( tmp, perm, self );
    }

    return detail::sign_perm( N, perm );
  }
};

template<>
struct do_sign_sort<0>
{
  template<class F>
  static inline bool apply( F &self )
  {
    typedef unsigned index_t;

    const std::size_t sz = self.size();

    typedef container::unsafe_vector<index_t> perm_t;
    perm_t perm ( sz, 0 );

    //! sort an indexed vector
    {
      //! temporary indexed array to be sorted
      typedef std::pair< expr, index_t > pair_t;
      typedef container::unsafe_vector< pair_t > tmp_t;

      tmp_t tmp (
        sz
      , detail::indexer( self.begin() )
      , detail::indexer( self.end() )
      );

      do_sort( tmp, perm, self );
    }

    return detail::sign_perm( sz, perm );
  }
};

} // namespace detail

template<unsigned N>
inline void function<N>::sort()
{ std::sort( this->begin(), this->end() ); }

template<unsigned N>
bool function<N>::sign_sort()
{ return detail::do_sign_sort<N>::apply( *this ); }

}

#include "analysis/function/derivative.hpp"

#endif
