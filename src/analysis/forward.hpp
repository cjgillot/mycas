#ifndef FORWARD_HPP_
#define FORWARD_HPP_

#include <vector>
#include <deque>
#include <stack>
#include <queue>
#include <list>
#include <map>
#include <set>

#include "numeric/number.hpp"

namespace analysis {
  class expr;

  using ::numeric::number;
}

namespace std {

template<> struct less< analysis::expr >;

}

namespace analysis {

#define DECLARE(klass)  \
  typedef std::klass<expr> ex##klass

DECLARE(multiset);
DECLARE(vector);
DECLARE(deque);
DECLARE(stack);
DECLARE(queue);
DECLARE(list);
DECLARE(set);

#undef DECLARE

typedef std::map<expr,expr> exmap;
typedef std::multimap<expr,expr> exmultimap;

class basic;

class sum;
class prod;
class power;
class numerical;

class expr;
class symbol;

}

namespace analysis {
namespace match_detail {

class match_state;

}

using match_detail::match_state;

}

#endif /* FORWARD_HPP_ */
