#include "analysis/stdfunc.hpp"
#include "analysis/function/function.ipp"

#include "analysis/stdfunc/exp.hpp"

using namespace analysis;

template<>
symbol exp_::super::m_name( "exp" );

inline
exp_::exp_(const expr &a)
: super(a) {}

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

expr analysis::exp(const expr &a)
{
  return expr( new exp_(a) );
}
