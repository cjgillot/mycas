/*
 * utils/refcounted.hpp
 *
 *  Created on: 14 juin 2011
 *      Author: k1000
 */

#ifndef UTILS_REFCOUNTED_HPP_
#define UTILS_REFCOUNTED_HPP_

#include "stdlib.hpp"

namespace util {

class refcount_access;

/*!
 * \struct refcounter
 *
 * \brief A simple reference counter class
 *
 * This class handles the internal
 * integer counter.
 *
 * \invariant \c m_refc == 0 at construction and destruction
 *
 * \todo Implement atomic operations
 *
 */
struct refcounter
: private boost::noncopyable {
  //! \brief Reference count
  unsigned long m_refc;

  //! \brief Default constructor
  inline
  refcounter()
  : m_refc(0) {}

  //! \brief Destructor
  inline
  ~refcounter()
  { assert(m_refc == 0); }

  //! \brief Incrementation
  inline void grab()
  { ++m_refc; }

  //! \brief Decrementation
  inline bool drop()
  { return --m_refc; }

  /*!
   * \brief Uniqueness test
   *
   * A pointer is unique if :
   * -> there is only one intrusive_ptr (m_refc == 1)
   * -> there is no intrusive_ptr (m_refc == 0)
   *
   */
  inline bool unique() const
  { return m_refc <= 1; }
};

/*!
 * \def GET_REFCOUNTED
 *
 * Macro giving the name of the refcount integer
 *
 */
#define GET_REFCOUNTED \
  _m_refcount_

/*!
 * \brief Macro implementing the refcounting in the class \c klass
 *
 * This adds a reference counter and
 * defines the two \c intrusive_ptr_* functions used
 * by \c boost::intrusive_ptr.
 *
 * The \c refcounter structure cares for itself.
 */
#define MAKE_REFCOUNTED(klass)   \
  private: \
    mutable ::util::refcounter GET_REFCOUNTED; \
    \
  public: \
    /*! \brief Ref counter incrementation for boost::intrusive_ptr */ \
    friend inline void \
    intrusive_ptr_add_ref(const klass* it) { \
      it->GET_REFCOUNTED.grab(); \
    } \
    \
    /*! \brief Ref counter decrementation for boost::intrusive_ptr */ \
    friend void  \
    intrusive_ptr_release(const klass* it) {    \
      if(! it->GET_REFCOUNTED.drop())       \
        delete it;      \
    }   \
    \
    /*! \brief Test whether pointer is unique (ie. {refcount == 1}) */ \
    friend class ::util::refcount_access // missing ";"

/*!
 * \brief Implementation of unique
 *
 * This class is used as friend by
 * the \c MAKE_REFCOUNTED(...) macro,
 * and enables the implementation of
 * \c unique().
 */
struct refcount_access {
  //! \brief Pointer uniqueness test \see util::unique
  template<class T>
  static bool unique(const T* it) {
    return it->GET_REFCOUNTED.unique();
  }
  //! \brief Pointer unification \see util::unify
  template<class T>
  static void unify(
    boost::intrusive_ptr<T> &a
  , boost::intrusive_ptr<T> &b
  ) {
    // get rid of the less referenced
    if(b->GET_REFCOUNTED.m_refc < a->GET_REFCOUNTED.m_refc) {
      b = a;
      return;
    }
    a = b;
  }
};

/*!
 * \brief Unique pointer test
 *
 * \param it : a ref-counted pointer
 * \return true if the ref-counter equals 1,
 *      false otherwise
 */
template<class T>
inline bool
unique(const T* it) {
  return refcount_access::unique(it);
}

/*!
 * \brief Pointer unification
 *
 * If two pointers have the same dereferenced
 * value, they can be unified by this function.
 * It cares for getting rid of the less referenced.
 *
 * \param a,b : \c boost::intrusive_ptr to be unified
 */
template<class T>
inline void
unify_ptr(
  boost::intrusive_ptr<T> &a
, boost::intrusive_ptr<T> &b
)
{ refcount_access::unify(a,b); }

template<class T>
inline void
unify_ptr(
  boost::shared_ptr<T> &a
, boost::shared_ptr<T> &b
) {
  const int ac = a.use_count(), bc = b.use_count();
  if(ac <= bc)
    a.reset(b);
  b.reset(a);
}

}

#endif /* UTILS_REFCOUNTED_HPP_ */
