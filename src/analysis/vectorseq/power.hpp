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
class power
: public basic {

  REGISTER_FINAL( power, basic )

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
  typedef const power* const_pointer;

public: // cdtor
  handle(const_pointer p)
  : m_ptr(p) { ASSERT(p); }
  ~handle() throw() {}

  void swap(handle &o) throw()
  { m_ptr.swap( o.m_ptr ); }

public: // coercion with expr
  handle( const expr &ex )
  : m_ptr( ex.get()->as_power() ) {}
  operator expr() const
  { return expr( m_ptr.get() ); }

public: // operations
  handle operator+(const handle &) const;
  handle operator-(const handle &) const;
  handle operator-() const;

public: // tests
  bool null() const
  { return m_ptr->m_expo.null(); }

  static util::cmp_t compare(const handle &a, const handle &b);
  static util::cmp_t deep_compare(const handle &a, const handle &b)
  {
    util::cmp_t c = util::compare( a.hash() , b.hash() );
    if(c) return c;
    return a.m_ptr->power::compare_same_type(*b.m_ptr);
  }

public: // misc
  std::size_t hash() const
  { return m_ptr->power::hash(); }

  std::size_t coef_hash() const
  { return m_ptr->m_expo.get()->hash(); }

  std::size_t value_hash() const
  { return m_ptr->m_base.get()->hash(); }

  template<class S>
  void print(S &os) const
  { m_ptr->power::print(os); }

  const_pointer get() const
  { return m_ptr.get(); }

private: // data
  ptr<const power> m_ptr;
};

}

namespace std {

inline void swap(
  analysis::power::handle &a
, analysis::power::handle &b
) {
  a.swap( b );
}

} // namespace std

#endif /* POWER_HPP_ */
