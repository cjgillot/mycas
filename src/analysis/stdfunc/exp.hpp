#ifndef ANALYSIS_STDFUNC_EXP_HPP
#define ANALYSIS_STDFUNC_EXP_HPP

#include "analysis/stdfunc.hpp"
#include "analysis/function/function.hpp"

namespace analysis {

class exp_
: public function<exp_, 1> {

public:
  typedef function<exp_, 1> super;
  REGISTER_CLASS( exp_, super )

private:
  explicit exp_(const expr &);
  ~exp_();

  // grant construction right
  friend expr exp(const expr&);

  exp_* clone() const;
  expr differentiate(const symbol &) const;
};

} // namespace analysis


#endif
