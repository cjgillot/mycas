#ifndef NUMBER_HPP
#define NUMBER_HPP

#include "analysis/expr.hpp"

#include "analysis/numeric.hpp"

#include "operators.hpp"

namespace analysis {

class number
: public expr
, public operators::printable<number
, operators::ordered<number
> > {

public:
  void swap(number &);

  number(const numeric*);
  number(double v);

public:
  inline bool null() const { return get()->numeric::null(); }
  inline bool unit() const { return get()->numeric::unit(); }

  inline const numeric*
  get() const
  { return static_cast<const numeric*>( expr::get() ); }

  static const number &zero()
  {
    static const number value ( numeric::zero() );
    return value;
  }
  static const number &one()
  {
    static const number value ( numeric::one() );
    return value;
  }

public:
  friend number operator+(const number &a) { return a; }
  friend number operator-(const number &a) { return a.neg(); }

#define OP( op, name )                  \
  friend number operator op             \
    (const number &a, const number &b)  \
  {                                     \
    return numeric::name(               \
      *a.get(), *b.get()                \
    );                                  \
  }

  OP( +, add )
  OP( -, sub )
  OP( *, mul )
  OP( /, div )

#undef OP

  number neg() const { return numeric::neg( *get() ); }
  number inv() const { return numeric::inv( *get() ); }

  number pow (const number &expo) const
  { return get()->numeric::pow( *expo.get() ); }

public:

#define CMPND_OP( op )                      \
  number &operator op##=( const number &o ) \
  { ( *this op o ).swap( *this ); return *this; }

  CMPND_OP( + )
  CMPND_OP( - )
  CMPND_OP( * )
  CMPND_OP( / )

#undef CMPND_OP

  number &ineg() { neg().swap( *this ); return *this; }
  number &iinv() { inv().swap( *this ); return *this; }

public:
  inline void
  print(std::basic_ostream<char> &os) const
  { get()->numeric::print(os); }

  static util::cmp_t
  compare(const number &, const number &);

  std::size_t hash() const
  { return get()->numeric::hash(); }
};


}

#endif
