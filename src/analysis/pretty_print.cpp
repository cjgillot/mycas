#include "analysis/basic.hpp"

#include "analysis/sum.hpp"
#include "analysis/prod.hpp"
#include "analysis/power.hpp"
#include "analysis/symbol.hpp"
#include "analysis/numeric.hpp"

#include "rtti/visitor.hpp"

using namespace analysis;

void pprint( const expr &ex );

struct pretty_printer
{
  std::basic_ostream< char >* stream;
  mutable unsigned level;

  MAKE_VISITOR(
    pretty_printer
  , basic
  , ( sum     )
    ( prod    )
    ( power   )
    ( symbol_ )
    ( numeric )
  )

public:
  void visit( const expr &ex ) const
  {
    ex.get()->accept( *this );
  }

  void visit( const basic &bp ) const
  { // fallback
    bp.print( *stream );
  }

  void visit( const sum &add ) const
  {
    const unsigned lv = level;
    level = 0;

    if( lv > 0 )
      *stream << "( ";

    sum::const_iterator
      it = add.begin()
    , en = add.end();

    if( ! add.coef().null() )
      visit( *add.coef().get() );

    else
    {
      visit( **it );
      ++it;
    }

    for( ; it != en; ++it )
    {
      *stream << " + ";
      visit( **it );
    }

    if( lv > 0 )
      *stream << " )";
    level = lv;
  }

  void visit( const prod &mul ) const
  {
    const unsigned lv = level;
    level = 1;

    if( lv > 1 )
      *stream << "( ";

    prod::const_iterator
      it = mul.begin()
    , en = mul.end();

    if( ! mul.coef().unit() )
      visit( *mul.coef().get() );

    else
    {
      visit( **it );
      ++it;
    }

    for( ; it != en; ++it )
    {
      *stream << " * ";
      visit( **it );
    }

    if( lv > 1 )
      *stream << " )";
    level = lv;
  }

  void visit( const power &be ) const
  {
    const unsigned lv = level;
    level = 2;

    if( lv >= 2 )
      *stream << "( ";

    visit( be.base() );

    --level;

    if( ! be.expo().unit() )
    {
      *stream << '^';
      visit( be.expo() );
    }

    if( lv >= 2 )
      *stream << " )";
    level = lv;
  }
};

void pprint( const expr &ex )
{
  pretty_printer p = { &std::cout, 0 };
  p.visit( ex );
  std::cout << std::endl;
}
