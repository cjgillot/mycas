#include "analysis/expr.hpp"

#include "analysis/vectorseq.hpp"
#include "analysis/vectorseq/expand/fwd.hpp"

using namespace analysis;
using namespace expand_detail;

expr   sum::expand() const { return   sum_expand( *this ); }
expr  prod::expand() const { return  prod_expand( *this ); }
expr power::expand() const { return power_expand( *this ); }
