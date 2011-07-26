#include "analysis/stdfunc.hpp"
#include "analysis/function/function.ipp"

using namespace analysis;

class exponential
: public function<1> {

  typedef function<1> super;

public:
  exponential(const expr &a)
  : super("exp", a) {}

  ~exponential() {}

  exponential(const exponential &o)
  : super(o) {}

  exponential* clone() const
  { return new exponential(*this); }

  expr diff(const symbol &s, unsigned n = 1) const
  {
    if( n == 0 )
      return expr(this);

    const expr &a = super::arg();

    if( ! a.has(s) )
      return number::zero();

    expr ret = a.diff(s) * expr(this);

    if( n > 1 )
      return ret.diff(s, n-1);

    return ret;
  }

  expr eval(unsigned lv) const
  {
    const expr &a = super::arg();
    a.eval(lv);

    return expr(this);
  }

};

namespace analysis {

expr exp(const expr &a)
{
  return expr( new exponential(a) );
}

}
