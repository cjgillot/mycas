#include "analysis/expr.hpp"

#include "analysis/vectorseq.hpp"

using namespace analysis;

const unsigned expr::default_eval_depth = 10;

// tests
bool expr::null() const
{
  if( m_impl->null() )
  {
//     expr( number::zero() ).swap( *this );
    return true;
  }
  return false;
}
bool expr::unit() const
{
  if( m_impl->unit() )
  {
//     expr( number::one() ).swap( *this );
    return true;
  }
  return false;
}

#define PREPARE( argp, arg, klass )       \
  ptr< const klass > argp ( (arg).get()->as_##klass() )

#define OPERATE_BIN( klass, op )    \
  PREPARE( ap, a, klass );          \
  PREPARE( bp, b, klass );          \
  expr ret ( klass::op( *ap, *bp) );\
  ret.eval();                       \
  return ret // ;

#define BINARY( klass, name ) \
  expr expr::name(const expr &a, const expr &b) \
  { OPERATE_BIN( klass, name ); }

BINARY( sum , add )
BINARY( sum , sub )

BINARY( prod, mul )
BINARY( prod, div )

#undef BINARY
#undef OPERATE_BIN

#define OPERATE_UN( klass, op )     \
  PREPARE( ap, *this, klass );      \
  expr ret ( klass::op( *ap ) );    \
  ret.eval();                       \
  return ret // ;

#define UNARY( klass, name )        \
  expr expr::name() const           \
  { OPERATE_UN( klass, name ); }

UNARY( sum , neg )
UNARY( prod, inv )

#undef UNARY
#undef OPERATE_UN

#undef PREPARE

expr expr::pow(const expr &o) const
{
  return get()->pow( o );
}

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
