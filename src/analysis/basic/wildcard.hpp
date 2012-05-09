#ifndef ANALYSIS_MATCH_HPP
#define ANALYSIS_MATCH_HPP

#include "analysis/expr.hpp"
#include "analysis/symbol.hpp"

namespace analysis {

class wildcard;

namespace match_detail {

class wildcard_base
: public basic
{
  REGISTER_CLASS( wildcard_base, basic )
  friend class analysis::wildcard;

private:
  unsigned m_id;

protected:
  wildcard_base(unsigned);

public:
  virtual ~wildcard_base();

  wildcard_base* clone() const;

public:
  unsigned id() const;
  std::size_t hash() const;

  expr subs(const exmap &m) const;
  void print(std::basic_ostream<char> &os) const;

private:
  expr differentiate(const symbol &) const;
  util::cmp_t compare_same_type(const basic &o_) const;
};

class wildcard_ final
: public wildcard_base
{
  REGISTER_FINAL( wildcard_, wildcard_base )

private:
  wildcard_(unsigned);

  friend class analysis::wildcard;

public:
  ~wildcard_();

  using wildcard_base::id;
  using wildcard_base::clone;

  void print(std::basic_ostream<char> &os) const;
};

} // namespace match_detail

class wildcard
: public expr
{
public:
  wildcard(unsigned);

  struct unbound_tag {};
  wildcard(unsigned, unbound_tag);

  void swap(wildcard &o);

  ~wildcard();

public:
  unsigned id() const;
  const match_detail::wildcard_base* get() const;

  wildcard unbind() const;

public:
  std::size_t hash() const;
  static util::cmp_t compare(const wildcard&, const wildcard&);
};

template<>
inline bool expr::is_a<wildcard>() const
{ return rtti::is_a<match_detail::wildcard_>( get() ); }

template<>
inline bool expr::is_exactly_a<wildcard>() const
{ return rtti::is_exactly_a<match_detail::wildcard_>( get() ); }

} // namespace analysis

#endif

#include "wildcard.ipp"
