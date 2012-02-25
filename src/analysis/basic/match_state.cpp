#include "match_state.hpp"

using namespace analysis;
using namespace match_detail;

using namespace boost::multi_index;

template class match_state_types<true>;

void
match_state::as_exmap(exmap &ret) const
{
  exmap::iterator it = ret.end();
  foreach( const pair_t &p, *this )
  {
    it = ret.insert( it,
      std::make_pair(
        wildcard( p.first, wildcard::unbound_tag() )
      , p.second
    ) );
  }
}
