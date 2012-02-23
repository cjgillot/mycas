#include <caml.hpp>

#define EXPORT extern "C" CAMLprim

#include "expr_wrap.hpp"
#include "iter_wrap.hpp"

using namespace analysis;

namespace _caml_expr {

static void
iter_finalize(value it)
{
  Iter_val(it)->~iterator();
}

static int
iter_compare(value it1, value it2)
{
  return Iter_val(it1)->compare( *Iter_val(it2) );
}

static struct custom_operations iter_ops = {
  (char*)"analysis.expr",
  &iter_finalize,
  &iter_compare,
  custom_hash_default,
  custom_serialize_default,
  custom_deserialize_default,
  custom_compare_ext_default
};

value iter_allocate(std::size_t sz)
{
  // TODO maybe adapt the used/max values to the stored expr ?
  return caml_alloc_custom( &iter_ops, sz, 0, 1000 );
}

#define iter_construct( e ) \
  new( Data_custom_val( e ) )

} // namespace _caml_expr

EXPORT value __caml_iterator_copy(value it)
{
  CAMLparam1( it );
  CAMLlocal1( ret );

  ret = _caml_expr::iter_allocate( Iter_val(it)->size() );
  Iter_val(it)->clone( Iter_val(ret) );

  CAMLreturn( ret );
}

EXPORT value __caml_iterator_deref(value it)
{
  CAMLparam1( it );
  CAMLlocal1( ret );

  ret = Iter_val(it)->deref();

  CAMLreturn( ret );
}

EXPORT value __caml_iterator_advance(value it, value n)
{
  Iter_val( it )->advance( Int_val(n) );
  return Val_unit;
}
