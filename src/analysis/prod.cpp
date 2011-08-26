#include "analysis/sum.hpp"
#include "analysis/prod.hpp"

#include "analysis/expr.ipp"
#include "analysis/basic.ipp"

using namespace analysis;

prod::prod(const number &n)
: super(n) {}

// operations
prod* prod::mul(const prod &a, const prod &b)
{
  const number &c = a.coef() * b.coef();
  util::move_ptr< prod > tmp ( new prod( c ) );
  tmp->construct_add( a, b );
  return tmp.release();
}
prod* prod::div(const prod &a, const prod &b)
{
  ASSERT( ! b.coef().null() );
  const number &c = a.coef() / b.coef();
  util::move_ptr< prod > tmp ( new prod( c ) );
  tmp->construct_sub( a, b );
  return tmp.release();
}
prod* prod::inv(const prod &b)
{
  ASSERT( ! b.coef().null() );
  const number &c = b.coef().inv();
  util::move_ptr< prod > tmp ( new prod( c ) );
  tmp->construct_neg( b );
  return tmp.release();
}

// test
bool prod::null() const {
  return coef().null();
}

bool prod::unit() const {
  return coef().unit()
      && empty();
}

// static members
prod* prod::from_basic(const basic* a)
{
  boost::intrusive_ptr<const power> pa = a->as_power();
  util::move_ptr<prod> tmp ( new prod( number::one() ) );
  tmp->construct_monomial( pa.get() );
  return tmp.release();
}
prod* prod::from_numeric(const numeric* n)
{
  return new prod(n);
}

// eval
expr prod::eval(unsigned lv) const {
  const number &c = coef();

  // (* c) -> c
  if( empty() )
    return c.eval(lv);

  // (* 0 ...) -> 0
  if( c.null() )
    return number::zero();

  if( lv == 0 )
    return expr(this);
  --lv;

  // (* c x) ?
  if( super::is_mono() )
  {
    expr b ( super::mono() );
    b.eval(lv);

    // (* 1 x) -> x
    if( c.unit() )
      return b;

    // expand the addition
    // (* c (+ c' a...)) -> (+ cc' ca...)
    if( b.is_a< sum >() )
      return expr( sum::sca( c, *b.as_a< sum >() ) );
  }

  return basic::eval(++lv);
}
