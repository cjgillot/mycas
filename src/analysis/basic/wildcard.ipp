#ifndef ANALYSIS_MATCH_IPP
#define ANALYSIS_MATCH_IPP

#include "analysis/expr.hpp"
#include "analysis/symbol.hpp"

#include "wildcard.hpp"

namespace analysis {
namespace match_detail {

// wildcard_base
inline
wildcard_base::wildcard_base(unsigned i)
: m_id( i ) {}

inline
wildcard_base::~wildcard_base()
{}

inline unsigned
wildcard_base::id() const
{ return m_id; }

inline std::size_t
wildcard_base::hash() const
{ return id(); }

// wildcard_
inline
wildcard_::wildcard_(unsigned i)
: wildcard_base( i ) {}

inline
wildcard_::~wildcard_() {}

} // namespace match_detail

inline
wildcard::wildcard(unsigned i)
: expr( new match_detail::wildcard_(i) ) {}

inline
wildcard::wildcard(unsigned i, unbound_tag)
: expr( new match_detail::wildcard_base(i) ) {}

inline
wildcard::~wildcard() {}

inline void
wildcard::swap(wildcard &o)
{ expr::swap( o ); }

inline unsigned
wildcard::id() const
{ return get()->id(); }

inline const match_detail::wildcard_base*
wildcard::get() const
{ return static_cast<const match_detail::wildcard_base*>( expr::get() ); }

inline wildcard
wildcard::unbind() const
{ return wildcard( id(), unbound_tag() ); }

inline std::size_t
wildcard::hash() const
{ return get()->wildcard_base::hash(); }

inline util::cmp_t
wildcard::compare(const wildcard &a, const wildcard &b)
{
  // equivalent to the hash comparison in this case
  return util::compare( a.id(), b.id() );
}

} // namespace analysis

#endif
