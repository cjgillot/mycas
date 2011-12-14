#ifndef ANALYSIS_MATCH_STATE_HPP
#define ANALYSIS_MATCH_STATE_HPP

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>

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
  match_state_types::container::nth_index<1>::type &stack;

public:
   match_state() : stack( this->get<1>() ) {}
  ~match_state() {}

public:
  void push_state(unsigned id, const expr &e)
  { stack.push_front( std::make_pair( id, e ) ); }

  unsigned top_state() const { return stack.front().first; }

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
};

}}

#endif
