/*
 * main.cxx
 *
 *  Created on: 21 mai 2011
 *      Author: k1000
 */

#include <iostream>

#include "algebra/print.hxx"

#include "analysis/expr.hxx"
#include "analysis/numeric.hxx"
#include "analysis/symbol.hxx"

#include "analysis/function/function.ixx"

using namespace analysis;

int main() {
  expr e (number(5));
  expr x (new ident("x"));
  expr y (new ident("y"));

  e.print(std::cout);
  std::cout << std::endl;

  for(int i = 1; i <= 5000; ++i) {
    expr f = number(i);

    for(int j = 0; j < 20; ++j) {
      expr s ( new symbol );
      f *= s;
    }

    e += f;
  }

  return 0;
}
