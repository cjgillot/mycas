#include <iostream>

#include "analysis/expr.hpp"
#include "analysis/symbol.hpp"
#include "analysis/vectorseq.hpp"

#include <cstdlib>

using namespace analysis;

inline long random(long max)
{ return std::rand() % max; }

int main()
{
  symbol x, y, z, t;

  expr f = expr::pow(1 + x + y + z + t, 60).expand();
  expr g = f + 1;

//   expr h = (f * g).expand();

  return 0;
}
