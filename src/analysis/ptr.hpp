#ifndef PTR_HPP_
#define PTR_HPP_

#include <boost/intrusive_ptr.hpp>
#include "util/compare.hpp"

#include "util/null.hpp"

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_convertible.hpp>

namespace analysis {

// template alias
template<class T>
class ptr
: public boost::intrusive_ptr<T>
{

  typedef boost::intrusive_ptr<T> super;

  struct enabler {};

public:
  ptr() {}

  ptr(T* p)
  : super( p ) {}

  template<class U>
  ptr(U* p)
  : super( p ) {}

  // default copy ctor
  template<class U>
  ptr(const ptr<U> &p
  , T* = static_cast< U* >( nullptr ) // enabler
  )
  : super( p.get() ) {}

  // default assignment
  template<class U>
  ptr& operator=(const ptr<U> &p)
  {
    super::operator=( p );
    return *this;
  }

  //! \brief Non-throwing swap
  void swap(ptr &o)
  { super::swap( o ); }

  ~ptr() throw() {}
};

}


#endif /* PTR_HPP_ */
