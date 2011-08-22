#include "analysis/stdfunc.hpp"
#include "analysis/function/function.ipp"

#include "analysis/stdfunc/exp.hpp"

namespace analysis {

inline
exp_::exp_(const expr &a)
: super("exp", a) {}

inline
exp_::exp_(const exp_ &o)
: super(o) {}

exp_::~exp_() {}

exp_* exp_::clone() const
{ return new exp_(*this); }

expr exp_::diff(const symbol &s, unsigned n) const
{
  if( n == 0 )
    return expr(this);

  const expr &a = super::arg<0>();

  if( ! a.has(s) )
    return number::zero();

  const expr &ret = a.diff(s) * expr(this);

  if( n > 1 )
    return ret.diff(s, --n);

  return ret;
}

expr exp_::eval(unsigned lv) const
{
  const expr &a = super::arg<0>();
  a.eval(--lv);

  return expr(this);
}

expr exp(const expr &a)
{
  return expr( new exp_(a) );
}

} // namespace analysis
