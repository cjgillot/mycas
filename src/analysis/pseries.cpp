#include "analysis/pseries.hpp"

#include "analysis/expr.hpp"
#include "analysis/expr.ipp"

#include "pseries/repr.hpp"

#include "util/ptr.hpp"

using namespace analysis;
using pseries_detail::repr;

pseries::pseries(const symbol& var)
: basic(), m_var( var ), m_rep( nullptr )
{}

pseries::pseries(const pseries &o)
: basic(o), m_var( o.m_var ), m_rep( o.m_rep )
{}

pseries::~pseries()
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
