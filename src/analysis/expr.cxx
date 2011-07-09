/*
 * expr.cxx
 *
 *  Created on: 21 juin 2011
 *      Author: k1000
 */

#include "analysis/expr.hxx"
#include "analysis/numeric.hxx"

#include "analysis/add.hxx"
#include "analysis/mul.hxx"
#include "analysis/power.hxx"

namespace analysis {

const unsigned expr::default_eval_depth = 10;

expr::expr() {}
expr::expr(const expr &o)
: super(o) {}
expr &expr::operator=(const expr &o) {
  super::operator=(o);
  return *this;
}

expr::expr(const basic* p)
: super(p) {}

expr::~expr() {}

void
expr::swap(expr &o) {
  super::swap(o);
}

void expr::eval(unsigned lv) const
{ super::eval(lv); }

template<class T>
T* expr::cow() {
  return static_cast<T*>(super::cow());
}


#define PREPARE_SELF(klass)                     \
  klass* ap = 0;                                \
  do {                                          \
    const klass* ap0 =                          \
      dynamic_cast<const klass*>(get());        \
    if(!ap0) ap0 = get()->as_##klass();         \
    ap = ap0->cow<klass>();                     \
    reset(ap);                                  \
  } while(0)

#define PREPARE_OTHER(klass)                    \
  boost::intrusive_ptr<const klass> bp          \
  = b.get()->as_##klass();

expr &expr::operator+=(const expr &b) {
  if(!b.get()) return *this;
  if(!get())   return *this = b;

  PREPARE_SELF (add);
  PREPARE_OTHER(add);

  ap->iadd(*bp.get());

  eval();

  return *this;
}
expr &expr::operator-=(const expr &b) {
  if(!b.get()) return  *this;
  if(!get())   return (*this = b).ineg();

  PREPARE_SELF (add);
  PREPARE_OTHER(add);

  ap->isub(*bp.get());

  eval();

  return *this;
}

expr &expr::operator*=(const expr &b) {
  if(!b.get()) return *this = b;
  if(!get())   return *this;

  PREPARE_SELF (mul);
  PREPARE_OTHER(mul);

  ap->imul(*bp.get());

  eval();

  return *this;
}
expr &expr::operator/=(const expr &b) {
  assert(b.get());
  if(!get())   return *this;

  PREPARE_SELF (mul);
  PREPARE_OTHER(mul);

  ap->idiv(*bp.get());

  eval();

  return *this;
}

expr &expr::ineg() {
  if(!get()) return *this;

  PREPARE_SELF(add);

  ap->ineg();

  eval();

  return *this;
}

expr &expr::ipow(const expr &o) {
  if(!o.get()) return *this = number::one;
  if(!get()) return *this;

  PREPARE_SELF(power);

  ap->ipow(o.get());

  eval();

  return *this;
}

expr expr::pow(const expr &o) const
{ return expr(*this).ipow(o); }

int
expr::do_compare(const basic &b) const
{ return basic::compare(*get(),b); }

}
