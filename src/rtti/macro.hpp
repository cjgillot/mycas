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
 * - virtual \c rtti_get    : return the current instance type id (defined in each subclass)
 *
 * These also define the following protected member type :
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
#define RTTI_DECL_TYPES( klass )        \
protected:                              \
  typedef const klass rtti_self_type;   \
  typedef const klass rtti_base_type;   \
  typedef const void  rtti_super_type;

/*!
 * \brief Import member types from parent
 * We use a "<tt>typedef typename</tt>" in order to
 * allow \c template in hierarchy.
 */
#define RTTI_IMPORT_TYPES( klass, parent )  \
protected:                                  \
  typedef const klass rtti_self_type;       \
  using parent::rtti_base_type;             \
  typedef typename                          \
    parent::rtti_self_type                  \
      rtti_super_type;

//! \brief prototype of \c rtti_get function
#define RTTI_DECL_GET         \
  virtual                     \
    const ::rtti::rtti_node*  \
    ATTRIBUTE_PURE            \
      rtti_get()              \
        const throw()

//! \brief pure virtual \c rtti_get function
#define RTTI_PURE_GET \
  RTTI_DECL_GET = 0;

//! \brief implemented \c rtti_get function
#define RTTI_IMPL_GET( klass )  \
  inline RTTI_DECL_GET          \
  { return RTTI_TYPE_NODE( klass ); }

//! \brief Enum flags for some optimizations
#define RTTI_FLAGS( abstract, final, static ) \
  enum {                              \
    rtti_is_abstract = abstract       \
  , rtti_is_final = final             \
  , rtti_is_static = static           \
  } // ;

//! \brief Register static \c rtti
#define RTTI_STATIC( id )       \
  enum { rtti_static_id = id };

//! \brief Common part of base case
#define RTTI_BASE_DECL( klass ) \
  RTTI_FRIEND_DECL              \
protected:                      \
  RTTI_DECL_TYPES( klass )

// ***** externally used macros ***** //
//! \brief Abstract base case
#define ABSTRACT_RTTI( klass, static_max )  \
  RTTI_BASE_DECL( klass )                   \
private:                                    \
  RTTI_PURE_GET                             \
  enum { rtti_static_max = static_max };    \
  RTTI_FLAGS( true, false, false )

//! \brief Base case
#define DECLARE_RTTI( klass, static_max )   \
  RTTI_BASE_DECL( klass )                   \
private:                                    \
  RTTI_IMPL_GET( klass )                    \
  enum { rtti_static_max = static_max };    \
  RTTI_FLAGS( false, false, false )

//! \brief Derived case
#define IMPLEMENT_RTTI( klass, parent ) \
  RTTI_FRIEND_DECL                      \
protected:                              \
  RTTI_IMPORT_TYPES( klass, parent )    \
private:                                \
  RTTI_IMPL_GET( klass )                \
  RTTI_FLAGS( false, false, false )

//! \brief Static derived case
#define STATIC_RTTI( klass, parent, id )  \
  RTTI_FRIEND_DECL                        \
protected:                                \
  RTTI_IMPORT_TYPES( klass, parent )      \
private:                                  \
  RTTI_IMPL_GET( klass )                  \
  RTTI_STATIC( id )                       \
  RTTI_FLAGS( false, false, true )

//! \brief Final case
#define FINAL_RTTI( klass, parent )   \
  RTTI_FRIEND_DECL                    \
private:                              \
  RTTI_IMPORT_TYPES( klass, parent )  \
  RTTI_IMPL_GET( klass )              \
  RTTI_FLAGS( false, true, false )

//! \brief Static final case
#define STATIC_FINAL_RTTI( klass, parent, id ) \
  RTTI_FRIEND_DECL                      \
private:                                \
  RTTI_IMPORT_TYPES( klass, parent )    \
  RTTI_IMPL_GET( klass )                \
  RTTI_STATIC( id )                     \
  RTTI_FLAGS( false, true, true )
//@}
/*! \} */

#endif

#include "rtti/creator.tpp"
#include "rtti/holder.tpp"
