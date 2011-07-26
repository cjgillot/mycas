#include "analysis/stdfunc.hpp"
#include "analysis/function/function.ipp"

using namespace analysis;

class logarithm
: public function<1> {

  typedef function<1> super;

public:
  logarithm(const expr &a)
  : super("log", a) {}

  ~logarithm() {}

  logarithm(const logarithm &o)
  : super(o) {}

  logarithm* clone() const
  { return new logarithm(*this); }

  expr diff(const symbol &s, unsigned n = 1) const
  {
    if( n == 0 )
      return expr(this);

    const expr &a = super::arg();

    if( ! a.has(s) )
      return number::zero();

    if( n > 1 )
      return diff(s).diff(s, n-1);

    return a.diff(s) / a;
  }

  expr eval(unsigned lv) const
  {
    const expr &a = super::arg();
    a.eval(lv);

    return expr(this);
  }

};

namespace analysis {

expr log(const expr &a)
{
  return expr( new logarithm(a) );
}

}
