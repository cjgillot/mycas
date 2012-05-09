#include "analysis/vectorseq/sum.hpp"
#include "analysis/vectorseq/prod.hpp"

#include "analysis/expr.hpp"

#include "analysis/vectorseq/vectorseq.tpp"

using namespace analysis;

template class vectorseq<prod, power>;

prod::prod(const number &n)
: super(n) {}

// operations
prod* prod::mul(const prod &a, const prod &b)
{
  const number &c = a.coef() * b.coef();
  std::unique_ptr< prod > tmp ( new prod( c ) );
  tmp->construct_add( a, b );
  return tmp.release();
}
prod* prod::div(const prod &a, const prod &b)
{
  ASSERT( ! b.coef().null() );
  const number &c = a.coef() / b.coef();
  std::unique_ptr< prod > tmp ( new prod( c ) );
  tmp->construct_sub( a, b );
  return tmp.release();
}
prod* prod::inv(const prod &b)
{
  ASSERT( ! b.coef().null() );
  const number &c = b.coef().inv();
  std::unique_ptr< prod > tmp ( new prod( c ) );
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
  ptr<const power> pa = a->as_power();
  std::unique_ptr<prod> tmp ( new prod( 1 ) );
  tmp->construct_monomial( pa.get() );
  return tmp.release();
}
prod* prod::from_number(const number &n)
{
  return new prod(n);
}

// eval
expr prod::eval(unsigned lv) const
{
  const number &c = coef();

  // (* c) -> c
  if( empty() )
    return c;

  // (* 0 ...) -> 0
  if( c.null() )
    return 0;

  if( lv == 0 )
    return basic::eval( 0 );

  // (* c x) ?
  if( super::is_monomial() )
  {
    const power* mono = super::monomial();
    const expr &x = mono->eval( lv - 1 );

    // (* 1 x) -> x
    if( c.unit() )
      return x;

    // expand the addition
    // (* c (+ c' a...)) -> (+ cc' ca...)
    if( x.is_a< sum >() )
      return sum::sca( c, *x.as_a< sum >() );
  }

  return basic::eval( lv );
}
