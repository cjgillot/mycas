#ifndef EXPR_IPP
#define EXPR_IPP

#include "analysis/basic/expr.hpp"

namespace analysis {

// cdtor
//@{
//!\brief Default ctor : zero
inline expr::expr()
: m_impl( small_numeric_cache(0) ) {}

//!\brief Copy ctor
inline expr::expr(const expr &o)
: m_impl( o.m_impl ) {}
//!\brief Assignment operator
inline expr& expr::operator=(const expr &o)
{ m_impl = o.m_impl; return *this; }

//!\brief Non-throwing swap
inline void expr::swap(expr &o)
{ m_impl.swap(o.m_impl); }

//!\brief Dtor
inline expr::~expr() {}

//! \brief Explicit creation from \c basic instance
//! \param b a non-null pointer to a \c basic instance
inline expr::expr( const basic* bp )
: m_impl( bp ) { ASSERT( bp ); eval(); }

//! \brief Creation from \ref ptr instance
//! \param bp a non-null \c ptr to a \c basic instance
template<class T>
inline expr::expr( const ptr<T> &bp )
: m_impl( bp ) { ASSERT( bp ); eval(); }

//! \brief Creation from \ref number
inline expr::expr( const number &n )
: m_impl( new numerical( n ) ) {}

//! \name Arithmetic ctor
//! \brief Creation from fondamental arithmetic types
//!
//! Integer construction use a cache for small integer, with absolute
//! value below \c EXPR_NUMERIC_CACHE_LIMIT
//! \param n : a number
//! \{
//@{
#define INTEGER_CTOR( type )                \
inline expr::expr(signed type n)            \
: m_impl( /* null */ ) {                    \
  if( (unsigned long)std::abs(n)            \
    <= EXPR_NUMERIC_CACHE_LIMIT )          \
    m_impl.reset( small_numeric_cache(n) ); \
  else                                      \
    m_impl.reset( new numerical( n ) );     \
}
INTEGER_CTOR(int)
INTEGER_CTOR(long)
// INTEGER_CTOR(long long)
#undef INTEGER_CTOR

#define UNSIGNED_CTOR( type )               \
inline expr::expr(unsigned type n)          \
: m_impl( /* null */ ) {                    \
  if( n <= EXPR_NUMERIC_CACHE_LIMIT )      \
    m_impl.reset( small_numeric_cache(n) ); \
  else                                      \
    m_impl.reset( new numerical( n ) );     \
}
UNSIGNED_CTOR(int)
UNSIGNED_CTOR(long)
// UNSIGNED_CTOR(long long)
#undef UNSIGNED_CTOR

#define FLOAT_CTOR( type )      \
inline expr::expr(type n)       \
: m_impl( new numerical( n ) )  \
{}
FLOAT_CTOR(float) FLOAT_CTOR(double) FLOAT_CTOR(long double)
#undef FLOAT_CTOR
//@}
//! \}

//@}

//!\name Basic methods forwarding
//!\{
//@{
//!\brief Null test
inline bool expr::null() const
{ return m_impl->null(); }
//!\brief Unit test
inline bool expr::unit() const
{ return m_impl->unit(); }

//! \brief Evaluation
//! \see basic::eval
inline void expr::eval(unsigned lv) const
{
  ++lv;
  while( lv && ! m_impl->is_evaluated() )
    m_impl->eval(--lv).m_impl.swap( m_impl );
  ASSERT( m_impl->is_evaluated() );
}

//! \brief Symbol dependency test
//! \see basic::has
inline bool expr::has(const symbol &s) const
{ return m_impl->has(s); }

//! \brief Differentiation
//! \see basic::diff
inline expr expr::diff(const symbol &s, unsigned n) const
{ return m_impl->diff(s,n); }

//! \brief Expansion
//! \see basic::expand
inline expr expr::expand() const
{
  if( m_impl->is_expanded() )
    return *this;
  return m_impl->expand();
}

//! \brief Power series expansion
//! \see basic::series
inline expr expr::series(const symbol &s) const
{ return m_impl->series(s); }

//! \brief Pattern substitution
//! \see basic::subs
inline expr expr::subs(const std::map<expr,expr> &map) const
{ return m_impl->subs( map ); }

//! \brief Pattern matching
//! \see basic::match
inline bool expr::match(const expr &e, match_state &mm) const
{ return m_impl->match(e, mm); }

//! \brief Printing
//! \see basic::print
inline void expr::print(std::basic_ostream<char> &os) const
{ m_impl->print(os); }

//! \brief Hashing
//! \see basic::hash
inline std::size_t expr::hash() const
{ return m_impl->hash(); }
//@}
//!\}

//!\name Runtime typing support
//!\{
//@{
//!\brief Type test
template<class T>
inline bool expr::is_a() const
{ return rtti::is_a<T>( get() ); }

//!\brief Number test (special case)
inline bool expr::is_numerical() const
{ return m_impl->is_numerical(); }

template<>
inline bool expr::is_a< numerical >() const
{ return is_numerical(); }

//!\brief Strong type test
template<class T>
inline bool expr::is_exactly_a() const
{ return rtti::is_exactly_a<T>( get() ); }

//!\brief Coercion
template<class T>
inline const T* expr::as_a() const
{ ASSERT( is_a<T>() ); return static_cast<const T*>( get() ); }

template<>
inline const basic* expr::as_a<basic>() const
{ return get(); }

//!\brief Visitor class acceptation
template<class V>
inline void expr::accept(const V &visitor) const
{ m_impl->accept( visitor ); }
//!\brief Non-const visitor class acceptation
template<class V>
inline void expr::accept(V &visitor) const
{ m_impl->accept( visitor ); }
//@}
//!\}

}

#endif
