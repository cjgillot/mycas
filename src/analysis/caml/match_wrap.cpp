#include <caml.hpp>

#define EXPORT extern "C"

#include "analysis/expr.hpp"
#include "analysis/symbol.hpp"
#include "analysis/vectorseq.hpp"

#include "analysis/function/function.hpp"

#include "expr_wrap.hpp"
#include "iter_wrap.hpp"

using namespace analysis;
using vectorseq_detail::vectorseq_base;

namespace {

template<class It>
class adapt_iterator
: public _caml_expr::iterator
{
public:
  adapt_iterator(const It &i)
  : m_it( i ) {}

  ~adapt_iterator() {}

  std::size_t size()
  { return sizeof( adapt_iterator ); }
  void clone(void* dest)
  { new(dest) adapt_iterator( *this ); }

  void advance(std::size_t n)
  { std::advance( m_it, n ); }
  value deref()
  { return expr_copy( *m_it ); }

  util::cmp_t compare(iterator &o)
  {
    return std::distance( m_it, static_cast<adapt_iterator&>(o).m_it );
  }

private:
  It m_it;
};

typedef adapt_iterator<const expr*> function_iterator;

typedef adapt_iterator<const basic* const*> vseq_iterator;

}

EXPORT value __caml_expr_get_function(value ex)
{
  CAMLparam1(ex);
  CAMLlocal3(range_b, range_e, ret);

  const expr &e = *Expr_val( ex );
  const exprarray* a = e.as_a< exprarray >();

  const expr
    *it = a->pbegin(),
    *en = a->pend();

  range_b = _caml_expr::iter_allocate( sizeof(function_iterator) );
  new( Iter_val(range_b) ) function_iterator( it );

  range_e = _caml_expr::iter_allocate( sizeof(function_iterator) );
  new( Iter_val(range_e) ) function_iterator( en );

  ret = caml_alloc_small( 3, 0 );
  Field( ret, 0 ) = dynamic_cast<const function_base*>( a )->id().hash;
  Field( ret, 1 ) = range_b;
  Field( ret, 2 ) = range_e;

  CAMLreturn( ret );
}

EXPORT value __caml_expr_get_vseq(value ex)
{
  CAMLparam1(ex);
  CAMLlocal3(range_b, range_e, ret);

  const expr &e = *Expr_val( ex );
  const vectorseq_base* a = e.as_a< vectorseq_base >();

  range_b = _caml_expr::iter_allocate( sizeof(vseq_iterator) );
  new( Iter_val(range_b) ) vseq_iterator( a->pbegin() );

  range_e = _caml_expr::iter_allocate( sizeof(vseq_iterator) );
  new( Iter_val(range_e) ) vseq_iterator( a->pend() );

  ret = caml_alloc_small( 3, 0 );
  Field( ret, 0 ) = expr_copy( a->coef() );
  Field( ret, 1 ) = range_b;
  Field( ret, 2 ) = range_e;

  CAMLreturn( ret );
}
