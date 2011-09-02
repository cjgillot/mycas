#ifndef ANALYSIS_STDFUNC_EXP_HPP
#define ANALYSIS_STDFUNC_EXP_HPP

#include "analysis/stdfunc.hpp"
#include "analysis/function/function.ipp"

namespace analysis {

class exp_
: public function<exp_, 1> {

public:
  typedef function<exp_, 1> super;
  REGISTER_CLASS( exp_, super )

private:
  exp_(const expr &);
  exp_(const exp_ &);
 ~exp_();

  friend expr exp(const expr&);

  exp_* clone() const;

public:
  expr diff(const symbol &, unsigned) const;
};

} // namespace analysis


#endif
