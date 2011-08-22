/*
 * prod.hpp
 *
 *  Created on: 20 juin 2011
 *      Author: k1000
 */

#ifndef MUL_HPP_
#define MUL_HPP_

#include "analysis/basic.hpp"

#include "analysis/expairseq.hpp"
#include "analysis/power.hpp"

#include "util/final.hpp"

namespace analysis {

class prod
: public expairseq<prod, power> {

  friend class expairseq<prod, power>;
  typedef expairseq<prod, power> super;
  REGISTER_CLASS( prod, super )

public:
  using super::handle;
  struct ep;

protected:
  explicit prod(const number &);
  prod(const number &, const power*);

  using super::add_t;
  using super::sub_t;
  using super::neg_t;

  template<class Tag>
  prod(const prod &a, const prod &b, Tag);
  prod(const prod &a, neg_t);

public:
  prod* clone() const
  { return new prod(*this); }

public:
  virtual const prod* as_prod() const
  { return this; }

public:
  bool null() const;
  bool unit() const;
  expr eval(unsigned) const;
  expr diff(const symbol&, unsigned=1) const;

public:
  prod* mul(const prod &o) const;
  prod* div(const prod &o) const;
  prod* inv() const;

private:
  void print_base(std::basic_ostream<char> &os) const
  { os << '*'; }

public:
  static prod*
  from_1basic(const basic*);

  static prod*
  from_numeric(const numeric*);
};

}

#endif /* MUL_HPP_ */
