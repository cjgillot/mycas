/*
 * utils/refcounted.hxx
 *
 *  Created on: 14 juin 2011
 *      Author: k1000
 */

#ifndef UTILS_REFCOUNTED_HXX_
#define UTILS_REFCOUNTED_HXX_

namespace util {

/*!
 * \class refcounted
 *
 * \brief Abstract base class for refcounted objects.
 *
 * This abstract class holds a reference counter and
 * defines the two {intrusive_ptr_*} functions used
 * by boost::intrusive_ptr.
 *
 * Constructor semantic :
 *   all constructor initialize {refcount} to 0
 *   since they create a new object.
 *
 * Assignement semantic :
 *   the {refcount} is not modified since we still use
 *   the same object.
 *
 * FIXME : shall we make this class {boost::noncopyable} ?
 */
class refcounted {
  mutable unsigned char refcount;

protected:
  /*!
   * \brief Default constructor
   */
  inline
  refcounted()
  : refcount(0) {}
  /*!
   * \brief Copy constructor
   */
  inline
  refcounted(const refcounted &o)
  : refcount(0) {}
  /*!
   * \brief Assignment operator
   */
  inline refcounted &
  operator=(const refcounted &o)
  { return *this; }

  /*!
   * \brief Pure virtual destructor
   */
  virtual
  ~refcounted() = 0;

public:
  /*!
   * \brief Reference counter incrementation for boost::intrusive_ptr
   */
  friend inline void
  intrusive_ptr_add_ref(const refcounted* it) {
    assert(it != 0);
    ++it->refcount;
  }
  /*!
   * \brief Reference counter decrementation for boost::intrusive_ptr,
   *   with deleting if {refcount} gets 0.
   */
  friend inline void
  intrusive_ptr_release(const refcounted* it) {
    assert(it != 0);
    if(--it->refcount == 0)
      delete it;
  }
  /*!
   * \brief Simple function testing if referenced pointer is unique
   *    (ie. if {refcount == 1}).
   */
  friend bool
  unique(const refcounted* it) {
    return it->refcount == 1;
  }
};

inline
refcounted::~refcounted() {}

}

#endif /* UTILS_REFCOUNTED_HXX_ */
