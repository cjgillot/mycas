#include "analysis/expr.hpp"
#include "analysis/numeric.hpp"

#include "analysis/sum.hpp"
#include "analysis/prod.hpp"
#include "analysis/power.hpp"

#include "analysis/expr.ipp"
#include "analysis/basic.ipp"

using namespace analysis;

const unsigned expr::default_eval_depth = 10;

// tests
bool expr::null() const {
  if( m_impl->null() ) {
    expr( number::zero() ).m_impl.swap(m_impl);
    return true;
  }
  return false;
}
bool expr::unit() const {
  if( m_impl->unit() ) {
    expr( number::one() ).m_impl.swap(m_impl);
    return true;
  }
  return false;
}

void expr::eval(unsigned lv) const {
  m_impl->eval(lv).m_impl.swap(m_impl);
}

expr expr::diff(const symbol &s, unsigned n) const
{ eval(); return m_impl->diff(s,n); }


#define PREPARE_SELF(klass)   \
  const klass* ap =   get()->as_##klass()

#define OPERATE_SELF( klass, op )   \
  PREPARE_SELF( klass );            \
  expr ret ( klass::op( *ap ) );    \
  ret.eval();                       \
  return ret // ;

#define PREPARE_OTHER(klass)  \
  const klass* bp = b.get()->as_##klass()

#define PREPARE(klass)        \
  PREPARE_OTHER(klass);       \
  PREPARE_SELF (klass) // ;

#define OPERATE( klass, op )        \
  PREPARE( klass );                 \
  expr ret ( klass::op( *ap, *bp) );\
  ret.eval();                       \
  return ret // ;

#define OPERATOR( op, klass, name )             \
  expr expr::operator op( const expr &b ) const \
  { OPERATE( klass, name ); }

OPERATOR( +, sum , add )
OPERATOR( -, sum , sub )

OPERATOR( *, prod, mul )
OPERATOR( /, prod, div )

#undef OPERATOR
#undef OPERATE
#undef PREPARE
#undef PREPARE_OTHER

#define UNARY( klass, name )        \
  expr expr::name() const           \
  { OPERATE_SELF( klass, name ); }

UNARY( sum , neg )
UNARY( prod, inv )

#undef UNARY
#undef OPERATE_SELF
#undef PREPARE_SELF

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
