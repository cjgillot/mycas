#ifndef EXPAND_FWD_HPP
#define EXPAND_FWD_HPP

#include "analysis/expr.hpp"
#include "analysis/basic.hpp"

#include "analysis/expr.ipp"
#include "analysis/basic.ipp"

#include "util/move.hpp"
#include "util/assert.hpp"
#include "util/foreach.hpp"

#include "analysis/power.hpp"
#include "analysis/prod.hpp"
#include "analysis/sum.hpp"

#include "analysis/symbol.hpp"

#include "analysis/ptr.hpp"

#include "container/unsafe_vector.hpp"
#include "container/ptr_container.hpp"

namespace analysis {
namespace expand_detail {

template<class handle, class RRangen class ARange>
void expand_heap(RRange &, const ARange &, const ARange &);

ptr<const sum>  expand_sum_sum ( const sum  &, const sum & );
ptr<const sum>  expand_prod_sum( const expr &, const sum & );
ptr<const sum>  expand_sum_pow ( const sum  &, unsigned long );

ptr<const basic>   sum_expand( const sum   & );
ptr<const basic>  prod_expand( const prod  & );
ptr<const basic> power_expand( const power & );

}} // namespace analysis::expand_detail

#endif
