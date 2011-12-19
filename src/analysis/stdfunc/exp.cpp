#include "analysis/stdfunc.hpp"
#include "analysis/stdfunc/exp.hpp"

#include "analysis/expr.ipp"
#include "analysis/function/function.ipp"
#include "analysis/function/match.ipp"

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
