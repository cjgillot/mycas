#include "analysis/pseries/iterator.hpp"

#include "analysis/pseries/repr.hpp"

#include "analysis/caml/expr_wrap.hpp"

#include <caml.hpp>

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

#define rep (*(value*)&m_rep)

iterator::iterator(const pseries::repr* r)
: m_rep( (void*)r->m_value )
{
  caml_register_global_root( &rep );
}

iterator::iterator( const iterator &o )
: m_rep( o.m_rep )
{
  caml_register_global_root( &rep );
}

iterator::~iterator()
{
  caml_remove_global_root( &rep );
}

void iterator::increment()
{ rep = incr( rep ); }

const expr &iterator::dereference() const
{
  value ret = deref( rep );
  return *Expr_val( ret );
}

bool iterator::empty() const
{
  return ::empty( rep );
}

bool iterator::forced() const
{
  value ret = pseries_export_iterator_forced( rep );
  return Bool_val( ret );
}
