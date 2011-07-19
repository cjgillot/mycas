/*
 * prod.hpp
 *
 *  Created on: 20 juin 2011
 *      Author: k1000
 */

#ifndef MUL_HPP_
#define MUL_HPP_

#include "analysis/expairseq.hpp"
#include "analysis/power.hpp"

#include "util/final.hpp"

namespace analysis {

class prod
: public expairseq<prod, power> {

  DEFINE_CONST_VISITABLE()

private:
  typedef expairseq<prod, power> super;

public:
  using super::handle;
  struct ep;

public:
  prod();

  prod(const prod &);
  void swap(prod &);

  virtual ~prod();

private:
  explicit
  prod(const number &);
  prod(const number &, const epair &);

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
  virtual bool null() const;
  virtual bool unit() const;
  virtual expr eval(unsigned) const;

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
