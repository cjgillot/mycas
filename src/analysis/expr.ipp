#ifndef EXPR_IPP
#define EXPR_IPP

#include "analysis/expr.hpp"
#include "analysis/numeric.hpp"

namespace analysis {

// cdtor
inline void expr::swap(expr &o)
{ m_impl.swap(o.m_impl); }

inline expr::~expr() {}

inline expr::expr( const basic* bp )
: m_impl( bp ) {}

// has
inline bool expr::has(const symbol &s) const
{ return m_impl->has(s); }

// expand
inline expr expr::expand() const
{ return m_impl->expand(); }

// printing
inline void expr::print(std::basic_ostream<char> &os) const
{ m_impl->print(os); }

// pointer access
inline const basic* expr::get() const
{ return m_impl.get(); }

// RTTI
inline bool expr::is_numeric() const
{ return m_impl->is_numeric(); }

template<class T>
inline bool expr::is_a() const
{ return rtti::is_a<T>( get() ); }

template<>
inline bool expr::is_a< numeric >() const
{ return is_numeric(); }

template<class T>
inline bool expr::is_exactly_a() const
{ return rtti::is_exactly_a<T>( get() ); }

template<class T>
inline const T* expr::as_a() const
{ ASSERT( is_a<T>() ); return static_cast<const T*>( get() ); }

// hash
inline std::size_t expr::hash() const
{ return m_impl->hash(); }

// IO
inline std::basic_ostream<char> &
operator<<(std::basic_ostream<char> &os, const expr &e)
{ e.print(os); return os; }

// pow
inline expr
pow(const expr &a, const expr &b)
{ return a.pow(b); }

}

#endif
