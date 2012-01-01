#include "mp_operators.hpp"

#include "number.hpp"

using namespace numeric;

int main()
{
  number a ( 3 ), b( 2 );
  number c = fmpq_class( 2.5 );

  std::cout << std::boolalpha << (c / b.inv()).is_integer() << std::endl;

//   std::cout << c.pow( a ) << std::endl;

  return 0;
}
