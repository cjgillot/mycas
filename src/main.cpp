/*
 * main.cpp
 *
 *  Created on: 21 mai 2011
 *      Author: k1000
 */

#include <iostream>

#include "algebra/print.hpp"

#include "analysis/expr.hpp"
#include "analysis/numeric.hpp"
#include "analysis/symbol.hpp"

#include "analysis/function/function.ipp"

using namespace analysis;

int main() {
  expr e (number(5));
  expr x (new ident("x"));
  expr y (new ident("y"));

  //e.print(std::cout);
  //std::cout << std::endl;

  for(int i = 1; i <= 500; ++i) {
    //std::cerr << i << std::endl;

    expr f = number(i);

    for(int j = 0; j < 200; ++j) {
      expr s ( new symbol );
      f *= s;
    }

    e += f;
  }

  //std::cout << e << std::endl;

  return 0;
}
