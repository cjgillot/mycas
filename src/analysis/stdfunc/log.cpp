#include "analysis/stdfunc.hpp"
#include "analysis/function/function.ipp"

#include "analysis/stdfunc/log.hpp"

namespace analysis {

inline
log_::log_(const expr &a)
: super("log", a) {}

inline
log_::log_(const log_ &o)
: super(o) {}

log_::~log_() {}

log_* log_::clone() const
{ return new log_(*this); }

expr log_::diff(const symbol &s, unsigned n) const
{
  if( n == 0 )
    return expr(this);

  const expr &a = super::arg<0>();

  if( ! a.has(s) )
    return number::zero();

  const expr &ret = a.diff(s) / a;

  return n == 1 ? ret
  : ret.diff(s, --n);
}

expr log_::eval(unsigned lv) const
{
  const expr &a = super::arg<0>();
  a.eval(--lv);

  return expr(this);
}

expr log(const expr &a)
{
  return expr( new log_(a) );
}

} // namespace analysis
