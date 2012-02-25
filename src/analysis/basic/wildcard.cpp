#include "analysis/wildcard.hpp"

#include "analysis/expr.hpp"

using namespace analysis;
using namespace match_detail;

wildcard_base*
wildcard_base::clone() const
{ return const_cast<wildcard_base*>( this ); }

expr
wildcard_base::differentiate(const symbol &) const
{ return numerical::zero(); }

util::cmp_t
wildcard_base::compare_same_type(const basic &o_) const
{
  const wildcard_base &o = static_cast<const wildcard_base&>( o_ );
  return util::compare( m_id, o.m_id );
}

expr
wildcard_base::subs( const exmap &m ) const
{
  exmap::const_iterator it = m.find( this );
  if( it != m.end() )
    return it->second;
  return this;
}

void
wildcard_base::print(std::basic_ostream<char> &os) const
{ os << '$' << '*' << id(); }

void
wildcard_::print(std::basic_ostream<char> &os) const
{ os << '$' << '&' << id(); }
