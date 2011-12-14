#ifndef ADD_HPP_
#define ADD_HPP_

#include "analysis/basic.hpp"

#include "analysis/expairseq.hpp"
#include "analysis/prod.hpp"

#include "util/functor.hpp"

namespace analysis {

/*!\brief Addition class
 *
 * This \c expairseq-derived class represents
 * sums as a linear combination of multiplications,
 * whose coefficients are pure \c number's.
 *
 * Printing : (+ c m1 m2 m3 ...)
 */
class sum
: public expairseq<sum, prod> {

  friend class expairseq<sum, prod>;
  typedef expairseq<sum, prod> super;
  REGISTER_FINAL( sum, super )

//   friend class prod;
//   friend class power;

private:
  explicit sum( const number & = number::zero() );

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
  sum* clone() const
  { return new sum(*this); }

public:
  virtual const sum* as_sum() const
  { return this; }

public:
  bool null() const;
  expr eval(unsigned) const;
  expr expand() const;
  expr subs(const std::map<expr,expr> &) const;

private:
  expr differentiate(const symbol&) const;
  void print_base(std::basic_ostream<char> &os) const { os << '+'; }
  bool match_same_type(const basic &, match_state &) const;

public:
  static sum* add(const sum &, const sum &);
  static sum* sub(const sum &, const sum &);
  static sum* neg(const sum &);

  static sum* sca(const number &, const sum &);

  static sum* from_basic (const basic*);
  static sum* from_number(const number&);

  template< class Iter >
  static sum* from_expr_range(const Iter &b, const Iter &e);
  template< class Iter >
  static sum* from_prod_range(const number &n, const Iter &b, const Iter &e);
  template< class Iter >
  static sum* from_mutable_prod_range(const number &n, const Iter &b, const Iter &e);
};

namespace detail {

struct expr2prod
: std::unary_function<const prod*, const expr&>
{
  inline const prod* operator()( const expr &ex )
  { return ex.get()->as_prod(); }
};

}

template< class Iter >
inline sum* sum::from_expr_range(const Iter &b, const Iter &e)
{
  util::scoped_ptr< sum > tmp ( new sum );
  tmp->construct_expr_range( b, e, detail::expr2prod(), functor::plus_eq<number>() );
  return tmp.release();
}
template< class Iter >
inline sum* sum::from_prod_range(const number &n, const Iter &b, const Iter &e)
{
  util::scoped_ptr< sum > tmp ( new sum( n ) );
  tmp->construct_mono_range( b, e );
  return tmp.release();
}
template< class Iter >
inline sum* sum::from_mutable_prod_range(const number &n, const Iter &b, const Iter &e)
{
  util::scoped_ptr< sum > tmp ( new sum( n ) );
  tmp->construct_mutable_mono_range( b, e );
  return tmp.release();
}

}

#endif /* ADD_HPP_ */
