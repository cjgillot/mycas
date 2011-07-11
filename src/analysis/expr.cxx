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

#include "analysis/ptr.ixx"

namespace analysis {

const unsigned expr::default_eval_depth = 10;

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
    reset(ap0);                                 \
    ap = cow<klass>();                          \
  } while(0)

#define PREPARE_OTHER(klass)                    \
  const klass* bp = b.get()->as_##klass()       \

// take care to cache the {b.get()} pointer _before_ cow'ing
#define PREPARE(klass)        \
  PREPARE_OTHER(klass);       \
  PREPARE_SELF (klass)

expr &expr::operator+=(expr b) {
  if(!b.get()) return *this;
  if(!get())   return *this = b;

  PREPARE(add);

  ap->iadd(*bp);

  eval();

  return *this;
}
expr &expr::operator-=(expr b) {
  if(!b.get()) return  *this;
  if(!get())   return (*this = b).ineg();

  PREPARE(add);

  ap->isub(*bp);

  eval();

  return *this;
}

expr &expr::operator*=(expr b) {
  if(!b.get()) return *this = b;
  if(!get())   return *this;

  PREPARE(mul);

  ap->imul(*bp);

  eval();

  return *this;
}
expr &expr::operator/=(expr b) {
  assert(b.get());
  if(!get())   return *this;

  PREPARE(mul);

  ap->idiv(*bp);

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

expr &expr::iinv() {
  if(!get()) return *this;

  PREPARE_SELF(mul);

  ap->iinv();

  eval();

  return *this;
}

expr expr::pow(const expr &o) const {
  expr ret ( power::from_be(get(), o.get()) );
  ret.eval();
  return ret;
}

namespace {

struct comparator {
  inline int
  operator()(const basic &a, const basic &b)
  { return basic::compare(a,b); }
};

}

int
expr::compare(const expr &a, const expr &b)
{ return super::compare(a,b, comparator()); }

}
