#include "analysis/stdfunc.hpp"
#include "analysis/stdfunc/log.hpp"

#include "analysis/expr.hpp"
#include "analysis/function/function.ipp"

#include "analysis/stdfunc/exp.hpp"

using namespace analysis;

inline log_::log_(const expr &a)
: super(s_id, a) {}

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

expr log_::eval(unsigned lv) const
{
  super::arg<0>().eval( --lv );

  if( super::arg<0>().unit() )
    return 0;

  if( super::arg<0>().is_a<exp_>() )
  {
    const exp_* ea = super::arg<0>().as_a<exp_>();
    return ea->arg<0>();
  }

  return basic::eval( ++lv );
}

#include "caml.hpp"

template<>
func_id log_::s_id = {
  symbol( "log" ),
  caml_hash_variant( LOG_HASH )
};
