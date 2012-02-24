#ifndef EXPR_IPP
#define EXPR_IPP

#include "analysis/basic/expr.hpp"

namespace analysis {

// cdtor
inline expr::expr()
: m_impl( numerical::zero() ) {}

inline expr::expr(const expr &o)
: m_impl( o.m_impl ) {}
inline expr& expr::operator=(const expr &o)
{ m_impl = o.m_impl; return *this; }

inline void expr::swap(expr &o)
{ m_impl.swap(o.m_impl); }

inline expr::~expr() {}

inline expr::expr( const basic* bp )
: m_impl( bp ) { ASSERT( bp ); eval(); }

template<class T>
inline expr::expr( const ptr<T> &bp )
: m_impl( bp ) { ASSERT( bp ); eval(); }

inline expr::expr( const number &n )
: m_impl( new numerical( n ) ) {}

#define INTEGER_CTOR( type )                \
inline expr::expr(signed type n)            \
: m_impl( /* null */ ) {                    \
  if( (unsigned long)std::abs(n)            \
    <= small_numeric_cache_limit )          \
    m_impl.reset( small_numeric_cache(n) ); \
  else                                      \
    m_impl.reset( new numerical( n ) );     \
}
INTEGER_CTOR(int) INTEGER_CTOR(long) // INTEGER_CTOR(long long)
#undef INTEGER_CTOR

#define UNSIGNED_CTOR( type )               \
inline expr::expr(unsigned type n)          \
: m_impl( /* null */ ) {                    \
  if( n <= small_numeric_cache_limit )      \
    m_impl.reset( small_numeric_cache(n) ); \
  else                                      \
    m_impl.reset( new numerical( n ) );     \
}
UNSIGNED_CTOR(int) UNSIGNED_CTOR(long) // UNSIGNED_CTOR(long long)
#undef UNSIGNED_CTOR

#define FLOAT_CTOR( type )      \
inline expr::expr(type n)       \
: m_impl( new numerical( n ) )  \
{}
FLOAT_CTOR(float) FLOAT_CTOR(double) FLOAT_CTOR(long double)
#undef FLOAT_CTOR

// eval
inline void expr::eval(unsigned lv) const
{
  ++lv;
  while( lv && ! m_impl->is_evaluated() )
    m_impl->eval(--lv).m_impl.swap( m_impl );
  ASSERT( m_impl->is_evaluated() );
}

// accept
template<class V>
inline void expr::accept(const V &visitor) const
{ m_impl->accept( visitor ); }
template<class V>
inline void expr::accept(V &visitor) const
{ m_impl->accept( visitor ); }

// has
inline bool expr::has(const symbol &s) const
{ return m_impl->has(s); }

inline bool expr::match(const expr &e, match_state &mm) const
{ return m_impl->match(e, mm); }

// diff
inline expr expr::diff(const symbol &s, unsigned n) const
{ return m_impl->diff(s,n); }

// expand
inline expr expr::expand() const
{
  if( m_impl->is_expanded() )
    return *this;
  return m_impl->expand();
}

// subs
inline expr expr::subs(const std::map<expr,expr> &map) const
{ return m_impl->subs( map ); }

// series
inline expr expr::series(const symbol &s) const
{ return m_impl->series(s); }

// printing
inline void expr::print(std::basic_ostream<char> &os) const
{ m_impl->print(os); }

// RTTI
inline bool expr::is_numerical() const
{ return m_impl->is_numerical(); }

template<class T>
inline bool expr::is_a() const
{ return rtti::is_a<T>( get() ); }

template<>
inline bool expr::is_a< numerical >() const
{ return is_numerical(); }

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
