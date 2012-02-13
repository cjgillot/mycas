#include "analysis/pseries/repr.hpp"

#include "analysis/caml/expr_wrap.hpp"

#include <caml.hpp>

using namespace analysis;

pseries::repr::repr()
: m_value( Val_int(0) )
{}

pseries::repr::repr( const expr &e )
{
  value ex = expr_copy( e );
  m_value = pseries_export_alone( ex );
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
