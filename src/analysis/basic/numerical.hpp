#ifndef NUMERIC_HPP_
#define NUMERIC_HPP_

#include "analysis/basic/basic.hpp"
#include "analysis/basic/expr.hpp"
#include "analysis/basic/ptr.hpp"

namespace analysis {

class numerical
: public basic {

  REGISTER_FINAL( numerical, basic )

  friend class number;

public:
  numerical(const numerical &);

  numerical(const number &n)
  : m_value( n ) {}

  template<class T>
  numerical(T v)
  : m_value( v ) {}

  ~numerical() {}

  static const numerical* zero()
  {
    static ptr<const numerical> value ( new numerical( 0 ) );
    return value.get();
  }
  static const numerical* one()
  {
    static ptr<const numerical> value ( new numerical( 1 ) );
    return value.get();
  }

public:
  numerical* clone() const;

public:
  bool null() const;
  bool unit() const;
  bool has(const symbol&) const;

public:
  const number &get() const
  { return m_value; }

public:
  virtual bool is_numerical() const;
  virtual const sum* as_sum() const;
  virtual const prod* as_prod() const;

public:
  static const numerical* add(const numerical &, const numerical&);
  static const numerical* sub(const numerical &, const numerical&);

  static const numerical* mul(const numerical &, const numerical&);
  static const numerical* div(const numerical &, const numerical&);

  static const numerical* neg(const numerical &);
  static const numerical* inv(const numerical &);

public:
  expr pow(const expr &o) const;
  const numerical* pow(const numerical &o) const;
  void print(std::basic_ostream<char> &os) const;
  expr subs(const exmap &) const;

private:
  expr differentiate(const symbol&) const;

  util::cmp_t compare_same_type(const basic&) const;

  std::size_t hash() const
  { return m_value.hash(); }

private:
  number m_value;
};


} // namespace analysis


#endif /* NUMERIC_HXX_ */
