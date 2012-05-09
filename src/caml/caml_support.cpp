#include "caml_support.hpp"

#include <caml.hpp>

#include <cstdlib>

using namespace caml;

static std::size_t counter = 0;

initializer_t::initializer_t()
{
  if( counter++ == 0 )
  {
    char* argv[] = { nullptr };
    caml_startup(argv);
  }
}

initializer_t::~initializer_t()
{
  if( --counter == 0 )
  {
    // TODO finalize caml
  }
}
