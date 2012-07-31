#ifndef ANALYSIS_PSERIES_REPR_HPP
#define ANALYSIS_PSERIES_REPR_HPP

#include "analysis/pseries/pseries.hpp"

#include "util/refcounted.hpp"

#include <caml.hpp>

namespace analysis {

class pseries::repr
: public analysis::memory_base<repr>
, private util::noncopyable {

  value m_value;

  MAKE_REFCOUNTED( repr );

public:
  repr();
  explicit repr(value);
  explicit repr(const expr &);
  ~repr();

  repr* clone() const
  { return new repr( m_value ); }

  friend class pseries;
  friend class pseries_detail::iterator;
};

} // namespace analysis

#include "analysis/pseries/export.h"

#endif
