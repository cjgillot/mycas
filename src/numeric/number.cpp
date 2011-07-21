#include "number.hpp"

#include "util/refcounted.hpp"

#include<gmpfrxx.h>

#include "repr.hpp"

#include "compare.hpp"

#include "pow.hpp"
#include "operation.hpp"

namespace numeric {


number number::zero ( new repr_t( 0 ) );
number number::one  ( new repr_t( 1 ) );

bool number::null() const
{ return m_repr->null(); }

bool number::unit() const
{ return m_repr->unit(); }

void number::print(std::basic_ostream<char> &os) const
{ m_repr->print(os); }

int number::compare(const number &a, const number &b)
{ return repr_t::compare(*a.m_repr, *b.m_repr); }

std::size_t
number::get_hash() const
{ return m_repr->hash(); }

#define OP_NAME( op ) BOOST_PP_CAT(__op_, __LINE__)

#define CMPND_BOP( op )                      \
  DECL_BOP( OP_NAME( op ), op );            \
  number &                                  \
  number::operator op##=(const number &o)   \
  {                                         \
    m_repr = repr_t::bin_op<OP_NAME( op )>  \
      ( *m_repr, *o.m_repr );               \
    return *this;                           \
  }


#define CMPND_UOP( op, fnc )                \
  DECL_UOP( OP_NAME( op ), fnc );           \
  number &number::op()                      \
  {                                         \
    m_repr = repr_t::una_op<OP_NAME( op )>  \
      ( *m_repr );                          \
    return *this;                           \
  }


CMPND_BOP( + )
CMPND_BOP( - )
CMPND_BOP( * )
CMPND_BOP( / )
// CMPND_OP( % )

CMPND_UOP( operator++,  1+ )
CMPND_UOP( operator--, -1+ )

CMPND_UOP( ineg, - )
CMPND_UOP( iinv, 1/)

} // namespace numeric

#undef DECL_UOP
#undef DECL_BOP
