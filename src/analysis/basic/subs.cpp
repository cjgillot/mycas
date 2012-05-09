#include "analysis/expr.hpp"

#include "analysis/wildcard.hpp"

using namespace analysis;

expr basic::subs_once(const exmap &map) const
{
  auto end = map.cend();
  auto it  = map.find( this );

  if( it != end )
    return it->second.subs( map );

  match_state s;

  for( it =  map.begin(); it != end; ++it )
    if( match( it->first, s ) )
    {
      exmap m;
      s.as_exmap( m );
      return it->second.subs( m );
    }

  return this->eval(2);
}
