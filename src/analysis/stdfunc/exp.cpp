#include "analysis/stdfunc.hpp"
#include "analysis/stdfunc/exp.hpp"

#include "analysis/expr.hpp"
#include "analysis/function/function.ipp"

#include "analysis/stdfunc/log.hpp"

using namespace analysis;

template<>
symbol exp_::m_name( "exp" );

inline exp_::exp_(const expr &a)
: super(m_name, a) {}

exp_::~exp_() {}

exp_* exp_::clone() const
{ return new exp_(*this); }

expr exp_::differentiate(const symbol &s) const
{
  const expr &a = super::arg<0>();
  return a.diff(s) * expr(this);
}

expr analysis::exp(const expr &a)
{
  return new exp_(a);
}

expr exp_::eval(unsigned lv) const
{
  super::arg<0>().eval( --lv );

  if( super::arg<0>().null() )
    return 1;

  if( super::arg<0>().is_a<log_>() )
  {
    const log_* la = super::arg<0>().as_a<log_>();
    return la->arg<0>();
  }

  return basic::eval( ++lv );
}
