#include "numeric/number.hpp"

using numeric::number;
using numeric::detail::impl_t;
using numeric::detail::wrapper;

namespace {

struct null_visitor
: public boost::static_visitor<bool>
{
  bool operator()(const wrapper<fmpz_t> &c) const
  { return fmpz_is_zero( c->get_fmpz_t() ); }
  bool operator()(const wrapper<mpfr_t> &c) const
  { return *c == 0; }

  // integer shall not be in an fmpq_t
  bool operator()(const wrapper<fmpq_t> &c) const
  { ASSERT( ! fmpq_is_zero( c->get_fmpq_t() ) ); return false; }
};

}

bool number::null() const
{ return boost::apply_visitor( null_visitor(), m_impl ); }

namespace {

struct unit_visitor
: public boost::static_visitor<bool>
{
  bool operator()(const wrapper<fmpz_t> &c) const
  { return fmpz_is_one( c->get_fmpz_t() ); }
  bool operator()(const wrapper<mpfr_t> &c) const
  { return *c == 1; }

  // integer shall not be in an fmpq_t
  bool operator()(const wrapper<fmpq_t> &c) const
  { ASSERT( ! fmpq_is_one( c->get_fmpq_t() ) ); return false; }
};

}

bool number::unit() const
{ return boost::apply_visitor( unit_visitor(), m_impl ); }

namespace {

struct ineg_visitor
: public boost::static_visitor<>
{
  template<class T>
  void operator()(T &c) const
  { *c = - *c; }
};

}

number &number::ineg()
{
  boost::apply_visitor( ineg_visitor(), m_impl );
  return *this;
}

namespace {

struct inv_visitor
: public boost::static_visitor<>
{
  mutable impl_t *impl;

  explicit inv_visitor(impl_t &i)
  : impl( &i ) {}

  void operator()(const wrapper<fmpz_t> &c) const
  { *impl = fmpq_class( 1, *c ); }
  void operator()(const wrapper<fmpq_t> &c) const
  {
    if( fmpz_is_pm1( c->get_num_fmpz_t() ) )
      *impl = c->get_den() * c->get_num();
    else {
      *impl = fmpq_class();
      fmpq* p = boost::get< wrapper<fmpq_t> >( *impl )->get_fmpq_t();
      fmpq_inv( p, c->get_fmpq_t() );
    }
  }
  void operator()(const wrapper<mpfr_t> &c) const
  { *impl = 1 / *c; }
};

}

number number::inv() const
{
  number ret;

  boost::apply_visitor( inv_visitor(ret.m_impl), m_impl );

  return ret;
}

namespace {

struct print_visitor
: public boost::static_visitor<>
{
  std::ostream *os;

  explicit print_visitor(std::ostream &s)
  : os( &s ) {}

  template<class T>
  void operator()(const T &c) const
  { *os << *c; }
};

}

void number::print(std::ostream &os) const
{ boost::apply_visitor( print_visitor(os), m_impl ); }
