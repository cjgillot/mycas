#include "analysis/pseries.hpp"

#include "analysis/expr.hpp"
#include "analysis/expr.ipp"

#include "pseries/pseries_import.hpp"

using namespace analysis;
using pseries_detail::repr;

pseries::pseries(const symbol& var)
: basic(), m_var( var ), m_rep( new repr )
{}

pseries::pseries(const pseries &o)
: basic(o), m_var( o.m_var ), m_rep( o.m_rep )
{}

pseries::~pseries()
{}

// ----- eval ----- //
void eval_closure( const expr &ex, void* data )
{
  ex.eval( (unsigned long)data );
}

expr pseries::eval(unsigned lv) const
{
  if( lv == 0 )
    return this;

  pseries_detail::closure_t closure =
    { &eval_closure, 0 };

  closure.data = (void*)(unsigned long)lv;

  m_rep->map_closure( &closure );

  basic::eval(lv);
  return this;
}

// ----- hash ----- //
void hash_closure( const expr &ex, void* data )
{
  boost::hash_combine( *(std::size_t*)data, ex.hash() );
}

std::size_t pseries::hash() const
{
  std::size_t seed = m_var.hash();

  pseries_detail::closure_t closure =
    { &hash_closure, &seed };

  m_rep->map_closure( &closure );

  return seed;
}

// ----- has ----- //
void has_closure( const expr &ex, void* data )
{
  if( *(void**)data )
    if( ex.has( **(const symbol**)data ) )
      *(void**)data = nullptr;
}

bool pseries::has(const symbol &s) const
{
  if( m_var.has( s ) )
    return true;

  const symbol* sym = &s;

  pseries_detail::closure_t closure =
    { &has_closure, &sym };

  m_rep->map_closure( &closure );

  return sym;
}

// ----- print ----- //
struct print_data {
  std::ostream* os;
  std::size_t index;
  const symbol* var;
};

void print_closure( const expr &ex, void* data )
{
  print_data* d = (print_data*)data;

  std::size_t n = d->index;

  if( n != 0 )
    *d->os << ' ';

  *d->os << ex;

  if( n > 0 )
  {
    *d->os << '@' << *d->var;

    if( n > 1 )
      *d->os << '^' << n;
  }

  ++d->index;
}

void pseries::print(std::ostream &os) const
{
  print_data dat =
    { &os, 0, &m_var };
  pseries_detail::closure_t closure =
    { &print_closure, &dat };

  m_rep->map_closure( &closure );
}

/*
util::cmp_t pseries::compare_same_type(const basic &o_) const
{
  const pseries &o = static_cast<const pseries&>( o_ );

  util::cmp_t c = symbol::compare( m_var, o.m_var );
  if( c ) return c;

  c = util::compare( m_seq.size(), o.m_seq.size() );
  if( c ) return c;

  seq_t::const_iterator i1 = m_seq.begin(), i2 = o.m_seq.begin();
  for( std::size_t n = m_seq.size(); n != 0; --n )
  {
    c = expr::compare( *i1, *i2 );
    if( c ) return c;
  }

  return 0;
}

expr pseries::differentiate(const symbol &s) const
{
  ptr< pseries > ret = new pseries( m_var );

  if( symbol::compare( m_var, s ) == 0 )
  {
    seq_t::const_iterator it = ++m_seq.begin(), en = m_seq.end();
    std::size_t count = 1;

    for( ; it != en; ++it )
      ret->m_seq.push_back( count * *it );
  }
  else
  {
    foreach( const expr &e, m_seq )
      ret->m_seq.push_back( e.diff( s ) );
  }

  return ret;
}
*/
