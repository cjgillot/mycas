#include "algebra/real.hpp"

namespace algebra {

namespace {

union flong {
  long i64;
  double d64;
};

inline long
getZerom() {
  return 1l << 63;
}

inline long
getUlps() {
  return 5;
}

inline long
getNaN() {
  flong nan;
  nan.d64 = std::numeric_limits<real>::quiet_NaN();
  // strip sign bit
  //nan.i64 &= ~getZerom();
  return nan.i64;
}

}

// http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm
int compare(real a, real b) {
  assert(sizeof(real) <= sizeof(long));

  static const long zerom = getZerom();

  flong aInt, bInt;
  aInt.d64 = a; bInt.d64 = b;

  // Make bInt lexicographically ordered as a twos-complement int
  if (aInt.i64 < 0) aInt.i64 = zerom - aInt.i64;
  if (bInt.i64 < 0) bInt.i64 = zerom - bInt.i64;

  static const long maxUlps = getUlps();

  // Make sure maxUlps is non-negative and small enough that the
  // default NAN won't compare as equal to anything.
  assert(maxUlps > 0 && maxUlps < getNaN());

  const long intDiff = aInt.i64 - bInt.i64;
  if (std::abs(intDiff) <= maxUlps)
      return 0;

  static const int not_sgn = ~(1 << 31);

  // {ret} must have the same sign than {intDiff} (first line)
  // and must be non-null iff {intDiff} is
  int ret = intDiff >> 32;
  ret |= ( intDiff >> 1 ) & not_sgn;
  ret |= intDiff & 1;
  return ret;
}

}
