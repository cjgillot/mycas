#ifndef ANALYSIS_MATCH_STATE_IPP
#define ANALYSIS_MATCH_STATE_IPP

#include "match_state.hpp"

namespace analysis {
namespace match_detail {

inline
match_state::match_state()
: stack( this->get<1>() ) {}

inline
match_state::~match_state() {}

inline void
match_state::push_state(unsigned id, const expr &e)
{ stack.push_front( std::make_pair( id, e ) ); }

inline void
match_state::pop_state(size_t sz) const
{
  ASSERT( stack.size() >= sz );
  for( size_t i = stack.size() - sz; i != 0; --i )
    stack.pop_front();
}

inline std::size_t
match_state::size_state() const
{ return stack.size(); }

inline const expr&
match_state::at(unsigned id) const
{
  const_iterator it = find( id ), en = end();
  if( it == en )
    throw std::out_of_range("match_state::at() [const]");
  return it->second;
}
inline expr&
match_state::at(unsigned id)
{
  iterator it = find( id ), en = end();
  if( it == en )
    throw std::out_of_range("match_state::at()");
  return const_cast<expr&>( it->second );
}

inline const expr&
match_state::operator[](unsigned id) const
{ return find( id )->second; }
inline expr&
match_state::operator[](unsigned id)
{ return const_cast<expr&>( find( id )->second ); }

}} // namespace analysis::match_detail

#endif
