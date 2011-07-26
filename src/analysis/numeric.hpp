/*
 * numeric.hpp
 *
 *  Created on: 20 juin 2011
 *      Author: k1000
 */

#ifndef NUMERIC_HPP_
#define NUMERIC_HPP_

#include "analysis/basic.hpp"

#include "algebra/real.hpp"
#include "util/final.hpp"

#include<boost/intrusive_ptr.hpp>

namespace analysis {

class numeric;

DECLARE_FINAL_CLASS(numeric)

class numeric: FINAL_CLASS(numeric)
, public basic {

  DEFINE_CONST_VISITABLE()

public:
  numeric(const numeric &);
  numeric &operator=(const numeric &);
  void swap(numeric &);

public:
  numeric(double v);
  ~numeric();

  static const numeric* zero()
  {
    static boost::intrusive_ptr<const numeric> value ( new numeric( 0. ) );
    return value.get();
  }
  static const numeric* one()
  {
    static boost::intrusive_ptr<const numeric> value ( new numeric( 1. ) );
    return value.get();
  }

public:
  numeric* clone() const;

public:
  bool null() const;
  bool unit() const;
  expr diff(const symbol&,unsigned) const;
  bool has(const symbol&) const;

public:
  virtual bool is_numeric() const;
  virtual const sum* as_sum() const;
  virtual const prod* as_prod() const;

public:
  const numeric* plus()  const;
  const numeric* minus() const;

  numeric* iadd(const numeric*);
  numeric* isub(const numeric*);

  numeric* ineg();

  numeric* imul(const numeric*);
  numeric* idiv(const numeric*);

  numeric* iinv();

  const numeric* pow(const numeric* o) const;

public:
  void print(std::basic_ostream<char> &os) const;
  int compare_same_type(const basic&) const;

private:
  std::size_t calc_hash() const
  { return boost::hash<algebra::real>()(m_value); }

private:
  algebra::real m_value;
};


}


#endif /* NUMERIC_HXX_ */
