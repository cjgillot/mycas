/*
 * main.cxx
 *
 *  Created on: 21 mai 2011
 *      Author: k1000
 */

#include "algebra/integer.hxx"

#include "imperative/heap.hxx"
#include "polynomial/sparse/poly.hxx"

template class poly::sparse::poly<algebra::integer>;

int main() {
  poly::sparse::poly<algebra::integer> p;

  p *= algebra::integer(5);

  return 1;
}
