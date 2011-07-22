#include "real.hpp"
#include "real/real_t.hpp"

#include "real/operation.hpp"

#if 0

namespace numeric {


real real::zero ( new real_t( 0 ) );
real real::one  ( new real_t( 1 ) );

bool real::null() const
{ return m_repr->null(); }

bool real::unit() const
{ return m_repr->unit(); }

void real::print(std::basic_ostream<char> &os) const
{ m_repr->print(os); }

int real::compare(const real &a, const real &b)
{ return real_t::compare(*a.m_repr, *b.m_repr); }

std::size_t
real::get_hash() const
{ return m_repr->hash(); }

#define OP_NAME( op ) BOOST_PP_CAT(__op_, __LINE__)

#define CMPND_BOP( op )                     \
  DECL_BOP( OP_NAME( op ), op );            \
  real &                                    \
  real::operator op##=(const real &o)       \
  {                                         \
    m_repr = real_t::bin_op<OP_NAME( op )>  \
      ( *m_repr, *o.m_repr );               \
    return *this;                           \
  }


#define CMPND_UOP( op, fnc )                \
  DECL_UOP( OP_NAME( op ), fnc );           \
  real &real::op()                          \
  {                                         \
    m_repr = real_t::una_op<OP_NAME( op )>  \
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

#endif
