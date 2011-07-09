/*
 * main.cxx
 *
 *  Created on: 21 mai 2011
 *      Author: k1000
 */

#include <iostream>

#include "analysis/expr.hxx"
#include "analysis/numeric.hxx"
#include "analysis/symbol.hxx"

using namespace analysis;

int main() {
  expr e (number(5));
  expr x (new ident("x"));
  expr y (new ident("y"));

  e.print(std::cout);
  std::cout << std::endl;

  for(int i = 0; i < 100; ++i) {
    for(int j = 0; j < 100; ++j) {
      expr s (new symbol);
      e *= s;
    }
    e *= e;
    e.ipow(y);
  }

  e.print(std::cout);
  std::cout << std::endl;

  return 0;
}
