#include "analysis/expand/fwd.hpp"

namespace analysis {
namespace expand_detail {

namespace {

struct power_expander
: std::unary_function<const power*, ptr<const basic> >
{
  inline ptr<const basic> operator()( const power* ep ) const
  { return power_expand( *ep ); }
};

struct repower
: public std::unary_function<const expr&, expr>
{
  const expr* base;

  repower( const expr &b )
  : base( &b ) {}

  inline expr operator()( const expr &p ) const
  { return base->pow( p ).expand(); }
};

}

expr power_expand(const power &self)
{
  const expr &e_base = self.base().expand();
  const expr &e_expo = self.expo().expand();

  // (^ b (+ ec e1 e2 ...)) -> (^ b ec) * (^ b e1) * ...
  if( e_expo.is_a< sum >() )
  {
    const sum &s_expo = *e_expo.as_a< sum >();

    // (^ b ec)
    const expr &base_coef = e_base.pow( s_expo.coef() ).expand();

    // (^ b e1) * (^ b e2) * ...
    expr rest ( number::zero() );
    {
      container::unsafe_vector<expr> seq ( s_expo.size() );
      std::transform(
        s_expo.begin(), s_expo.end()
      , std::back_inserter( seq )
      , repower( e_base )
      );

      ptr< prod > restp =
        prod::from_expr_range( seq.begin(), seq.end() );

      // This expand is normally very cheap,
      // since all expressions in the range are
      // expanded powers (by repower).

      // However, it is useful when at least one of the
      // powers has been changed.
      rest = prod_expand( *restp );
    }

    // now, we shall build up base_coef * rest

    // asking for base_coef * rest does not work,
    // since the prod::mul behaviour is to sum exponents,
    // ie. back to square one

    if( rest.is_a< sum >() )
    {
      const sum &rest_sum = *rest.as_a< sum >();

      if( base_coef.is_a< sum >() )
        return expand_sum_sum( *base_coef.as_a< sum >(), rest_sum );

      return expand_prod_sum( base_coef, rest_sum );
    }

    if( base_coef.is_a< sum >() )
      return expand_prod_sum( rest, *base_coef.as_a< sum >() );

    const expr &ret = base_coef * rest;
    return ret.get()->basic::expand();
  }

  // treat case base^n
  if( e_expo.is_numeric() && e_base.is_a< sum >() )
  {
    const numeric &n_expo = e_expo.as_a< numeric >();

    if( !n_expo.is_integer() || !n_expo.fits_slong_p() )
      goto fini;

    const long &e = n_expo.get().get_slong();

    if( e == 0 )
      return number::one();

    if( e < 0 )
    {
      const unsigned long me = 1 + (unsigned long)~e;
      const ptr<const sum> ex = expand_sum_pow( e_base, me );
      const expr ret = self.pow(-1);
      ret.get()->basic::expand();
      return ret.get();
    }

    // e > 0 thanks to power::eval()
    {
      const ptr<const sum> ex = expand_sum_pow( e_base, e );
      const expr ret = self.pow(-1);
      ret.get()->basic::expand();
      return ret.get();
    }
  }

fini:
  self.basic::expand();
  return &self;
}

}} // namespace analysis::expand_detail
