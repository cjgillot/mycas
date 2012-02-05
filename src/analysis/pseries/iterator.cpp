#include "analysis/pseries/iterator.hpp"

#include "analysis/pseries/pseries_export.h"

#include "analysis/caml/expr_wrap.hpp"

#include <caml/callback.h>
#include <caml/mlvalues.h>
#include <caml/memory.h>

using namespace analysis;
using namespace analysis::pseries_detail;

static inline value empty(value it)
{
  it = pseries_export_iterator_force( it );
  return Int_val( it ) == 0;
}

static inline value deref(value it)
{
  it = pseries_export_iterator_force( it );
  return Field( it, 0 );
}

static inline value incr(value it)
{
  it = pseries_export_iterator_force( it );
  return Field( it, 1 );
}


iterator::iterator( value p )
: m_rep( p )
{
  caml_register_global_root( &m_rep );
}

iterator::iterator( const iterator &o )
: m_rep( o.m_rep )
{
  caml_register_global_root( &m_rep );
}

iterator::~iterator()
{
  caml_remove_global_root( &m_rep );
}

iterator& iterator::operator++()
{
  m_rep = incr( m_rep );
  return *this;
}

const expr &iterator::operator*() const
{
  value ret = deref( m_rep );
  return *Expr_val( ret );
}

const expr *iterator::operator->() const
{
  value ret = deref( m_rep );
  return  Expr_val( ret );
}

bool iterator::empty() const
{
  return ::empty( m_rep );
}

bool iterator::forced() const
{
  value ret = pseries_export_iterator_forced( m_rep );
  return Bool_val( ret );
}
