#include "analysis/vectorseq/vectorseq_base.hpp"

using namespace analysis;
using namespace vectorseq_detail;

#define VSB vectorseq_base

VSB::VSB(const number &n)
: m_coef( n ), m_poly( /* null */ )
, m_seqhash( 0ul ), m_coefhash( 0ul )
{}

VSB::VSB(const VSB &o)
: basic(o)
, m_coef(o.m_coef), m_poly( o.m_poly )
, m_seqhash(o.m_seqhash), m_coefhash(o.m_coefhash)
{}

VSB::~VSB() {}

bool
VSB::has(const symbol &s) const
{
  if( !poly() ) return false;

  for( const_piterator pit = pbegin(), pen = pend();
       pit != pen; ++pit )
    if( (*pit)->has(s) )
      return true;

  return false;
}

namespace {

struct printer
{
  std::basic_ostream<char> &os;

  printer(std::basic_ostream<char> &s)
  : os(s) {}

  printer(const printer &o)
  : os(o.os) {}

  template<class T>
   void
  operator()(const T &x)
  {
    os << ' ';
    expr(x).print(os);
  }
};

}

void
VSB::print(std::basic_ostream<char> &os) const
{
  os << '(';
  this->print_base(os);
  os << ' ';
  coef().print(os);
  if( ! empty() )
    std::for_each( pbegin(), pend(), printer(os) );
  os << ')';
}
