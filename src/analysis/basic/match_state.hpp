#ifndef ANALYSIS_MATCH_STATE_HPP
#define ANALYSIS_MATCH_STATE_HPP

#include "wildcard.hpp"

#include "util/foreach.hpp"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>
#include <utility>

namespace analysis {
namespace match_detail {

using namespace boost::multi_index;

template<bool Dummy>
struct match_state_types;

template<>
struct match_state_types<true>
{
  typedef std::pair<unsigned, expr> pair_t;

  typedef boost::multi_index::multi_index_container<
    pair_t
  , boost::multi_index::indexed_by<
      boost::multi_index::ordered_unique<
        boost::multi_index::member<pair_t, unsigned, &pair_t::first>
      >
    , boost::multi_index::sequenced<> // stack index
    >
  > container;
};

class match_state
: public match_state_types<true>::container
{
  typedef match_state_types<true>::pair_t pair_t;
  typedef match_state_types<true>::container container;
  container::nth_index<1>::type &stack;

public:
   match_state();
  ~match_state();

public:
  void push_state(unsigned, const expr&);
  void pop_state(size_t) const;

  std::size_t size_state() const;

public:
  // inherited from container

  const expr &at(unsigned) const;
  expr &at(unsigned);

  const expr &operator[](unsigned) const;
  expr &operator[](unsigned);

public:
  void as_exmap(exmap&) const;
};

} // namespace match_detail

using match_detail::match_state;

} // namespace analysis

#endif

#include "match_state.ipp"
