#ifndef ANALYSIS_MATCH_HPP
#define ANALYSIS_MATCH_HPP

#include "analysis/basic.hpp"
#include "analysis/symbol.hpp"

#include "analysis/match_state.hpp"

namespace analysis {

class wildcard;

class wildcard_
: public basic
{
  REGISTER_FINAL( wildcard_, basic )

private:
  wildcard_(unsigned i)
  : m_id( i ) {}

  friend class wildcard;

public:
  ~wildcard_() {}

  inline unsigned id() const
  { return m_id; }

  wildcard_* clone() const
  { return const_cast<wildcard_*>( this ); }

  std::size_t hash() const
  { return m_id; }

  void print(std::basic_ostream<char> &os) const
  { os << '$' << m_id; }

private:
  expr differentiate(const symbol &) const
  { return number::zero(); }

  util::cmp_t compare_same_type(const basic &o_) const
  {
    const wildcard_ &o = static_cast<const wildcard_&>( o_ );
    return util::compare( m_id, o.m_id );
  }

private:
  unsigned m_id;
};

class wildcard
: public expr
{
public:
  wildcard(unsigned i)
  : expr( new wildcard_( i ) ) {}

  ~wildcard() {}

  void swap(wildcard &o)
  { expr::swap( o ); }

  unsigned id() const
  { return get()->id(); }

  const wildcard_* get() const
  { return static_cast<const wildcard_*>( expr::get() ); }

  std::size_t hash() const
  { return get()->wildcard_::hash(); }

  static util::cmp_t compare(const wildcard &a, const wildcard &b)
  {
    // equivalent to the hash comparison in this case
    return a.get()->wildcard_::compare_same_type( *b.get() );
  }
};

}

#endif
