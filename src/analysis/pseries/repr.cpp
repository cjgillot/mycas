#include "analysis/pseries/repr.hpp"

#include "analysis/caml/expr_wrap.hpp"
#include <caml/memory.h>

using namespace analysis;

pseries::repr::repr()
: m_value( pseries_export_empty() )
{}

pseries::repr::repr( const expr &e )
{
  value ex = expr_copy( e );
  m_value = pseries_export_make( ex );
  caml_register_global_root( (value*)&m_value );
}

pseries::repr::~repr()
{
  caml_remove_global_root( (value*)&m_value );
}

pseries::repr::repr( value val )
: m_value( val )
{
  caml_register_global_root( (value*)&m_value );
}
