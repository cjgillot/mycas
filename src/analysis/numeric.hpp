#ifndef NUMERIC_HPP_
#define NUMERIC_HPP_

#include "analysis/basic.hpp"

#include "algebra/real.hpp"

#include "analysis/ptr.hpp"

namespace analysis {

class numeric
: public basic {

  REGISTER_FINAL( numeric, basic )

  friend class number;

private:
  algebra::real m_value;

public:
  numeric(const numeric &);
  numeric &operator=(const numeric &);
  void swap(numeric &);

public:
  numeric(double v);
  ~numeric();

  static const numeric* zero()
  {
    static ptr<const numeric> value ( new numeric( 0. ) );
    return value.get();
  }
  static const numeric* one()
  {
    static ptr<const numeric> value ( new numeric( 1. ) );
    return value.get();
  }

public:
  numeric* clone() const;

public:
  bool null() const;
  bool unit() const;
  bool has(const symbol&) const;

public:
  virtual bool is_numeric() const;
  virtual const sum* as_sum() const;
  virtual const prod* as_prod() const;

public:
  static const numeric* add(const numeric &, const numeric&);
  static const numeric* sub(const numeric &, const numeric&);

  static const numeric* mul(const numeric &, const numeric&);
  static const numeric* div(const numeric &, const numeric&);

  static const numeric* neg(const numeric &);
  static const numeric* inv(const numeric &);

public:
  expr pow(const expr &o) const;
  const numeric* pow(const numeric &o) const;
  void print(std::basic_ostream<char> &os) const;
  expr subs(const exmap &) const;

private:
  expr differentiate(const symbol&) const;

  util::cmp_t compare_same_type(const basic&) const;

  std::size_t hash() const
  { return boost::hash<algebra::real>()(m_value); }
};


}


#endif /* NUMERIC_HXX_ */
