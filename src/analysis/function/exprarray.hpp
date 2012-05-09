#ifndef ANALYSIS_FUNCTION_EXPRARRAY_HPP
#define ANALYSIS_FUNCTION_EXPRARRAY_HPP

#include "analysis/expr.hpp"

namespace analysis {

// thin class for visitor use
class exprarray
: public basic
{
  REGISTER_CLASS( exprarray, basic )

protected:
  exprarray() = default;
  exprarray(const exprarray&) = default;

public:
  ~exprarray() = default;

public: // range
  virtual       expr* pbegin() = 0;
  virtual       expr* pend  () = 0;

  virtual const expr* pbegin() const = 0;
  virtual const expr* pend  () const = 0;

  std::size_t size()  const { return std::distance( pbegin(), pend() ); }
  bool        empty() const { return pbegin() == pend(); }

        expr& operator[](std::size_t i)       { return pbegin()[i]; }
  const expr& operator[](std::size_t i) const { return pbegin()[i]; }

        expr& at(std::size_t i)       { ASSERT( i < size() ); return pbegin()[i]; }
  const expr& at(std::size_t i) const { ASSERT( i < size() ); return pbegin()[i]; }

protected:
  std::size_t hash() const;

  util::cmp_t compare_same_type(const basic &o_) const;

  bool has(const symbol &s) const;

  void print_children(std::basic_ostream<char> &os) const;

  bool match_same_type(const basic &, match_state &) const;
  expr subs(const exmap &) const;
};

} // namespace analysis

#endif
