#ifndef UTILS_REFCOUNTED_HPP_
#define UTILS_REFCOUNTED_HPP_

#include<new>

#include<boost/shared_ptr.hpp>
#include<boost/noncopyable.hpp>
#include<boost/intrusive_ptr.hpp>

#include "util/assert.hpp"
#include "util/attribute.hpp"

namespace util {

class refcount_access;

//! \brief Non-heap allocable
struct nonheapable {
private:
  void* operator new( std::size_t ) throw( std::bad_alloc );
  void* operator new( std::size_t, std::nothrow_t ) throw();

  void  operator delete( void* ) throw();
  void  operator delete( void*, std::nothrow_t ) throw();

  // leave placement operators -> containers give stack scoping
};

//! \brief Non-assignable class
struct nonassignable {
private:
  nonassignable& operator=(const nonassignable&) ATTRIBUTE_DELETED;
};

//! \brief Non-copyable class
struct noncopyable {
protected:
  noncopyable() {}
private:
  noncopyable(const noncopyable&) ATTRIBUTE_DELETED;
  noncopyable& operator=(const noncopyable&) ATTRIBUTE_DELETED;
};

/*!
 * \brief A simple reference counter class
 *
 * This class handles the internal
 * integer counter.
 *
 * \invariant \c m_refc == 0 at construction and destruction
 *
 * \todo Implement atomic operations
 */
struct refcounter
: private nonassignable {

public:
  //! \brief Default constructor
  inline  refcounter() throw()
  : m_refc(0) {}

  //! \brief Copy constructor
  inline  refcounter(const refcounter&) throw()
  : m_refc(0) {}

  //! \brief Destructor
  inline ~refcounter() throw()
  { ASSERT(m_refc == 0); }

public:
  //! \brief Incrementation
  inline void grab() throw()
  {
    ++m_refc;
    ASSERT_MSG( m_refc != 0, "Reference counter overflow" );
  }

  //! \brief Decrementation
  template<class T>
  inline void
  ATTRIBUTE_NONNULL(1)
  drop(const T* p) throw()
  {
    // T is known to be complete
    // since drop() is called by intrusive_ptr_release()
    if( --m_refc == 0 )
      delete p;
  }

  /*!
   * \brief Uniqueness test
   *
   * A pointer is unique in these cases :
   * - there is only one intrusive_ptr (\ref m_refc == 1)
   * - there is no intrusive_ptr (\ref m_refc == 0)
   */
  inline bool
  ATTRIBUTE_PURE
  unique() const throw()
  { return m_refc <= 1; }

public:
  //! \brief Reference count
  unsigned long m_refc;
};

/*!
 * \def REFCOUNT_NAME
 *
 * Macro giving the name of the \ref refcounter member
 */
#define REFCOUNT_NAME \
  _m_refcount_

/*!
 * \brief Macro implementing the refcounting in the class \c klass
 *
 * This adds a reference counter and
 * defines the two \c intrusive_ptr_* functions used
 * by \c boost::intrusive_ptr.
 *
 * The \ref refcounter structure cares for itself.
 */
#define MAKE_REFCOUNTED(klass)              \
  private:                                  \
    mutable ::util::refcounter              \
      REFCOUNT_NAME;                        \
  public:                                   \
    friend inline void                      \
    intrusive_ptr_add_ref(const klass* it)  \
    { it->REFCOUNT_NAME.grab(); }           \
    friend inline void                      \
    intrusive_ptr_release(const klass* it)  \
    { it->REFCOUNT_NAME.drop( it ); }       \
    friend class ::util::refcounter;        \
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
  static bool ATTRIBUTE_PURE
  unique(const T* it) {
    return it->REFCOUNT_NAME.unique();
  }
  //! \brief Pointer unification \see util::unify
  template<class T>
  static void unify(
    boost::intrusive_ptr<T> &a
  , boost::intrusive_ptr<T> &b
  ) {
    // get rid of the less referenced
    if(b->REFCOUNT_NAME.m_refc < a->REFCOUNT_NAME.m_refc) {
      b = a;
      return;
    }
    a = b;
  }
};

/*!
 * \brief Unique pointer test
 *
 * \param it : a reference counted pointer
 * \return true if the reference counter equals 0 or 1,
 *      false otherwise
 */
template<class T>
inline bool ATTRIBUTE_PURE
unique(const T* it) {
  return refcount_access::unique(it);
}

/*!
 * \brief Pointer unification
 *
 * If two pointers have the same dereferenced
 * value, they can be unified by this function.
 * It cares for getting rid of the less referenced.
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
  if(a.use_count() <= b.use_count())
    a = b;
  else
    b = a;
}

}

#endif /* UTILS_REFCOUNTED_HPP_ */
