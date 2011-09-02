#ifndef ANALYSIS_STDFUNC_LOG_HPP
#define ANALYSIS_STDFUNC_LOG_HPP

#include "analysis/stdfunc.hpp"
#include "analysis/function/function.ipp"

namespace analysis {

class log_
: public function<log_, 1> {

public:
  typedef function<log_, 1> super;
  REGISTER_CLASS( log_, super )

private:
  log_(const expr &);
  log_(const log_ &);
 ~log_();

  friend expr log(const expr&);

  log_* clone() const;

public:
  expr diff(const symbol &, unsigned n) const;
};

} // namespace analysis


#endif
