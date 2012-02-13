#ifndef ANALYSIS_PSERIES_HPP
#define ANALYSIS_PSERIES_HPP

#include "analysis/expr.hpp"
#include "analysis/symbol.hpp"

namespace analysis {

namespace pseries_detail { class iterator; }

class pseries
: public basic
{
  REGISTER_FINAL( pseries, basic )

public:
  class repr;

public:
  pseries(const pseries &);
  ~pseries();

  explicit pseries(const symbol &);
  pseries(const symbol &, repr*);

  pseries* clone() const
  { return new pseries(*this); }

public:
  bool null() const;
  bool unit() const;

//   expr pow(const expr &) const;

  bool has(const symbol &) const;
  expr subs(const exmap &) const;

  expr eval(unsigned) const;

  void print(std::ostream &) const;
  std::size_t hash() const;

public:
  typedef pseries_detail::iterator const_iterator;

  const_iterator begin() const;
  const_iterator end()   const;

private:
  bool match_same_type(const basic &, match_state &) const;
  util::cmp_t compare_same_type(const basic &) const;
  expr differentiate(const symbol &) const;

public:
  static pseries neg_series(const pseries &);
  static pseries inv_series(const pseries &);

  static pseries sca_series(const pseries &, const expr &);

  static pseries add_series(const pseries &, const pseries &);
  static pseries sub_series(const pseries &, const pseries &);
  static pseries mul_series(const pseries &, const pseries &);
  static pseries div_series(const pseries &, const pseries &);

private:
  symbol m_var;
  boost::intrusive_ptr<repr> m_rep;
};

} // namespace analysis

#include "analysis/pseries/iterator.hpp"

#endif
