#ifndef NUMERIC_POW_HPP
#define NUMERIC_POW_HPP

#include <gmpfrxx.h>

#include "numeric/real.hpp"

namespace numeric {

struct bad_pow
: public std::exception {

  inline bad_pow() throw() {}
  inline ~bad_pow() throw() {}

  const char* what() const throw()
  { return "Power computing failure\n"; }

};

rational pow(const integer  &b, const integer &e) throw(bad_pow);
rational pow(const rational &b, const integer &e) throw(bad_pow);

rational pow(const integer  &b, const rational &e) throw(bad_pow);
rational pow(const rational &b, const rational &e) throw(bad_pow);
real     pow(const real     &b, const rational &e) throw(bad_pow);

}

#endif
