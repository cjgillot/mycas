#ifndef ANALYSIS_STDFUNC_LOG_HPP
#define ANALYSIS_STDFUNC_LOG_HPP

#include "analysis/stdfunc.hpp"
#include "analysis/function/function.hpp"

namespace analysis {

class log_
: public function<log_, 1> {

public:
  typedef function<log_, 1> super;
  REGISTER_CLASS( log_, super )

private:
  explicit log_(const expr &);
  ~log_();

  // grant construction right
  friend expr log(const expr&);

  log_* clone() const;
  expr differentiate(const symbol &) const;
};

} // namespace analysis


#endif