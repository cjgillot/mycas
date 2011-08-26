#ifndef ANALYSIS_STDFUNC_LOG_HPP
#define ANALYSIS_STDFUNC_LOG_HPP

#include "analysis/stdfunc.hpp"
#include "analysis/function/function.ipp"

namespace analysis {

class log_
: public function<1> {

  REGISTER_CLASS( log_, function<1> )

  typedef function<1> super;

private:
  log_(const expr &);
  log_(const log_ &);
 ~log_();

  friend expr log(const expr&);

  log_* clone() const;

public:
  expr diff(const symbol &, unsigned n = 1) const;
  expr eval(unsigned) const;
};

} // namespace analysis


#endif
