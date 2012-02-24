#ifndef UTILS_PTR_HPP_
#define UTILS_PTR_HPP_

#include "operators.hpp"

namespace util {

/*!
 * \class ptr
 *
 * \brief pointer class
 *
 * This proxy class behaves like
 * a raw pointer, forwarding printing operators.
 */
template<class T>
class ptr
: public operators::printable<ptr<T> >
{
  T* p;

public:
  //! \brief Default constructor
  inline
  ptr()
  : p(0) {}
  //! \brief Pointer (cast)-constructor
  inline
  ptr(T* p_)
  : p(p_) {}

public:
  //! \brief Copy constructor
  inline
  ptr(const ptr &o)
  : p(o.p) {}
  //! \brief Assignment operator
  inline ptr &
  operator=(const ptr &o) {
    p=o.p;
    return *this;
  }
  //! \brief Non-throwing swap
  inline void
  swap(ptr &o) {
    std::swap(p, o.p);
  }

public:
  //! \brief Destructor
  inline
  ~ptr() {}

public:
  //! \brief Accessing the underlying pointer
  inline T*
  get() const
  { return p; }
  //! \brief Dereference operator
  inline T&
  operator*() const
  { return *p; }
  //! \brief Member dereference operator
  inline T*
  operator->() const
  { return p; }

public:
  //! \brief Pointer deletion member function
  inline void
  del() {
    // checked delete
    BOOST_STATIC_ASSERT(sizeof(T) > 0);
    delete p;
  }

public:
  //! \brief Printing function forwarding
  template<class S>
  inline void
  print(S &ios) const {
    ios << *p;
  }
};

} // namespace util

#endif /* UTILS_PTR_HPP_ */
