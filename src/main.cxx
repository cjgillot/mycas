/*
 * main.cxx
 *
 *  Created on: 21 mai 2011
 *      Author: k1000
 */

#include <iostream>

#include "series/sparse/series.hxx"

int main() {
  double array[] = {
     5., 4., 3., 2., 1., 0.
  };

  typedef poly::sparse::poly<double> poly_t;

  poly_t p0 = poly_t::from_coefs(array);

//  std::cout << p0 << std::endl;

  typedef series::sparse::series<double> s_t;

  s_t p = s_t::from_poly(p0);

  const int n = 7;

  p.print_n(n, std::cout); std::cout << std::endl;
//  (p * 5).print_n(n, std::cout); std::cout << std::endl;
//  (p + p).print_n(n, std::cout); std::cout << std::endl;
//  (p - p).print_n(n, std::cout); std::cout << std::endl;
//  (p / 2).print_n(n, std::cout); std::cout << std::endl;
  (p * p).print_n(n, std::cout); std::cout << std::endl;
/*  std::cout << p * 5 << std::endl;
  std::cout << p + p << std::endl;
  std::cout << p - p << std::endl;
  std::cout << p / 2 << std::endl;

  p *= p;

  std::cout << p << std::endl;
  */
  return 0;
}

/*
int main() {
  typedef imperative::stream::stream<int> stream_t;

  stream_t s;

  ++s.begin(); *s.begin();

  stream_t m = imperative::stream::map(s, functor::identity<int>());

  *++m.begin();

  return 0;
}
*/
