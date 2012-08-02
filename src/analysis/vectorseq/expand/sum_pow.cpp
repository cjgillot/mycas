#include "analysis/vectorseq/expand/fwd.hpp"

#include "analysis/vectorseq/expand/heapmul.hpp"

#include "analysis/vectorseq/expand/sort.hpp"

namespace analysis {
namespace expand_detail {

// compute via repeated multiplication
static ptr<const sum>
rmul( const sum &a, unsigned long e )
{
  ptr<const sum> pow = &a;

  if( ! e )
    return sum::from_number( 1 );

  while( --e )
    pow = expand_sum_sum( a, *pow );

  return pow;
}

// compute via repeated squaring
static ptr<const sum>
rsqr( const sum &a, unsigned long e )
{
  ptr<const sum> pow = &a;

  if( ! e )
    return sum::from_number( 1 );

  for(; ! (e & 1); e >>= 1 )
    pow = expand_sum_sum( *pow, *pow );

  // [e] odd
  ptr<const sum> ret = pow;
  e >>= 1;

  for(; e ; e >>= 1)
  {
    pow = expand_sum_sum( *pow, *pow );

    if( e & 1 )
      ret = expand_sum_sum( *ret, *pow );
  }

  return ret;
}

// compute via binomial expansion
static ptr<const sum>
binb( const sum &a, unsigned long k )
{
  std::size_t len = a.size();

  sum::const_iterator
    beg = a.begin(), mid = beg + (len/2), end = a.end();

  ptr<const sum> htmp = sum::from_sorted_prod_range( a.coef(), beg, mid );
  ptr<const sum> ltmp = sum::from_sorted_prod_range( a.coef(), mid, end );

  container::ptr_unsafe_vector<const sum> hpow { k+1 }, lpow { k+1 };
  hpow.push_back( sum::from_number(1) ); hpow.push_back( htmp );
  lpow.push_back( sum::from_number(1) ); lpow.push_back( ltmp );

  for(std::size_t i = 2; i <= k; ++i)
  {
    hpow.push_back( expand_sum_sum( *hpow[i-1], *htmp ) );
    lpow.push_back( expand_sum_sum( *lpow[i-1], *ltmp ) );
  }

  container::ptr_unsafe_vector<const sum> mpow { k+1 };
  for(std::size_t i = 0; i <= k; ++i)
  {
    mpow.push_back( expand_sum_sum( *hpow[i], *lpow[k-i] ) );
  }

  hpow.clear(); lpow.clear();

  heap_nmerge<sum::const_iterator> heap { k + 1 };
  fmpz_class binc;
  for(std::size_t i = 0; i <= k; ++i)
  {
    fmpz_bin_uiui(binc.get_fmpz_t(), k, i);
    heap.add_one( prod::from_number(binc), mpow[i]->begin(), mpow[i]->end() );
  }

  typedef container::ptr_vector< const prod > pvector_t;
  pvector_t seq; seq.reserve( k * std::pow(len, k/3) );
  heap.main(seq);

  expand_detail::sort( seq.begin(), seq.end() );

  ptr< const sum > retp ( sum::from_sorted_prod_range( a.coef().pow(k), seq.begin(), seq.end() ) );
  retp->basic::expand();
  return retp;
}

// compute via term combinations exhaustion (multinomial expansion)
static ptr<const sum>
mne( const sum &a, unsigned long n )
{
  std::vector<expr> result;
  unsigned long m = a.size();

  {
    fmpz_t size; fmpz_init(size);
    fmpz_bin_uiui(size, n + m, m);
    result.reserve(fmpz_get_ui(size));
    fmpz_clear(size);
  }

  std::vector<long>
    k ( m, 0 )
  , k_cum ( m, 0 ) // k_cum[l] = \sum_{i=0}^l k[i]
  , upper_limit ( m, n );

  fmpz_class f, tmp;

  for(;;)
  {
    std::vector<expr> term;

    term.reserve(m+1);
    { // build up next term
      // sequence from exponentiation of prod's
      std::size_t l = 0;
      for(const prod* b : a)
      {
        term.push_back( b->pow(k[l]) );
        ++l;
      }

      // complementary exponentiation of coef
      term.push_back( a.coef().pow(n - k_cum[l-1]) );
    }

    { // scaling multinomial term
      fmpz_bin_uiui(f.get_fmpz_t(), n, k[0]);
      for(std::size_t l = 1; l < m; ++l)
      {
        fmpz_bin_uiui(tmp.get_fmpz_t(), n-k_cum[l-1], k[l]);
        f *= tmp;
      }

      term.push_back( number(f) );
    }

    result.push_back( prod::from_expr_range(term.begin(), term.end()) );

    // increment k[]
    {
      std::size_t l = m - 1;
      while( (++k[l]) > upper_limit[l] )
      {
        k[l] = 0;
        if(l != 0)
          --l;
        else
          goto fini;
      }

      // recalc k_cum[] and upper_limit[]
      k_cum[l] = (l==0 ? k[0] : k_cum[l-1]+k[l]);

      for(std::size_t i = l+1; i < m; ++i)
        k_cum[i] = k_cum[i-1] + k[i];

      for(std::size_t i = l+1; i < m; ++i)
        upper_limit[i] = n-k_cum[i-1];
    }
  }

fini:
  return sum::from_expr_range(result.begin(), result.end());
}

// binomial expansion
static ptr<const sum>
square( const sum &a )
{
  std::vector<expr> result;
  unsigned long m = a.size();

  result.reserve(m + m*m);

  result.push_back( a.coef().pow(2) );
  for(auto it1 = a.begin(); it1 != a.end(); ++it1)
  {
    result.push_back( (*it1)->pow(2).expand() );
    result.push_back( prod::mul_num(**it1, 2 * a.coef()) );

    for(auto it2 = it1 + 1; it2 != a.end(); ++it2)
      result.push_back( (2 * expr(prod::mul(**it1, **it2)).expand() ) );
  }

  return sum::from_expr_range(result.begin(), result.end());
}

ptr<const sum>
expand_sum_pow( const sum &a, unsigned long e )
{ return mne( a, e ); }

}} // namespace analysis::expand_detail
