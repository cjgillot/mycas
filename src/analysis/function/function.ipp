#ifndef FUNCTION_IPP
#define FUNCTION_IPP

#include <algorithm>
#include <boost/array.hpp>

#include "analysis/function/function.hpp"

#include "analysis/expr.ipp"
#include "analysis/basic.ipp"

namespace analysis {

template<unsigned N>
template<class InputIterator>
function<N>::function(
  const std::string   &n,
  const InputIterator &b,
  const InputIterator &e
)
: m_name(n)
, m_args(b,e)
{}

template<unsigned N>
function<N>::function(const function &o)
: basic(o)
, m_name(o.m_name)
, m_args(o.m_args)
{}


template<unsigned N>
function<N>::~function()
{}

template<unsigned N>
inline void
function<N>::sort()
{ std::sort(m_args.begin(), m_args.end()); }

namespace detail {

// utility iterator class for sign_sort
template<class Iter>
class indexer_t
: boost::iterator_adaptor<
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
    super_type::increment();
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

} // namespace detail

template<unsigned N>
inline bool
function<N>::sign_sort() {
  typedef unsigned index_t;

  typedef boost::array<index_t, N> perm_t;
  perm_t perm;

  //! sort an indexed vector
  {
    //! temporary indexed array to be sorted
    typedef std::pair< expr, index_t > pair_t;
    typedef container::unsafe_array< pair_t, N > tmp_t;

    tmp_t tmp (
      detail::indexer( m_args.begin() )
    , detail::indexer( m_args.end() )
    );

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
          m_args.begin()
        , perm.begin()
        )
      )
    );
  }

  //! now compute the sign, via the number of even cycles in \c perm
  bool sign = false; // empty permutation ( = identity ) is even
  for( std::size_t i = N; i != 0; --i )
  {
    const unsigned start = perm[ i ];

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

      if( length & 1 == 0 )
        sign = ! sign;
    }
  }
  return sign;
}

}

#endif
