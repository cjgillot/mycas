#include "caml_support.hpp"
#include "util/null.hpp"

#include <caml/mlvalues.h>

extern "C" value caml_startup(const char**);

using namespace caml;

initializer_t::initializer_t()
{
  const char* argv[] = { "", nullptr };
  caml_startup(argv);
}

initializer_t::~initializer_t() {}

initializer_t initializer_t::instance;
