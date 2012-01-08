#ifndef ANALYSIS_MATCH_STATE_HPP
#define ANALYSIS_MATCH_STATE_HPP

#include "analysis/match.hpp"

#include "util/foreach.hpp"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>
#include <utility>

namespace analysis {
namespace match_detail {

using namespace boost::multi_index;

struct match_state_types {
  typedef std::pair<unsigned, expr> pair_t;

  typedef multi_index_container<
    pair_t
  , indexed_by<
      ordered_unique<
        member<pair_t, unsigned, &pair_t::first>
      >
    , sequenced<> // stack index
    >
  > container;
};

class match_state
: public match_state_types::container
{
  typedef match_state_types::pair_t pair_t;
  match_state_types::container::nth_index<1>::type &stack;

public:
   match_state() : stack( this->get<1>() ) {}
  ~match_state() {}

public:
  void push_state(unsigned id, const expr &e)
  { stack.push_front( std::make_pair( id, e ) ); }

  void pop_state(size_t sz) const
  {
    assert( stack.size() >= sz );
    for( size_t i = stack.size() - sz; i != 0; --i )
      stack.pop_front();
  }

  size_t size_state() const
  { return stack.size(); }

public:
  // inherited from container

  const expr &at(unsigned id) const
  {
    const_iterator it = find( id ), en = end();
    if( it == en )
      throw std::out_of_range("match_state::at()");
    return it->second;
  }
  expr &at(unsigned id)
  {
    iterator it = find( id ), en = end();
    if( it == en )
      throw std::out_of_range("match_state::at() [const]");
    return const_cast<expr&>( it->second );
  }

  const expr &operator[](unsigned id) const
  { return find( id )->second; }
  expr &operator[](unsigned id)
  { return const_cast<expr&>( find( id )->second ); }

  exmap as_exmap() const
  {
    exmap ret;
    exmap::iterator it = ret.end();
    foreach( const pair_t &p, *this )
    {
      it = ret.insert( it,
        std::make_pair(
          wildcard( p.first, wildcard::unbound_tag() )
        , p.second
      ) );
    }
    return ret;
  }
};

}}

#endif
