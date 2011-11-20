#ifndef NUMERIC_MODULAR_PPOW_HPP
#define NUMERIC_MODULAR_PPOW_HPP

#include "numeric/modular/basecase.hpp"

#include <flint.h>
#include <padic.h>

namespace numeric {

namespace detail_modular {

class primepow_ring
: public basecase_ring
{
  fmpz prime;
  ulong exp;
  padic_inv_t preinv;

public:
  primepow_ring(const fmpz_t, ulong);
  virtual ~primepow_ring();

public:
  typedef basecase_ring::obj_t obj_t;

public: // any kind of aliasing is allowed
  virtual bool inv(obj_t, const obj_t) const;
  virtual bool div(obj_t, const obj_t, const obj_t) const;

  virtual bool pow(obj_t, const obj_t, long) const;
};

}

}

#endif
