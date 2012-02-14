#ifndef ANALYSIS_STDFUNC_EXP_HPP
#define ANALYSIS_STDFUNC_EXP_HPP

#include "analysis/stdfunc.hpp"
#include "analysis/function/function.hpp"

namespace analysis {

class exp_
: public function<1>
{
  static symbol m_name;

public:
  typedef function<1> super;
  REGISTER_CLASS( exp_, super )

public:
  expr eval(unsigned) const;

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
