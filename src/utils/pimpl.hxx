/*
 * pimpl.hxx
 *
 *  Created on: 6 juil. 2011
 *      Author: k1000
 */

#ifndef PIMPL_HXX_
#define PIMPL_HXX_

#include "stdlib.hxx"

namespace util {

/*!
 * \brief Copy-on-write Pimpl implementation
 *
 * This class implements a Pimpl on top
 * of \c boost::shared_ptr.
 */
template<class T>
class cow_pimpl
: public operators::testable<cow_pimpl<T> > {
  boost::shared_ptr<const T> ptr;

public:
  //! \brief Default contructor : null pointer
  cow_pimpl() {}

  //! \brief Explicit copy constructor
  explicit
  cow_pimpl(const cow_pimpl &o)
  : ptr(o.ptr) {}
  //! \brief Assignment operator
  cow_pimpl &
  operator=(const cow_pimpl &o) {
    ptr=o.ptr;
    return *this;
  }
  //! \brief Non-throwing swap
  void swap(cow_pimpl &o) throw() {
    ptr.swap(o.ptr);
  }

  //! \brief Pointer constructor
  cow_pimpl(const T* p)
  : ptr(p) {}

  //! \brief Destructor
  ~cow_pimpl() {}

public:
  //! \brief Dereference operator
  const T &operator* () const { return *ptr; }
  //! \brief Member dereference operator
  const T *operator->() const { return ptr.operator->(); }
  //! \brief Pointer access
  const T *get() const { return ptr.get(); }

  //! \brief Pointer modification
  void reset(const T* p = 0)
  { ptr.reset(p); }

  /*!
   * \brief Copy-on-write function
   *
   * Calling this function is the only way to get
   * a mutable pointer. It copies properly the object
   * to get a modifiable reference.
   */
  T* cow() {
    if(! ptr.unique())
      ptr.reset(new T(*ptr));
    return const_cast<T*>(ptr.get());
  }

private:
  friend class operators::testable<cow_pimpl>;

  //! \brief Safe bool operator
  bool valid() const
  { return ptr; }
};

}

#endif /* PIMPL_HXX_ */
