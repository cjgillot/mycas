#ifndef NUMERIC_INTEGER_RING_HPP
#define NUMERIC_INTEGER_RING_HPP

#include "numeric/ring_base.hpp"

namespace numeric {

class integer_ring
: public ring_base
{
public:
  integer_ring() {}
  virtual ~integer_ring() {}

public:
  typedef ring_base::obj_t obj_t;

public:
  virtual void init(obj_t, long = 0) const;
  virtual void set (obj_t, long) const;
  virtual void copy(obj_t, const obj_t) const;
  virtual void clear(obj_t) const;

public: // any kind of aliasing is allowed
  virtual void neg(obj_t, const obj_t) const;

  virtual void incr(obj_t, const obj_t) const;
  virtual void decr(obj_t, const obj_t) const;

  virtual void add(obj_t, const obj_t, const obj_t) const;
  virtual void sub(obj_t, const obj_t, const obj_t) const;
  virtual void mul(obj_t, const obj_t, const obj_t) const;
  virtual void div(obj_t, const obj_t, const obj_t) const;
  virtual void mod(obj_t, const obj_t, const obj_t) const;

  virtual void addmul(obj_t, const obj_t, const obj_t) const;
  virtual void submul(obj_t, const obj_t, const obj_t) const;

  virtual void pow(obj_t, const obj_t, unsigned long) const;

  virtual bool equal(const obj_t, const obj_t) const;
  virtual int  cmp(const obj_t, const obj_t) const;
};

}

#endif
