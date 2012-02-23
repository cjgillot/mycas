#ifndef EXPR_HPP_
#define EXPR_HPP_

#include "operators.hpp"

#include "util/compare.hpp"
#include "util/attribute.hpp"

#include "analysis/basic/ptr.hpp"
#include "analysis/basic/basic.hpp"
#include "analysis/basic/numerical.hpp"

#include "analysis/forward.hpp"

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

  struct enabler {};

public:
  expr();

  expr(const expr &);
  expr &operator=(const expr &);

  //! \brief Non-throwing swap
  void swap(expr &o);
  //! \brief Non-virtual destructor
  ~expr();

  //! \brief Explicit creation from \c basic instance
  //! \param b a non-null pointer to a \c basic instance
  expr(const basic*);

  template<class T>
  expr(const ptr<T> &);

//   explicit
  expr(const number &);

  template<class T>
  expr(T n, typename boost::enable_if<boost::is_arithmetic<T>, enabler>::type = enabler() )
  : m_impl( new numerical( n ) ) {}

public: // ptr<> behaviour
  operator const ptr<const basic> &() const { return m_impl; }

  const basic* get() const { return m_impl.get(); }
//   const basic &operator *() const { return m_impl.operator*(); }
//   const basic *operator->() const { return m_impl.operator->(); }

private:
  static const unsigned default_eval_depth;
public:
  void eval(unsigned = default_eval_depth) const;

public:
  //! \brief Visitor accept
  template<class V> void accept(V &) const;
  template<class V> void accept(const V &) const;

public:
  //! \brief Nullity test
  bool null() const;
  //! \brief Unity test
  bool unit() const;

  //! \brief Printing
  void print(std::basic_ostream<char> &os) const;

  bool has(const symbol&) const;
  expr diff(const symbol&, unsigned=1) const;
  expr expand() const;
  bool match(const expr &, match_state &) const;
  expr subs(const exmap &) const;
  expr series(const symbol &) const;

public: // RTTI
  bool is_numerical() const;
  template<class T> bool     is_a() const;
  template<class T> const T* as_a() const;
  template<class T> bool     is_exactly_a() const;

public:
  expr neg() const;
  expr inv() const;

  static expr add(const expr&, const expr&);
  static expr sub(const expr&, const expr&);

  static expr mul(const expr&, const expr&);
  static expr div(const expr&, const expr&);

  expr pow(const expr &) const;

public:
  expr &ineg() {
    neg().swap( *this );
    return *this;
  }
  expr &iinv() {
    inv().swap( *this );
    return *this;
  }

#define OP_EQ( op, name )       \
  expr &                        \
  operator op##=(const expr &o) \
  {                             \
    expr::name( *this, o )      \
      .swap( *this );           \
    return *this;               \
  }

  OP_EQ( +, add )
  OP_EQ( -, sub )
  OP_EQ( *, mul )
  OP_EQ( /, div )

#undef OP_EQ

public:
  static util::cmp_t
  compare(const expr &a, const expr &b);

  std::size_t hash() const;

private:
  /*!\brief Shared pointer representation
   * \invariant m_impl != 0
   */
  mutable ptr<const basic> m_impl;
};

std::basic_ostream<char> &operator<<(std::basic_ostream<char> &, const expr &);

expr pow(const expr &, const expr &);

} // namespace analysis

namespace std {

template<>
struct less< analysis::expr >
: public std::binary_function< bool, const analysis::expr &, const analysis::expr & >
{
  inline bool operator()( const analysis::expr &a, const analysis::expr &b ) const
  { return analysis::expr::compare( a, b ) < 0; }
};

}

#endif /* EXPR_HPP_ */
