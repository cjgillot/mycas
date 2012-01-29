#include "analysis/pseries/iterator.hpp"

#include <caml/callback.h>
#include <caml/mlvalues.h>
#include <caml/memory.h>

// #include "iterator.def"
extern "C" {
  value iterator_incr(value);
  value iterator_deref(value);
  value iterator_forced_p(value);
}

using namespace analysis;
using namespace analysis::pseries_detail;

iterator::iterator( void* p )
: m_rep( p )
{
  caml_register_global_root( (value*)&m_rep );
}

iterator::iterator( const iterator &o )
: m_rep( o.m_rep )
{
  caml_register_global_root( (value*)&m_rep );
}

iterator::~iterator()
{
  caml_remove_global_root( (value*)&m_rep );
}

iterator& iterator::operator++()
{
  m_rep = (void*)iterator_incr( (value)m_rep );
  return *this;
}

#define Expr_val( x ) ( reinterpret_cast< const expr* >( Data_custom_val( (x) ) ) )

const expr &iterator::operator*() const
{
  void* ret = (void*)iterator_deref( (value)m_rep );
  return *Expr_val( ret );
}

const expr *iterator::operator->() const
{
  void* ret = (void*)iterator_deref( (value)m_rep );
  return Expr_val( ret );
}

bool iterator::empty() const
{
  return Val_unit == (value)m_rep;
}

bool iterator::forced() const
{
  void* ret = (void*)iterator_forced_p( (value)m_rep );
  return Bool_val( (value)ret );
}
