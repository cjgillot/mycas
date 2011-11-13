#ifndef NUMERIC_MODULAR_PPOW_HPP
#define NUMERIC_MODULAR_PPOW_HPP

#include "numeric/modular/ring_base.hpp"

#include <flint.h>
#include <padic.h>

namespace numeric {

namespace detail {

class primepow_ring
: public modular_ring
{
  padic_ctx_t context;
  padic_inv_t preinv;
  padic_t unit;

public:
  primepow_ring(const fmpz_t, long);
  virtual ~primepow_ring();

public:
  typedef modular_ring::obj_t obj_t;

public:
  virtual void init(obj_t, long = 0) const;
  virtual void set (obj_t, long) const;
  virtual void copy(obj_t, const obj_t) const;
  virtual void clear(obj_t) const;

public: // any kind of aliasing is allowed
  virtual void neg(obj_t, const obj_t) const;
  virtual void inv(obj_t, const obj_t) const;

  virtual void incr(obj_t, const obj_t) const;
  virtual void decr(obj_t, const obj_t) const;

  virtual void add(obj_t, const obj_t, const obj_t) const;
  virtual void sub(obj_t, const obj_t, const obj_t) const;
  virtual void mul(obj_t, const obj_t, const obj_t) const;
  virtual void div(obj_t, const obj_t, const obj_t) const;

  virtual void addmul(obj_t, const obj_t, const obj_t) const;
  virtual void submul(obj_t, const obj_t, const obj_t) const;

  virtual void pow(obj_t, const obj_t, long) const;

  virtual bool equal(const obj_t, const obj_t) const;
  virtual int  cmp(const obj_t, const obj_t) const;
};

}

}

#endif
