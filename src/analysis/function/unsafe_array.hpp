#ifndef ANALYSIS_FUNCTION_UNSAFE_ARRAY_HPP
#define ANALYSIS_FUNCTION_UNSAFE_ARRAY_HPP

#include <boost/type_traits/alignment_of.hpp>
#include <boost/aligned_storage.hpp>

namespace analysis {
namespace detail {

template<class T>
struct make_storage {

  typedef ::boost::aligned_storage<
    sizeof( T )
  , boost::alignment_of<T>::value
  > type;

};

template<class T, std::size_t N>
class unsafe_array {

  typedef typename make_storage<T[N]>::type storage_type;

public: // member typedefs
  typedef T         value_type;
  typedef T*        pointer;
  typedef const T*  const_pointer;
  typedef T&        reference;
  typedef const T&  const_reference;

  typedef std::size_t     size_type;
  typedef std::ptrdiff_t  difference_type;

  typedef pointer                               iterator;
  typedef std::reverse_iterator<iterator>       reverse_iterator;

  typedef const_pointer                         const_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

private:
  T* address() const
  {
    return static_cast<T*>( m_storage.address() );
  }

  unsafe_array&
  operator=(const unsafe_array&);

public: // cdtor
  template<class Iterator>
  unsafe_array(Iterator b, const Iterator &e)
  {
    assert( std::distance( b, e ) == N );

    T* cur = address();

    for(
      std::size_t cnt = N;
      cnt != 0;
      --cnt, ++b, ++cur
    ) {
      // redundant with the distance assert
      assert( b != e );

      new(cur) T (*b);
    }

    // redundant with the distance assert
    assert( b == e );
  }

  unsafe_array(const unsafe_array &o)
  {
    T* cur =   address()
    , ocur = o.address();

    for(
      std::size_t cnt = N;
      cnt != 0;
      --cnt, ++cur, ++ocur
    )
      new(cur) T (*ocur);
  }

  ~unsafe_array() throw()
  {
    T* cur = address();

    for(
      std::size_t cnt = N;
      cnt != 0;
      --cnt, ++cur
    )
      cur->~T();
  }

public: // access
  bool empty() const
  { return false; }

  size_type size() const
  { return N; }

public: // iterators
  iterator begin()
  { return address(); }

  const_iterator begin() const
  { return address(); }

  iterator end()
  { return address() + N; }

  const_iterator end() const
  { return address() + N; }

  reverse_iterator rbegin()
  { return reverse_iterator( end() ); }

  const_reverse_iterator rbegin() const
  { return const_reverse_iterator( end() ); }

  reverse_iterator rend()
  { return reverse_iterator( begin() ); }

  const_reverse_iterator rend() const
  { return const_reverse_iterator( begin() ); }

public: // front/back
  reference front()
  { return *begin(); }

  const_reference front() const
  { return *begin(); }

  reference back()
  { return *( end() - 1 ); }

  const_reference back() const
  { return *( end() - 1 ); }

private:
  storage_type m_storage;
};

}} // namespace analysis::detail

#endif
