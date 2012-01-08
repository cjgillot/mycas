#include <iostream>

#include "algebra/print.hpp"

#include "analysis/expr.hpp"
#include "analysis/numerical.hpp"
#include "analysis/symbol.hpp"

#include "analysis/expr.ipp"

#include <cstdlib>

using namespace analysis;

inline long random(long max)
{ return std::rand() % max; }

int main()
{
  symbol x ("x");
  symbol y ("y");

  expr e = 1, f = 1;

//   std::cout << e << std::endl;
//   std::cout << f << std::endl;

  std::srand( std::time( nullptr ) );

  for( std::size_t n = random(100); n != 0; --n )
    e += random(10) * x.pow(n * random(3));

  for( std::size_t n = random(100); n != 0; --n )
    f += random(10) * y.pow(n * random(3));

  std::cout << e << std::endl;
  std::cout << f << std::endl;

  expr ef = (e*f).expand();

  std::cout << ef << std::endl;

  return 0;
}
