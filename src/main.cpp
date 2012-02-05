#include <iostream>

#include "algebra/print.hpp"

#include "analysis/expr.hpp"
#include "analysis/numerical.hpp"
#include "analysis/symbol.hpp"

#include "analysis/expr.ipp"

#include <cstdlib>

#include "analysis/pseries.hpp"

using namespace analysis;

inline long random(long max)
{ return std::rand() % max; }

#include <caml/mlvalues.h>

int main()
{
  symbol x ("x");
  symbol y ("y");

  expr e = 1, f = 1;

//   std::cout << e << std::endl;
//   std::cout << f << std::endl;

  expr s = new pseries( x );

//   caml_startup(0);

  return 0;
}
