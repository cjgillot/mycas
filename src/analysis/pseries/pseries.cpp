#include "analysis/pseries/pseries.hpp"

#include "analysis/expr.hpp"

#include "analysis/pseries/repr.hpp"

#include "util/ptr.hpp"

using namespace analysis;

pseries::pseries(const symbol& var)
: basic(), m_var( var ), m_rep( new repr() )
{}

pseries::pseries(const pseries &o)
: basic(o), m_var( o.m_var ), m_rep( o.m_rep )
{}

pseries::~pseries()
{}

pseries::pseries(const symbol& var, repr* rep)
: basic(), m_var(var), m_rep( rep )
{}

bool pseries::match_same_type(const basic &, match_detail::match_state &) const
{
  // TODO
  return false;
}

expr pseries::subs(const exmap &) const
{
  // TODO
  return this;
}
