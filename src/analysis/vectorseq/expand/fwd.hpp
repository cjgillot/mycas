#ifndef EXPAND_FWD_HPP
#define EXPAND_FWD_HPP

#include "analysis/expr.hpp"

#include "util/move.hpp"
#include "util/assert.hpp"
#include "util/foreach.hpp"

#include "analysis/vectorseq.hpp"

#include "analysis/symbol.hpp"

#include "container/unsafe_vector.hpp"
#include "container/ptr_container.hpp"

namespace analysis {
namespace expand_detail {

template<class handle, class RRange, class ARange>
void expand_heap( RRange &, const ARange &, const ARange & );

ptr<const sum>  expand_sum_sum ( const sum  &, const sum & );
ptr<const sum>  expand_prod_sum( const expr &, const sum & );
ptr<const sum>  expand_sum_pow ( const sum  &, unsigned long );

expr   sum_expand( const sum   & );
expr  prod_expand( const prod  & );
expr power_expand( const power & );

}} // namespace analysis::expand_detail

#endif
