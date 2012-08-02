#ifndef PROD_HPP_
#define PROD_HPP_

#include "analysis/expr.hpp"

#include "analysis/vectorseq/vectorseq.hpp"
#include "analysis/vectorseq/power.hpp"

namespace analysis {

class prod final
: public vectorseq<prod, power> {

  // grant access to super
  friend class vectorseq<prod, power>;
  typedef vectorseq<prod, power> super;
  REGISTER_STATIC_FINAL( prod, super, PROD_RTTI )

//   friend class power;
  friend class sum;

public:
  using super::handle;

private:
  explicit prod( const number & );

public: // access
  using super::coef;

  using super::iterator;
  using super::reverse_iterator;
  using super::const_iterator;
  using super::const_reverse_iterator;

  using super::begin;
  using super::end;
  using super::rbegin;
  using super::rend;

  using super::size;
  using super::empty;

public:
  prod* clone() const
  { return new prod( *this ); }

  const prod* as_prod() const
  { return this; }

public:
  bool null() const;
  bool unit() const;
  expr eval(unsigned) const;
  expr expand() const;
  expr subs(const std::map<expr,expr> &) const;
  expr series(const symbol &) const;

private:
  expr differentiate(const symbol&) const;
  void print_base(std::basic_ostream<char> &os) const { os << '*'; }
  bool match_same_type(const basic &, match_state &) const;

public:
  static prod* mul_num(const prod &, const number &);
  static prod* div_num(const prod &, const number &);

  static prod* mul(const prod &, const prod &);
  static prod* div(const prod &, const prod &);
  static prod* inv(const prod &);

  static prod* from_basic (const basic*);
  static prod* from_number(const number&);

  using super::sort_predicate;

  template<typename Iter>
  static prod* from_expr_range(Iter&& b, Iter&& e);
  template<typename Iter>
  static prod* from_power_range(const number &n, Iter&& b, Iter&& e);
  template<typename Iter>
  static prod* from_sorted_power_range(const number &n, Iter&& b, Iter&& e);
};

template<typename Iter>
inline prod* prod::from_expr_range(Iter&& b, Iter&& e)
{
  std::unique_ptr< prod > tmp ( new prod( 1 ) );
  tmp->construct_expr_range(
    std::forward<Iter>(b), std::forward<Iter>(e),
    [](const expr &ex) { return ex.get()->as_power(); },
    [](number &acc, const number &n) { acc *= n; }
  );
  return tmp.release();
}
template<typename Iter >
inline prod* prod::from_power_range(const number &n, Iter&& b, Iter&& e)
{
  std::unique_ptr< prod > tmp ( new prod( n ) );
  tmp->construct_mono_range( std::forward<Iter>(b), std::forward<Iter>(e) );
  return tmp.release();
}
template<typename Iter>
inline prod* prod::from_sorted_power_range(const number &n, Iter&& b, Iter&& e)
{
  std::unique_ptr< prod > tmp ( new prod( n ) );
  tmp->construct_sorted_mono_range( std::forward<Iter>(b), std::forward<Iter>(e) );
  return tmp.release();
}

// specialize handle::from_expr
namespace vseq {

template<>
inline prod::handle::const_pointer
prod::handle::from_expr( const expr &e )
{ return e.get()->as_prod(); }

} // namespace vseq

} // namespace analysis

#endif
