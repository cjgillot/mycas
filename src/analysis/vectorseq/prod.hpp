#ifndef PROD_HPP_
#define PROD_HPP_

#include "analysis/expr.hpp"

#include "analysis/vectorseq/vectorseq.hpp"
#include "analysis/vectorseq/power.hpp"

#include "util/functor.hpp"

namespace analysis {

class prod
: public vectorseq<prod, power> {

  // grant access to super
  friend class vectorseq<prod, power>;
  typedef vectorseq<prod, power> super;
  REGISTER_FINAL( prod, super )

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
  static prod* mul(const prod &, const prod &);
  static prod* div(const prod &, const prod &);
  static prod* inv(const prod &);

  static prod* from_basic (const basic*);
  static prod* from_number(const number&);

  using super::sort_predicate;

  template< class Iter >
  static prod* from_expr_range(const Iter &b, const Iter &e);
  template< class Iter >
  static prod* from_power_range(const number &n, const Iter &b, const Iter &e);
  template< class Iter >
  static prod* from_sorted_power_range(const number &n, const Iter &b, const Iter &e);
};

namespace detail {

struct expr2power
: std::unary_function<const power*, const expr&>
{
  inline const power* operator()( const expr &ex )
  { return ex.get()->as_power(); }
};

}

template< class Iter >
inline prod* prod::from_expr_range(const Iter &b, const Iter &e)
{
  util::scoped_ptr< prod > tmp ( new prod( 1 ) );
  tmp->construct_expr_range( b, e, detail::expr2power(), functor::multiplies_eq<number>() );
  return tmp.release();
}
template< class Iter >
inline prod* prod::from_power_range(const number &n, const Iter &b, const Iter &e)
{
  ASSERT( ! n.null() );
  util::scoped_ptr< prod > tmp ( new prod( n ) );
  tmp->construct_mono_range( b, e );
  return tmp.release();
}

template< class Iter >
inline prod* prod::from_sorted_power_range(const number &n, const Iter &b, const Iter &e)
{
  ASSERT( ! n.null() );
  util::scoped_ptr< prod > tmp ( new prod( n ) );
  tmp->construct_sorted_mono_range( b, e );
  return tmp.release();
}

// specialize handle::from_expr
namespace epseq {

template<>
inline prod::handle::const_pointer
prod::handle::from_expr( const expr &e )
{ return e.get()->as_prod(); }

} // namespace epseq

} // namespace analysis

#endif
