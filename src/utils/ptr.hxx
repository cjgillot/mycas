/*
 * utils/ptr.hxx
 *
 *  Created on: 14 juin 2011
 *      Author: k1000
 */

#ifndef UTILS_PTR_HXX_
#define UTILS_PTR_HXX_

#include "operators.hxx"
#include "algebra/compare.hxx"

namespace util {

/*!
 * \class ptr
 *
 * \brief pointer class
 *
 * This proxy class behaves like
 * a raw pointer, forwarding printing and ordering
 * operators.
 */
template<class T>
class ptr
: public operators::ordered<ptr<T>
, operators::printable<ptr<T> >
> {
  T* p;

public:
  /*!
   * \brief Default constructor
   */
  inline
  ptr()
  : p(0) {}
  /*!
   * \brief Pointer (cast)-constructor
   */
  inline
  ptr(T* p)
  : p(p) {}

public:
  /*!
   * \brief Copy constructor
   */
  inline
  ptr(const ptr &o)
  : p(o.p) {}
  /*!
   * \brief Assignment operator
   */
  inline ptr &
  operator=(const ptr &o) {
    p=o.p;
    return *this;
  }
  /*!
   * \brief Non-throwing swap
   */
  inline void
  swap(ptr &o) {
    std::swap(p, o.p);
  }

public:
  /*!
   * \brief Destructor
   */
  inline
  ~ptr() {}

public:
  /*!
   * \brief Accessing the underlying pointer
   */
  inline T*
  get() const
  { return p; }
  /*!
   * \brief Dereference operator
   */
  inline T&
  operator*() const
  { return *p; }
  /*!
   * \brief Member dereference operator
   */
  inline T*
  operator->() const
  { return p; }

public:
  /*!
   * \brief Pointer deletion member function
   */
  inline void
  del() {
    BOOST_STATIC_ASSERT(sizeof(T) > 0);
    delete p;
  }

public:
  /*!
   * \brief Printing function forwarding
   */
  template<class S>
  inline void
  print(S &ios) const {
    ios << *p;
  }
  /*!
   * \brief Comparison function forwarding
   */
  static inline int
  compare(const ptr &a, const ptr &b) {
    return algebra::compare(*a.p, *b.p);
  }
};

} // namespace util

#endif /* UTILS_PTR_HXX_ */
