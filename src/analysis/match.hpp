#ifndef ANALYSIS_MATCH_HPP
#define ANALYSIS_MATCH_HPP

#include "analysis/expr.hpp"
#include "analysis/basic.hpp"
#include "analysis/symbol.hpp"
#include "analysis/numerical.hpp"

namespace analysis {

class wildcard;

namespace match_detail {

class wildcard_base
: public basic
{
  REGISTER_CLASS( wildcard_base, basic )

protected:
  wildcard_base(unsigned i)
  : m_id( i ) {}

  friend class analysis::wildcard;

public:
  ~wildcard_base() {}

  inline unsigned id() const
  { return m_id; }

  wildcard_base* clone() const
  { return const_cast<wildcard_base*>( this ); }

  std::size_t hash() const
  { return m_id; }

  void print(std::basic_ostream<char> &os) const
  { os << "$*" << m_id; }

  expr subs(const exmap &m) const;

private:
  expr differentiate(const symbol &) const
  { return numerical::zero(); }

  util::cmp_t compare_same_type(const basic &o_) const
  {
    const wildcard_base &o = static_cast<const wildcard_base&>( o_ );
    return util::compare( m_id, o.m_id );
  }

private:
  unsigned m_id;
};

class wildcard_
: public wildcard_base
{
  REGISTER_FINAL( wildcard_, wildcard_base )

private:
  wildcard_(unsigned i)
  : wildcard_base( i ) {}

  friend class analysis::wildcard;

public:
  ~wildcard_() {}

  using wildcard_base::id;

  wildcard_* clone() const
  { return const_cast<wildcard_*>( this ); }

  void print(std::basic_ostream<char> &os) const
  { os << "$&" << id(); }
};

} // namespace match_detail

class wildcard
: public expr
{
public:
  wildcard(unsigned i)
  : expr( new match_detail::wildcard_( i ) ) {}

  struct unbound_tag {};
  wildcard(unsigned i, unbound_tag)
  : expr( new match_detail::wildcard_base( i ) ) {}

  ~wildcard() {}

  void swap(wildcard &o)
  { expr::swap( o ); }

  unsigned id() const
  { return get()->id(); }

  const match_detail::wildcard_base* get() const
  { return static_cast<const match_detail::wildcard_base*>( expr::get() ); }

  wildcard operator*() const
  { return wildcard( id(), unbound_tag() ); }

  std::size_t hash() const
  { return get()->wildcard_base::hash(); }

  static util::cmp_t compare(const wildcard &a, const wildcard &b)
  {
    // equivalent to the hash comparison in this case
    return a.get()->wildcard_base::compare_same_type( *b.get() );
  }
};

namespace match_detail {

inline expr wildcard_base::subs( const exmap &m ) const
{
  exmap::const_iterator it = m.find( this );
  if( it != m.end() )
    return it->second;
  return this;
}

} // namespace match_detail

template<>
inline bool expr::is_a<wildcard>() const
{ return rtti::is_a<match_detail::wildcard_>( get() ); }

template<>
inline bool expr::is_exactly_a<wildcard>() const
{ return rtti::is_exactly_a<match_detail::wildcard_>( get() ); }

} // namespace analysis

#include "analysis/match_state.hpp"

#endif
