#include "analysis/pseries.hpp"

#include "analysis/expr.hpp"
#include "analysis/expr.ipp"

#include "util/foreach.hpp"

#include <boost/functional/hash.hpp>

using namespace analysis;

pseries::pseries(const symbol& var)
: basic(), m_var( var ), m_seq()
{}

pseries::pseries(const pseries &o)
: basic(o), m_var( o.m_var ), m_seq( o.m_seq )
{}

pseries::~pseries()
{}

expr pseries::eval(unsigned lv) const
{
  if( lv == 0 )
    return this;

  --lv;
  foreach( const expr &e, m_seq )
    e.eval(lv);

  basic::eval(lv);
  return this;
}

std::size_t pseries::hash() const
{
  std::size_t seed = m_var.hash();

  foreach( const expr &e, m_seq )
    boost::hash_combine( seed, e.hash() );

  return seed;
}

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

void pseries::print(std::basic_ostream< char > &os) const
{
  std::size_t n = 0;
  foreach( const expr &e, m_seq )
  {
    if( n != 0 )
      os << ' ';

    os << e;

    if( n > 0 )
    {
      os << " @ " << m_var;
      if( n > 1 )
        os << '^' << n;
    }

    ++n;
  }
}

bool pseries::has(const symbol &s) const
{
  if( m_var.has( s ) )
    return true;

  foreach( const expr &e, m_seq )
    if( e.has( s ) )
      return true;

  return false;
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
