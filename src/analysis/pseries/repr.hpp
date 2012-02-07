#ifndef ANALYSIS_PSERIES_REPR_HPP
#define ANALYSIS_PSERIES_REPR_HPP

#include <caml/mlvalues.h>

#include "analysis/pseries/pseries.hpp"

#include "util/refcounted.hpp"

namespace analysis {

class pseries::repr
: private util::noncopyable {

  value m_value;

  MAKE_REFCOUNTED( repr );

private:
  explicit repr(value);

public:
  repr();
  explicit repr(const expr &);
  ~repr();

  repr* clone() const
  { return new repr( m_value ); }

  friend class pseries;
};

} // namespace analysis

#include "analysis/pseries/pseries_export.h"

#endif
