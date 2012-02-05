#ifndef ANALYSIS_PSERIES_CLOSURE_HPP
#define ANALYSIS_PSERIES_CLOSURE_HPP

#include "analysis/expr.hpp"

namespace analysis {
namespace pseries_detail {

struct iter_closure_t {
  typedef void (*fun_t)(const expr&, void*);

  fun_t func;
  void* data;
};

struct map_closure_t {
  typedef expr (*fun_t)(const expr&, void*);

  fun_t func;
  void* data;
};

}} // namespace analysis::pseries_detail

#endif
