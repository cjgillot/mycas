#include "analysis/match.hpp"

using namespace analysis;

typedef basic::match_stack match_stack;

bool matched_wild_p( const wildcard_ &pattern, match_stack &mm )
{
  match_stack::const_iterator it = mm.begin(), en = mm.end();
  unsigned pid = pattern.id();

  while( it != en )
  {
    if( it->first == pid )
      return true;
    ++it;
  }

  return false;
}

bool match_wild( const basic &self, const wildcard_ &pattern, match_stack &mm )
{
  match_stack::const_iterator it = mm.begin(), en = mm.end();
  unsigned pid = pattern.id();

  while( it != en )
  {
    if( it->first == pid )
      // already matched somewhere
      return expr::compare( &self, it->second ) == 0;

    ++it;
  }

  mm.push_back( std::make_pair( pid, &self ) );
  return true;
}

bool basic::match(const expr &e, match_map &mm) const
{
  if( e.is_a< wildcard >() )
    return match_wild( *this, *e.as_a< wildcard_ >(), mm );

  if( RTTI_ID( this ) != RTTI_ID( e.get() ) )
    return false;

  return match_same_type( *e.get(), mm );
}

bool basic::match_same_type(const basic &o, match_map &) const
{
  return compare( *this, o ) == 0;
}

// sum
#include "analysis/sum.hpp"
#include "analysis/prod.hpp"
#include "analysis/power.hpp"

#include "container/unsafe_vector.hpp"

template<class I, class M, class F>
bool match_same_type_eps(const I &self, const I &pattern, match_map &mm, F generator) const
{
  if( &self == &pattern )
    return true;

  bool has_global_wild = false;
  ptr< wildcard_ > global_wild;

  std::list< expr > items ( self.begin(), self.end() );
  items.push_front( self.coef() );

  foreach( const expr &pit, pattern )
  {
    if( ! has_global_wild && pit->is_a< wildcard_ >() )
    {
      const wildcard_ &w = *pit.as_a< wildcard_ >();

      if( ! matched_wild_p( w, mm ) )
      {
        has_global_wild = true;
        global_wild.reset( pit->as_a< wildcard_ >() );
        continue;
      }
    }

    foreach( const expr &sit, items )
    {
      if( ! sit.match( pit, mm ) )
        return false;

      items.erase( it );
      goto next_p;
    }

  next_p:
    ;
  }

  if( ! has_global_wild )
    return items.empty();


}

bool prod::match_same_type(const basic &o_, match_map &mm) const
{
  return basic::match_same_type( o_, mm );
}

bool power::match_same_type(const basic &o_, match_map &mm) const
{
  return basic::match_same_type( o_, mm );
}
