#ifndef NUMERIC_RATIONAL_FIELD_HPP
#define NUMERIC_RATIONAL_FIELD_HPP

#include "numeric/ring_base.hpp"

namespace numeric {

class rational_field
: public ring_base
{
public:
  rational_field() {}
  virtual ~rational_field() {}

public:
  typedef ring_base::obj_t obj_t;

public:
  virtual void init(obj_t, long = 0, long = 1) const;
  virtual void set (obj_t, long, long = 1) const;
  virtual void copy(obj_t, const obj_t) const;
  virtual void clear(obj_t) const;

public: // any kind of aliasing is allowed
  virtual void neg(obj_t, const obj_t) const;

  virtual void incr(obj_t) const;
  virtual void decr(obj_t) const;

  virtual void add(obj_t, const obj_t, const obj_t) const;
  virtual void sub(obj_t, const obj_t, const obj_t) const;
  virtual void mul(obj_t, const obj_t, const obj_t) const;
  virtual void div(obj_t, const obj_t, const obj_t) const;
//   virtual void mod(obj_t, const obj_t, const obj_t) const;

  virtual void addmul(obj_t, const obj_t, const obj_t) const;
  virtual void submul(obj_t, const obj_t, const obj_t) const;

  virtual void pow(obj_t, const obj_t, unsigned long) const;
  virtual void pow(obj_t, const obj_t, long) const;

  virtual bool equal(const obj_t, const obj_t) const;
  virtual int  cmp(const obj_t, const obj_t) const;
};

}

#endif
