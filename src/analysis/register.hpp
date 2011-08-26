#ifndef REGISTER_HPP
#define REGISTER_HPP

#include<boost/noncopyable.hpp>

#include "util/refcounted.hpp"

#include "rtti.hpp"
#include "rtti/visitor.hpp"

namespace analysis {

/*!
 * Visitor usage :
 * \code
 *  using namespace analysis;
 *
 *  struct my_visitor {
 *
 *    MAKE_VISITOR(
 *      my_visitor  // the visitor class
 *    , basic       // the visited base class
 *    , (visited1)  // the visited class as a boost preprocessor sequence
 *      (visited2)
 *      ...
 *    )
 *
 *    // override void visit( const visited & ) for each visited class
 *    void visit( const basic & );
 *  };
 * \endcode
 *
 * You must always implement <em>visit( const basic & )</em>
 * for handling default case, or you'll get
 * a compiler error.
 *
 * All \c visited classes must be have the \c REGISTER_CLASS
 * macro called within their body, else you'll get undefined behaviour.
 *
 * It is safe to declare more visited types than overrides,
 * since the compiler will automatically upcast.
 *
 * \c template visit overrides be used since there
 * are no virtual functions involved.
 */
namespace visitor = ::rtti::visitor;

} // namespace analysis

//! \brief ABC case
#define REGISTER_BASE( klass )  \
  MAKE_REFCOUNTED( klass );           \
  ABSTRACT_RTTI( klass );             \
public:                               \
  MAKE_VISITABLE( klass )

//! \brief Derived case
#define REGISTER_CLASS( klass, parent ) \
  IMPLEMENT_RTTI( klass, parent );

//! \brief Final case
#define REGISTER_FINAL( klass, parent ) \
  FINAL_RTTI( klass, parent );

#endif
