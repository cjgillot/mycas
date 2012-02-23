#ifndef ANALYSIS_STDFUNC_LOG_HPP
#define ANALYSIS_STDFUNC_LOG_HPP

#include "analysis/stdfunc.hpp"
#include "analysis/function/function.hpp"

namespace analysis {

class log_
: public function<1>
{
  static func_id s_id;

public:
  typedef function<1> super;
  REGISTER_CLASS( log_, super )

public:
  expr eval(unsigned) const;

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
