#ifndef NUMERIC_MODULAR_RING_HPP
#define NUMERIC_MODULAR_RING_HPP

#include "util/refcounted.hpp"

namespace numeric {

namespace detail {

class modular_ring
: private util::noncopyable
{
public:
  modular_ring() {}
  virtual ~modular_ring() {}

public:
  typedef void* obj_t[1];

public:
  virtual void init(obj_t, long = 0) const = 0;
  virtual void set (obj_t, long) const = 0;
  virtual void copy(obj_t, const obj_t) const = 0;
  virtual void clear(obj_t) const = 0;

public: // any kind of aliasing is allowed
  virtual void neg(obj_t, const obj_t) const = 0;
  virtual void inv(obj_t, const obj_t) const = 0;

  virtual void incr(obj_t, const obj_t) const = 0;
  virtual void decr(obj_t, const obj_t) const = 0;

  virtual void add(obj_t, const obj_t, const obj_t) const = 0;
  virtual void sub(obj_t, const obj_t, const obj_t) const = 0;
  virtual void mul(obj_t, const obj_t, const obj_t) const = 0;
  virtual void div(obj_t, const obj_t, const obj_t) const = 0;

  virtual void addmul(obj_t, const obj_t, const obj_t) const = 0;
  virtual void submul(obj_t, const obj_t, const obj_t) const = 0;

  virtual void pow(obj_t, const obj_t, long) const = 0;

  virtual bool equal(const obj_t, const obj_t) const = 0;
  virtual int  cmp(const obj_t, const obj_t) const = 0;
};

}

}

#endif
