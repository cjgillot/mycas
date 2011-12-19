#include "numeric.hpp"

#include "sum.hpp"
#include "prod.hpp"

using namespace analysis;

numeric::numeric(const numeric &o)
: basic(o), m_value(o.m_value) {}
numeric &numeric::operator=(const numeric &o) {
  m_value = o.m_value;
  return *this;
}
void numeric::swap(numeric &o) {
  std::swap(m_value, o.m_value);
}

numeric::numeric(double v)
: m_value(v) {}
numeric::~numeric() {}

numeric* numeric::clone() const
{ return new numeric(m_value); }

bool numeric::null() const
{ return algebra::null(m_value); }
bool numeric::unit() const
{ return algebra::unit(m_value); }

#define OP( op, name )                  \
  const numeric* numeric::name(         \
    const numeric &a, const numeric &b  \
  ) {                                   \
    return new numeric(                 \
      a.m_value op b.m_value            \
    );                                  \
  }

OP( +, add )
OP( -, sub )
OP( *, mul )
OP( /, div )

#undef OP

const numeric* numeric::neg(const numeric &a) {
  return new numeric( - a.m_value );
}

const numeric* numeric::inv(const numeric &a) {
  return new numeric( 1. / a.m_value );
}

expr
numeric::pow(const expr &expo) const {
  return expo.is_numeric()
  ? pow( *expo.as_a< numeric >() )
  : basic::pow( expo );
}
const numeric*
numeric::pow(const numeric &expo) const {
  if( expo.null() )
    return one();
  if( expo.unit() )
    return this;
  return new numeric( std::pow(m_value, expo.m_value) );
}

void numeric::print(std::basic_ostream<char> &os) const
{ os << m_value; }

bool numeric::is_numeric() const { return true; }

const sum*  numeric::as_sum () const { return sum::from_number(this); }
const prod* numeric::as_prod() const { return prod::from_number(this); }

util::cmp_t numeric::compare_same_type(const basic &o) const
{
  return algebra::compare(
    m_value
  , static_cast<const numeric&>(o).m_value
  );
}


bool numeric::has(const symbol&) const { return false; }
expr numeric::subs(const exmap &) const { return this; }

// ****** number ****** //
void number::swap(number &o)
{ expr::swap( o ); }


number::number(const numeric* n)
: expr(n) {}

number::number(double v)
: expr(new numeric(v)) {}

util::cmp_t
number::compare(const number &a, const number &b)
{
  util::cmp_t c = util::compare( a.hash(), b.hash() );
  if( c ) return c;
  return a.get()->numeric::compare_same_type( *b.get() );
}
