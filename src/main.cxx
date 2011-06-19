/*
 * main.cxx
 *
 *  Created on: 21 mai 2011
 *      Author: k1000
 */

#include <iostream>

#include "series/dense/series.hxx"

typedef poly::dense::poly<double> poly_t;


int main() {
  double array1[] = {
     5., 4., 3., 2., 1.
  };

  double array2[] = {
     10., 20., 30., 40., 50., 60., 70., 80., 90., 100.
  };

  double array3[100000];
  {
    double val = 0.;
    for(unsigned i = 0; i < 100000; ++i)
      array3[i] = (val += 1.);
  }

  poly_t p01 = poly_t::from_coefs(array1);
  poly_t p02 = poly_t::from_coefs(array2);

//  std::cout << p0 << std::endl;

  typedef series::dense::series<double> s_t;
  //typedef poly_t s_t;

  s_t p1 = s_t::from_poly(p01);
  s_t p2 = s_t::from_poly(p02);
  //s_t p(p01);

  const int n = 20;

  //p1.print_n(n, std::cerr); std::cerr << std::endl;
  //p2.print_n(n, std::cerr); std::cerr << std::endl;
//  (p * 5).print_n(n, std::cout); std::cout << std::endl;
//  (p + p).print_n(n, std::cout); std::cout << std::endl;
//  (p - p).print_n(n, std::cout); std::cout << std::endl;
//  (p / 2).print_n(n, std::cout); std::cout << std::endl;
  //(p1 * p2).print_n(n, std::cerr); std::cerr << std::endl;

//  std::cout << p * 5 << std::endl;
//  std::cout << p + p << std::endl;
//  std::cout << p - p << std::endl;
//  std::cout << p / 2 << std::endl;
//
//  p *= p;
//
//  std::cout << p << std::endl;

  //std::cout << p << std::endl;
  //std::cerr << p01*p02 << std::endl;


  poly_t p03 = poly_t::from_coefs(array3);
  //s_t p3 = s_t::from_poly(p03);

  std::cerr << p03 << std::endl;

  std::cerr << p03 * p03 << std::endl;
  //(p3 * p3).print_n(200000, std::cerr); std::cerr << std::endl;

  return 0;
}
// */

