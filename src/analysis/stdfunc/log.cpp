#include "analysis/stdfunc.hpp"
#include "analysis/stdfunc/log.hpp"

#include "analysis/expr_operators.hpp"

using namespace analysis;

template<>
symbol log_::super::m_name( "log" );

inline
log_::log_(const expr &a)
: super(a) {}

log_::~log_() {}

log_* log_::clone() const
{ return new log_(*this); }

expr log_::differentiate(const symbol &s) const
{
  const expr &a = super::arg<0>();
  return a.diff(s) / a;
}

expr analysis::log(const expr &a)
{
  return expr( new log_(a) );
}
