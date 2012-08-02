#include "analysis/vectorseq/sum.hpp"

#include "analysis/expr.hpp"

#include "analysis/vectorseq/vectorseq.tpp"

using namespace analysis;

template class vectorseq<sum, prod>;

sum::sum(const number &n)
: super(n) {}

// operations
sum* sum::add_num(const sum &a, const number &b)
{
  std::unique_ptr< sum > tmp ( new sum( a ) );
  tmp->coef() += b;
  return tmp.release();
}
sum* sum::sub_num(const sum &a, const number &b)
{
  std::unique_ptr< sum > tmp ( new sum( a ) );
  tmp->coef() -= b;
  return tmp.release();
}
sum* sum::add(const sum &a, const sum &b)
{
  const number &c = a.coef() + b.coef();
  std::unique_ptr< sum > tmp ( new sum( c ) );
  tmp->construct_add( a, b );
  return tmp.release();
}
sum* sum::sub(const sum &a, const sum &b)
{
  const number &c = a.coef() - b.coef();
  std::unique_ptr< sum > tmp ( new sum( c ) );
  tmp->construct_sub( a, b );
  return tmp.release();
}
sum* sum::neg(const sum &b)
{
  const number &c = b.coef().neg();
  std::unique_ptr< sum > tmp ( new sum( c ) );
  tmp->construct_neg( b );
  return tmp.release();
}
sum* sum::sca(const number &n, const sum &b)
{
  const number &c = n * b.coef();
  std::unique_ptr< sum > tmp ( new sum( c ) );
  tmp->construct_sca( n, b );
  return tmp.release();
}

// test
bool sum::null() const
{
  return coef().null()
      && empty();
}

// static members
sum* sum::from_basic(const basic* a)
{
  ptr<const prod> pa = a->as_prod();
  std::unique_ptr<sum> tmp ( new sum( 0 ) );
  tmp->construct_monomial( pa.get() );
  return tmp.release();
}
sum* sum::from_number(const number &n)
{
  return new sum(n);
}

// eval
expr sum::eval(unsigned lv) const {
  const number &c = coef();

  if( super::empty() )
    return c;

  if( c.null() && super::is_monomial() )
    return super::monomial()->eval( lv - 1 );

  return basic::eval( lv );
}
