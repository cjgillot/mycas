#include "analysis/expr.hpp"

#include "analysis/vectorseq.hpp"

using namespace analysis;

const unsigned expr::default_eval_depth = 10;

//!\brief Expression comparison function
//! This is the fundamental method for expression canonical form.
//! \see basic::compare
util::cmp_t
expr::compare(const expr &a, const expr &b)
{
  if(a.m_impl == b.m_impl)
    return 0;

  util::cmp_t c = basic::compare(*a.m_impl, *b.m_impl);
  if(c) return c;

  // here, a == b
  util::unify_ptr(a.m_impl, b.m_impl);
  return 0;
}

// operations
#define CLONE_TO( argp, arg, klass )            \
  ptr< const klass > argp = arg.is_a< klass >() \
  ? static_cast< klass* >( arg.get()->clone() ) \
  : arg.get()->as_##klass() //;
#define PREPARE( argp, arg, klass )       \
  ptr< const klass > argp ( (arg).get()->as_##klass() ) //;

#define IS_NUM(a) ((a).is_numerical())
#define NUM( a ) ((a).as_a< numerical >())

#define OPERATE_NUM( arge, argn, klass, op ) do { \
  CLONE_TO( retp, arge, klass );                  \
  const_cast< number& >( retp->coef() )           \
  op NUM( argn )->get();                          \
  return retp->eval( default_eval_depth );        \
} while(0) //;
#define OPERATE_BIN( a, b, klass, fn ) do {       \
  PREPARE( ap, a, klass );                        \
  PREPARE( bp, b, klass );                        \
  expr ret ( klass::fn( *ap, *bp) );              \
  ret.eval();                                     \
  return ret;                                     \
} while(0) //;

#define BINARY( klass, name, op )                 \
expr expr::name(const expr &a, const expr &b) {   \
  bool an = IS_NUM(a), bn = IS_NUM(b);            \
  if( an & bn ) return numerical::name( *NUM(a), *NUM(b) );  \
  if( bn ) OPERATE_NUM( a, b, klass, op );        \
  if( an ) OPERATE_NUM( b, a, klass, op );        \
  OPERATE_BIN( a, b, klass, name );               \
}

BINARY( sum , add, += )
BINARY( sum , sub, -= )

BINARY( prod, mul, *= )
BINARY( prod, div, /= )

#undef BINARY
#undef OPERATE_BIN
#undef OPERATE_NUM

#define OPERATE_UN( klass, op ) do {\
  PREPARE( ap, a, klass );          \
  expr ret ( klass::op( *ap ) );    \
  ret.eval();                       \
  return ret;                       \
} while(0) //;

#define UNARY( klass, name )        \
expr expr::name(const expr &a) {    \
  if( IS_NUM( a ) )                 \
    return numerical::name( *NUM( a ) );  \
  OPERATE_UN( klass, name );        \
}

UNARY( sum , neg )
UNARY( prod, inv )

#undef UNARY
#undef OPERATE_UN

expr expr::pow(const expr &b, const expr &e)
{
  if( IS_NUM(b) && IS_NUM(e) )
    return numerical::pow( *NUM(b), *NUM(e) );
  return b.get()->pow(e);
}

#undef IS_NUM
#undef NUM

#undef CLONE_TO
#undef PREPARE

// numerical cache

#include <boost/preprocessor/repeat.hpp>
#include <boost/preprocessor/enum.hpp>

namespace {
struct numerical_cache_guard
{
  numerical_cache_guard(numerical* array)
  {
#define INCREF( z, n, offset )  intrusive_ptr_add_ref( &array[n + offset] );
    BOOST_PP_REPEAT( EXPR_NUMERIC_CACHE_LIMIT, INCREF, 0 )
    intrusive_ptr_add_ref( &array[EXPR_NUMERIC_CACHE_LIMIT] );
    BOOST_PP_REPEAT( EXPR_NUMERIC_CACHE_LIMIT, INCREF, EXPR_NUMERIC_CACHE_LIMIT + 1 )
#undef INCREF
  }
  ~numerical_cache_guard() {}
};
}

const numerical* expr::small_numeric_cache(long n)
{
  static numerical cache[ 2 * EXPR_NUMERIC_CACHE_LIMIT + 1 ] = {
#define ITEM( z, n, offset )  ( n + offset )
    BOOST_PP_ENUM( EXPR_NUMERIC_CACHE_LIMIT, ITEM, -EXPR_NUMERIC_CACHE_LIMIT ),
    ( 0l ),
    BOOST_PP_ENUM( EXPR_NUMERIC_CACHE_LIMIT, ITEM, 1 )
#undef ITEM
  };
  static numerical_cache_guard guard( cache );
  return &cache[ n + EXPR_NUMERIC_CACHE_LIMIT ];
}
