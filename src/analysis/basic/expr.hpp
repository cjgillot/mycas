#ifndef EXPR_HPP_
#define EXPR_HPP_

#include "operators.hpp"

#include "util/compare.hpp"
#include "util/attribute.hpp"

#include "analysis/basic/ptr.hpp"
#include "analysis/basic/basic.hpp"
#include "analysis/basic/numerical.hpp"

#include "analysis/forward.hpp"

#define EXPR_NUMERIC_CACHE_LIMIT 20

namespace analysis {

/*!\brief Expression class
 *
 * \c expr is an intrusive pointer class,
 * forwarding \c basic operations.
 */
class expr
: public operators::printable<expr
, operators::ordered<expr
> > {

private:
  //!\brief Shared pointer representation
  //! \invariant m_impl != 0
  mutable ptr<const basic> m_impl;

private:
  static const unsigned default_eval_depth;
  static const numerical* small_numeric_cache(long);

public:
  expr();

  expr(const expr &) = default;
  expr(expr&&) = default;

  expr &operator=(const expr &) = default;
  expr &operator=(expr&&) = default;

  void swap(expr &o);
  ~expr() = default;

  expr(const basic*);
  expr(const number &);
  template<class T> expr(const ptr<T> &);

  expr(signed int); expr(signed long); //expr(long long);
  expr(unsigned int); expr(unsigned long); //expr(unsigned long long);

  expr(float); expr(double); expr(long double);

public:
  //!\brief Underlying basic pointer access
  const basic* get() const { return m_impl.get(); }

  explicit operator const ptr<const basic>&() const { return m_impl; }

public: // basic forwarding
  void eval(unsigned = default_eval_depth) const;

  bool null() const;
  bool unit() const;

  expr diff(const symbol&, unsigned=1) const;
  expr expand() const;
  expr series(const symbol &) const;

  bool has(const symbol&) const;
  bool match(const expr &, match_state &) const;
  expr subs(const exmap &) const;

  void print(std::basic_ostream<char> &os) const;

  std::size_t hash() const;

public: // RTTI
  bool is_numerical() const;
  template<class T> bool     is_a() const;
  template<class T> const T* as_a() const;
  template<class T> bool     is_exactly_a() const;

  template<class V> void accept(V &) const;
  template<class V> void accept(const V &) const;

public: // operations
  expr &ineg();
  expr &iinv();

  expr &operator+=(const expr &);
  expr &operator-=(const expr &);
  expr &operator*=(const expr &);
  expr &operator/=(const expr &);

public:
  static expr neg(const expr&);
  static expr inv(const expr&);

  static expr add(const expr&, const expr&);
  static expr sub(const expr&, const expr&);

  static expr mul(const expr&, const expr&);
  static expr div(const expr&, const expr&);

  static expr pow(const expr&, const expr&);

  static util::cmp_t compare(const expr&, const expr&);
};

} // namespace analysis

namespace std {

template<>
struct less< analysis::expr >
: public std::binary_function<analysis::expr, analysis::expr, bool>
{
  inline bool operator()( const analysis::expr &a, const analysis::expr &b ) const
  { return analysis::expr::compare( a, b ) < 0; }
};

inline void swap(analysis::expr &a, analysis::expr &b)
{ a.swap( b ); }

}

#endif /* EXPR_HPP_ */
