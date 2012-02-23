#ifndef ANALYSIS_FUNCTION_INDETS_HPP
#define ANALYSIS_FUNCTION_INDETS_HPP

#include "analysis/expr.hpp"
#include "analysis/symbol.hpp"

namespace analysis {

std::set<symbol> indets(const expr &);

} // namespace analysis

#endif
