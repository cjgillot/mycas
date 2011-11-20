#ifndef NUMERIC_MODULAR_RING_HPP
#define NUMERIC_MODULAR_RING_HPP

#include "numeric/ring_base.hpp"

namespace numeric {

namespace detail_modular {

class modular_ring
// : public ring_base
{
public:
  modular_ring() {}
  virtual ~modular_ring() {}

public:
  typedef ring_base::obj_t obj_t;

public:
  virtual void init(obj_t, long = 0) const = 0;
  virtual void set (obj_t, long) const = 0;
  virtual void copy(obj_t, const obj_t) const = 0;
  virtual void clear(obj_t) const = 0;

public: // any kind of aliasing is allowed
  virtual void neg(obj_t, const obj_t) const = 0;
  virtual bool inv(obj_t, const obj_t) const = 0;

//   virtual void incr(obj_t, const obj_t) const = 0;
//   virtual void decr(obj_t, const obj_t) const = 0;

  virtual void add(obj_t, const obj_t, const obj_t) const = 0;
  virtual void sub(obj_t, const obj_t, const obj_t) const = 0;
  virtual void mul(obj_t, const obj_t, const obj_t) const = 0;
  virtual bool div(obj_t, const obj_t, const obj_t) const = 0;

  virtual void addmul(obj_t, const obj_t, const obj_t) const = 0;
  virtual void submul(obj_t, const obj_t, const obj_t) const = 0;

  virtual bool pow(obj_t, const obj_t, long) const = 0;

  virtual bool equal(const obj_t, const obj_t) const = 0;
  virtual int  cmp(const obj_t, const obj_t) const = 0;

private:
  void binpow(obj_t, const obj_t, long) const;
};

}

}

#endif
