/*
 * main.cxx
 *
 *  Created on: 21 mai 2011
 *      Author: k1000
 */

#include <iostream>

#include "polynomial/sparse/poly.hxx"

int main() {
  double array[] = { 5.,4.,3.,2.,1. };

  typedef poly::sparse::poly<double> poly_t;

  poly_t p = poly_t::from_coefs(array);

  std::cout << p << std::endl;
  std::cout << p * 5 << std::endl;
  std::cout << p + p << std::endl;
  std::cout << p - p << std::endl;
  std::cout << p / 2 << std::endl;

  p *= p;

  std::cout << p << std::endl;

  return 1;
}
