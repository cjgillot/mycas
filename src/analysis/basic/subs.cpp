#include "analysis/expr.hpp"

#include "analysis/match_state.hpp"

#include "util/move.hpp"
#include "util/functor.hpp"

using namespace analysis;

expr basic::subs_once(const exmap &map) const
{
  exmap::const_iterator end = map.end()
      , it = map.find( this );

  if( it != end )
    return it->second.subs( map );

  match_state s;

  for( it =  map.begin(); it != end; ++it )
    if( match( it->first, s ) )
      return it->second.subs( s.as_exmap() );

  return this->eval(2);
}
