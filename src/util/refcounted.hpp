/*
 * utils/refcounted.hpp
 *
 *  Created on: 14 juin 2011
 *      Author: k1000
 */

#ifndef UTILS_REFCOUNTED_HXX_
#define UTILS_REFCOUNTED_HXX_

#include "stdlib.hpp"

namespace util {

/*!
 * \struct refcounter
 *
 * \brief A simple reference counter class
 *
 * This class handles the internal
 * integer counter.
 *
 * \invariant \c m_refc == 0 at construction and destruction
 *
 */
struct refcounter
: public boost::noncopyable {
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
 * \def MAKE_REFCOUNTED(klass)
 *
 * Macro implementing the refcounting in the class {klass}
 *
 * This adds a reference counter and
 * defines the two {intrusive_ptr_*} functions used
 * by boost::intrusive_ptr.
 *
 * The {refcounter} structure cares for itself.
 */
#define MAKE_REFCOUNTED(klass)   \
  private: \
    mutable refcounter GET_REFCOUNTED; \
    \
  public: \
    /*! \brief Ref counter incrementation for boost::intrusive_ptr */ \
    friend inline void \
    intrusive_ptr_add_ref(const klass* it) { \
      assert(it); it->GET_REFCOUNTED.grab(); \
    } \
    \
    /*! \brief Ref counter decrementation for boost::intrusive_ptr */ \
    friend void  \
    intrusive_ptr_release(const klass* it) {    \
      assert(it != 0);  \
      if(! it->GET_REFCOUNTED.drop())       \
        delete it;      \
    }   \
    \
    /*! \brief Test whether pointer is unique (ie. {refcount == 1}) */ \
    friend class refcount_access // missing ";"

/*!
 * \class refcounted
 *
 * \brief Abstract base class for reference counted objects.
 *
 * Constructor semantic :
 *   all constructors initialize {refcount} to 0
 *   since they create a new object.
 *
 * Destructor semantic : (pure virtual)
 *   the {refcount} must be 0 : no handler
 */
class refcounted
: public boost::noncopyable {
  MAKE_REFCOUNTED(refcounted);

protected:
  //! \brief Default constructor
  inline   refcounted() {}

  //! \brief Pure virtual destructor
  virtual ~refcounted() = 0;
};

inline
refcounted::~refcounted() {}

/*!
 * \struct refcount_access
 *
 * \brief Implementation of unique
 *
 * This class is used as friend by
 * the MAKE_REFCOUNTED(...) macro,
 * and enables the implementation of
 * {unique(const T*)}.
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
 * \param a,b : boost::intrusive_ptr's
 */
template<class T>
inline void
unify_ptr(
  boost::intrusive_ptr<T> &a
, boost::intrusive_ptr<T> &b
) {
  return refcount_access::unify(a,b);
}

}

#endif /* UTILS_REFCOUNTED_HXX_ */
