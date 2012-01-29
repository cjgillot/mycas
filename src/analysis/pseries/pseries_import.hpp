#ifndef PSERIES_IMPORT_HPP
#define PSERIES_IMPORT_HPP

#include "analysis/expr.hpp"
#include "analysis/pseries.hpp"

#include "util/refcounted.hpp"

namespace analysis {

namespace pseries_detail {

struct closure_t {
  typedef void (*fun_t)(const expr &, void*);

  fun_t func;
  void* data;
};

class repr
: private util::noncopyable {

  void* m_value;

  MAKE_REFCOUNTED( repr );

private:
  explicit repr(void*);

public:
  repr();
  explicit repr(const expr &);
  ~repr();

public:
  void map_closure(pseries_detail::closure_t*);

public:
  static repr* neg(const repr &);
  static repr* inv(const repr &);

  static repr* add(const repr &, const repr &);
  static repr* sub(const repr &, const repr &);

  static repr* mul(const repr &, const repr &);
  static repr* div(const repr &, const repr &);
};

}} // namespace analysis::pseries_detail

#endif
