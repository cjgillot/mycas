#include "analysis/function/indets.hpp"

#include "analysis/expr.hpp"
#include "analysis/symbol.hpp"
#include "analysis/vectorseq.hpp"
#include "analysis/function/exprarray.hpp"

#include "rtti/visitor.hpp"

using namespace analysis;

class indets_visitor
{
  MAKE_VISITOR( indets_visitor
  , basic
  , ( numerical )( symbol_ )
    ( sum )( prod )( power )
    ( exprarray )
  )

public:
  std::set<symbol> &indets;

public:
  void visit(const basic &) {}

  void visit(const symbol &s)
  {
    indets.insert( s );
  }

  void visit(const sum &s)
  {
    foreach( const prod* p, s )
      visit( *p );
  }
  void visit(const prod &p)
  {
    foreach( const power* pw, p )
      visit( *pw );
  }
  void visit(const power &p)
  {
    p.base().accept( *this );
    p.expo().accept( *this );
  }

  void visit(const exprarray &a)
  {
    const expr
      *it = a.pbegin(),
      *en = a.pend();
    for( ; it != en; ++it )
      it->accept( *this );
  }
};

std::set<symbol> analysis::indets(const expr &ex)
{
  std::set<symbol> ret;
  indets_visitor v = { ret };
  ex.accept( v );
  return ret;
}
