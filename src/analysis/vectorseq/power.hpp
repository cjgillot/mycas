#ifndef POWER_HPP_
#define POWER_HPP_

#include "analysis/expr.hpp"

#include "analysis/vectorseq/vectorseq.hpp"

#include "util/concept.hpp"

namespace analysis {

/*!\brief This is the main power representation class
 *
 * This structure represents the power {b^e}
 * using the expair { coef=e; rest=b }.
 */
class power final
: public basic {

  REGISTER_STATIC_FINAL( power, basic, POWER_RTTI )

  friend class prod;
//   friend class sum;

public:
  struct handle;

private: // cdtor
  power(const expr&, const expr&);

public: // coercion
  power* clone() const;
  const power* as_power() const;

public: // access
  const expr &base() const
  { return m_base; }
  const expr &expo() const
  { return m_expo; }

public: // tests
  bool null() const;
  bool unit() const;

  expr eval(unsigned) const;
  bool has(const symbol&) const;
  expr expand() const;
  expr subs(const std::map<expr,expr> &) const;
  expr series(const symbol &) const;

public:
  void print(std::basic_ostream<char> &) const;

private: // misc.
  expr differentiate(const symbol&) const;
  util::cmp_t compare_same_type(const basic &) const;
  bool match_same_type(const basic &, match_state &) const;

private:
  std::size_t hash() const;

public: // static
  static const power* from_be(const expr &b, const expr &e);
  static const power* from_basic(const basic*);

private: // data
  expr m_base, m_expo;
};

struct power::handle
: private util::implement_concept< ExpairseqHandle< power::handle, power > >
{
public:
  typedef power monomial_type;
  typedef const power* const_pointer;

public: // operations
  static const_pointer add(const_pointer, const_pointer);
  static const_pointer sub(const_pointer, const_pointer);
  static const_pointer neg(const_pointer);

  static const_pointer sca(const_pointer, const number &);

public: // tests
  static bool null(const_pointer p)
  { return p->expo().null(); }

  static util::cmp_t compare(const_pointer a, const_pointer b);
  static util::cmp_t deep_compare(const_pointer a, const_pointer b)
  {
    util::cmp_t c = util::compare( handle::hash( a ) , handle::hash( b ) );
    if(c) return c;
    return a->power::compare_same_type( *b );
  }

public: // misc
  static std::size_t hash(const_pointer p)
  { return p->power::hash(); }

  static std::size_t coef_hash(const_pointer p)
  { return p->expo().hash(); }

  static std::size_t value_hash(const_pointer p)
  { return p->base().hash(); }

  template<class S>
  static void print(const_pointer p, S &os)
  { p->power::print(os); }
};

} // namespace analysis

#endif /* POWER_HPP_ */
