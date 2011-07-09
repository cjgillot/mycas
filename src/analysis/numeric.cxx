/*
 * numeric.cxx
 *
 *  Created on: 21 juin 2011
 *      Author: k1000
 */

#include "numeric.hxx"

#include "add.hxx"
#include "mul.hxx"

namespace analysis {

const number number::zero(static_cast<const numeric*>(0));
const number number::one(1.);

bool
numeric::is_numeric() const {
  return true;
}
const add*
numeric::as_add() const
{ return add::from_numeric(this); }
const mul*
numeric::as_mul() const
{ return mul::from_numeric(this); }

int numeric::compare_same_type(const basic &o) const {
  return algebra::compare(
    value
  , static_cast<const numeric&>(o).value
  );
}

}
