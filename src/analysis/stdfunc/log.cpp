#include "analysis/stdfunc.hpp"
#include "analysis/stdfunc/log.hpp"

#include "analysis/expr.ipp"
#include "analysis/function/function.ipp"

using namespace analysis;

template<>
symbol log_::m_name( "log" );

inline log_::log_(const expr &a)
: super(m_name, a) {}

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
  return new log_(a);
}
