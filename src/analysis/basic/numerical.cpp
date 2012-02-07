#include "analysis/expr.hpp"

#include "analysis/vectorseq.hpp"

#include "numeric/number.hpp"

#include "algebra/real.hpp"

using namespace analysis;

numerical::numerical(const numerical &o)
: basic(o), m_value(o.m_value) {}

// numerical::~numerical() {}

numerical* numerical::clone() const
{ return new numerical(m_value); }

bool numerical::null() const
{ return algebra::null(m_value); }
bool numerical::unit() const
{ return algebra::unit(m_value); }

#define OP( op, name )                  \
  const numerical* numerical::name(         \
    const numerical &a, const numerical &b  \
  ) {                                   \
    return new numerical(                 \
      a.m_value op b.m_value            \
    );                                  \
  }

OP( +, add )
OP( -, sub )
OP( *, mul )
OP( /, div )

#undef OP

const numerical* numerical::neg(const numerical &a)
{ return new numerical( a.m_value.neg() ); }

const numerical* numerical::inv(const numerical &a)
{ return new numerical( a.m_value.inv() ); }

expr
numerical::pow(const expr &expo) const {
  return expo.is_numerical()
  ? pow( *expo.as_a< numerical >() )
  : basic::pow( expo );
}
const numerical*
numerical::pow(const numerical &expo) const {
  if( expo.null() )
    return one();
  if( expo.unit() )
    return this;
  return new numerical( m_value.pow( expo.m_value ) );
}

void numerical::print(std::basic_ostream<char> &os) const
{ os << m_value; }

bool numerical::is_numerical() const { return true; }

const sum*  numerical::as_sum () const { return  sum::from_number( this->get() ); }
const prod* numerical::as_prod() const { return prod::from_number( this->get() ); }

util::cmp_t numerical::compare_same_type(const basic &o_) const
{
  const numerical &o = static_cast<const numerical&>(o_);
  return number::compare( m_value, o.m_value );
}


bool numerical::has(const symbol&) const { return false; }
expr numerical::subs(const exmap &) const { return this; }
