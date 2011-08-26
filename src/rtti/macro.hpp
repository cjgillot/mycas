#ifndef RTTI_MACRO_HPP
#define RTTI_MACRO_HPP

#include "rtti/rttifwd.hpp"
#include "rtti/getter.hpp"

/*!\name RTTI declaration macros
 *
 * These macros define a custom \c RTTI
 * mechanism internal to the hierarchy.
 * A type id is represented by a \c rtti::rtti_type integer,
 * uniquely defined for each class.
 * This implementation enables faster type comparisons
 * than with standard \c typeid / \c std::typeinfo.
 *
 * These define the following private members :
 * - typedef ... \c rtti_base_type : the base class of hierarchy
 *     (the class which has used \c DECLARE_RTTI or \c ABSTRACT_RTTI)
 * - typedef ... \c rtti_super_type : the closest base class
 * - typedef ... \c rtti_self_type : the class itself
 * - static  \c rtti_id     : return the current class type id (defined in each subclass)
 * - virtual \c rtti_get    : return the current instance type id (defined in each subclass)
 *     (equivalent to \c self_type::rtti_id)
 *
 * These define also following protected member type :
 * - \c rtti_base_type : the base class of hierarchy
 *     (the class which has used \c DECLARE_RTTI or \c ABSTRACT_RTTI)
 *
 * \invariant Base class has id 0
 * \invariant All generated id's are different
 *    (but order is implementation defined)
 *
 * If your compiler issues some encapsulation error
 * on the \c IMPLEMENT_RTTI macro, double ckeck that
 * the first argument is the surrounding class and that
 * the second argument is an effective base.
 *
 * \{
 */
//@{

//! \brief Friend class declaration to keep everything \c protected
#define RTTI_FRIEND_DECL \
  friend class RTTI_GETTER;

//! \brief Declare \c klass as hierarchy root
#define RTTI_DECL_TYPES( klass )  \
  typedef const klass rtti_base_type;   \
  typedef const void  rtti_super_type;  \
  typedef const klass rtti_self_type;

/*!
 * \brief Import member types from parent
 * We use a "<tt>typedef typename</tt>" in order to
 * allow \c template in hierarchy.
 */
#define RTTI_IMPORT_TYPES( klass, parent ) \
  typedef const klass rtti_self_type; \
  typedef typename              \
    parent::rtti_base_type      \
      rtti_base_type;           \
  typedef typename              \
    parent::rtti_self_type      \
      rtti_super_type;

//! \brief prototype of \c rtti_get function
#define RTTI_DECL_GET       \
  virtual                   \
    const ::rtti::rtti_node*\
    ATTRIBUTE_PURE          \
      rtti_get()            \
        const               \
        throw()

//! \brief pure virtual \c rtti_get function
#define RTTI_PURE_GET \
  RTTI_DECL_GET = 0;

//! \brief implemented \c rtti_get function
#define RTTI_IMPL_GET( klass )  \
  inline RTTI_DECL_GET          \
  { return RTTI_TYPE_NODE( klass ); }

//! \brief Enum flags for some optimizations
#define RTTI_FLAGS( abstract, final ) \
  enum {                              \
    rtti_is_abstract = abstract       \
  , rtti_is_final = final             \
  } // ;

//! \brief Common part of base case
#define RTTI_BASE_DECL( klass ) \
  RTTI_FRIEND_DECL              \
protected:                      \
  RTTI_DECL_TYPES( klass )

// ***** externally used macros ***** //
//! \brief Abstract base case
#define ABSTRACT_RTTI( klass )  \
  RTTI_BASE_DECL( klass )       \
private:                        \
  RTTI_PURE_GET                 \
  RTTI_FLAGS( true, false )

//! \brief Base case
#define DECLARE_RTTI( klass )   \
  RTTI_BASE_DECL( klass )       \
private:                        \
  RTTI_IMPL_GET( klass )        \
  RTTI_FLAGS( false, false )

//! \brief Derived case
#define IMPLEMENT_RTTI( klass, parent ) \
  RTTI_FRIEND_DECL                      \
protected:                              \
  RTTI_IMPORT_TYPES( klass, parent )    \
private:                                \
  RTTI_IMPL_GET( klass )                \
  RTTI_FLAGS( false, false )

//! \brief Final case
#define FINAL_RTTI( klass, parent )   \
  RTTI_FRIEND_DECL                    \
private:                              \
  RTTI_IMPORT_TYPES( klass, parent )  \
  RTTI_IMPL_GET( klass )              \
  RTTI_FLAGS( false, true )
//@}
/*! \} */

#endif
