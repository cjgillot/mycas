#ifndef NUMERIC_MODULAR_MTGM_HPP
#define NUMERIC_MODULAR_MTGM_HPP

#include "numeric/modular/basecase.hpp"

namespace numeric {

namespace detail_modular {

/*!
 * \brief Modular integer in Montgomery representation
 *
 * Given an odd modulus \f[ M \f], we choose \f[ N=2^n > M \f] a power of two and \f[ V = N^-1 mod M \f].
 *
 * Modular integer \f[ a \f] is now represented by \f[ a*N mod M \f].
 */
class montgom_ring
: public basecase_ring
{
protected:
  unsigned long m; // M <= 2^m
  unsigned long n; // N = 2^n, n <= m
  fmpz_t V;        // V = N^-1 mod M

private:
  montgom_ring(const fmpz_t, unsigned long, unsigned long, const fmpz_t);
  virtual ~montgom_ring();

public:
  static montgom_ring* try_make_ring(const fmpz_t);

public:
  typedef basecase_ring::obj_t obj_t;

public:
  virtual void init(obj_t, long = 0) const;
  virtual void set (obj_t, long) const;
//   virtual void copy(obj_t, const obj_t) const;
//   virtual void clear(obj_t) const;

public: // any kind of aliasing is allowed
//   virtual void neg(obj_t, const obj_t) const;
  virtual bool inv(obj_t, const obj_t) const;

  virtual void incr(obj_t, const obj_t) const;
  virtual void decr(obj_t, const obj_t) const;

//   virtual void add(obj_t, const obj_t, const obj_t) const;
//   virtual void sub(obj_t, const obj_t, const obj_t) const;
  virtual void mul(obj_t, const obj_t, const obj_t) const;
  virtual bool div(obj_t, const obj_t, const obj_t) const;

  virtual void addmul(obj_t, const obj_t, const obj_t) const;
  virtual void submul(obj_t, const obj_t, const obj_t) const;

  virtual bool pow(obj_t, const obj_t, long) const;

//   virtual bool equal(const obj_t, const obj_t) const;
//   virtual int  cmp(const obj_t, const obj_t) const;
};

}

}

#endif
