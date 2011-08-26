#include "analysis/sum.hpp"

#include "analysis/expr.ipp"
#include "analysis/basic.ipp"

#include "container/unsafe_vector.hpp"

using namespace analysis;

sum::sum(const number &n)
: super(n) {}

// operations
sum* sum::add(const sum &a, const sum &b)
{
  const number &c = a.coef() + b.coef();
  util::move_ptr< sum > tmp ( new sum( c ) );
  tmp->construct_add( a, b );
  return tmp.release();
}
sum* sum::sub(const sum &a, const sum &b)
{
  const number &c = a.coef() - b.coef();
  util::move_ptr< sum > tmp ( new sum( c ) );
  tmp->construct_sub( a, b );
  return tmp.release();
}
sum* sum::neg(const sum &b)
{
  const number &c = b.coef().neg();
  util::move_ptr< sum > tmp ( new sum( c ) );
  tmp->construct_neg( b );
  return tmp.release();
}
sum* sum::sca(const number &n, const sum &b)
{
  const number &c = n * b.coef();
  util::move_ptr< sum > tmp ( new sum( c ) );
  tmp->construct_sca( n, b );
  return tmp.release();
}

// test
bool sum::null() const {
  return coef().null()
      && empty();
}

// static members
sum* sum::from_basic(const basic* a) {
  boost::intrusive_ptr<const prod> pa = a->as_prod();
  util::move_ptr<sum> tmp ( new sum( number::zero() ) );
  tmp->construct_monomial( pa.get() );
  return tmp.release();
}
sum* sum::from_numeric(const numeric* n) {
  return new sum(n);
}

// eval
expr sum::eval(unsigned lv) const {
  const number &c = coef();

  if( empty() )
    return c;

  if( c.null() && super::is_mono() ) {
    expr m ( super::mono() );
    m.eval(--lv);
    return m;
  }

  return basic::eval(lv);
}
