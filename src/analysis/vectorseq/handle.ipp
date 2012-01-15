#ifndef EXPAIRSEQ_HANDLE_IPP
#define EXPAIRSEQ_HANDLE_IPP

#include "analysis/vectorseq/handle.hpp"
#include "analysis/basic.hpp"
#include "analysis/expr.hpp"

#include "util/assert.hpp"
#include "util/move.hpp"

namespace analysis {
namespace epseq {

// cdtor
template<class I, class M>
inline handle<I,M>::
handle(const eps_t* p)
: m_ptr(p) { ASSERT(p); }

template<class I, class M>
inline handle<I,M>::
handle(const handle &o)
: m_ptr(o.m_ptr) {}

template<class I, class M>
inline handle<I,M> &handle<I,M>::
operator=(const handle &o)
{
  m_ptr = o.m_ptr;
  return *this;
}

template<class I, class M>
inline handle<I,M>::
~handle() throw() {}

template<class I, class M>
inline void handle<I,M>::
swap(handle &o) throw()
{ m_ptr.swap( o.m_ptr ); }

// coercion
template<class I, class M>
handle<I,M>::handle( const expr &e )
: m_ptr( from_expr( e ) ) {}

template<class I, class M>
inline
handle<I,M>::operator expr() const
{ return expr(m_ptr.get()); }

// operations
template<class I, class M>
inline handle<I,M>
handle<I,M>::operator+(const handle &o) const {
  ASSERT(compare(*this, o) == 0);
  return chg_coef( m_ptr->coef() + o.m_ptr->coef() );
}

template<class I, class M>
inline handle<I,M>
handle<I,M>::operator-(const handle &o) const {
  ASSERT(compare(*this, o) == 0);
  return chg_coef( m_ptr->coef() - o.m_ptr->coef() );
}
template<class I, class M>
inline handle<I,M>
handle<I,M>::operator-() const {
  return chg_coef( - m_ptr->coef() );
}
template<class I, class M>
inline handle<I,M>
handle<I,M>::sca(const number &n) const {
  return chg_coef( n * m_ptr->coef() );
}

template<class I, class M>
inline handle<I,M>
handle<I,M>::operator*(const handle &o) const
{
  util::scoped_ptr<I> ret ( I::from_number( m_ptr->coef() * o.m_ptr->coef() ) );
  ret->construct_add( *m_ptr, *o.m_ptr );
  return ret.release();
}

// tests & comparison
template<class I, class M>
inline bool
handle<I,M>::null() const
{ return m_ptr->coef().null(); }

template<class I, class M>
inline util::cmp_t
handle<I,M>::compare(const handle &a, const handle &b)
{ return a.m_ptr->partial_compare(*b.m_ptr); }

template<class I, class M>
inline util::cmp_t
handle<I,M>::deep_compare(const handle &a, const handle &b)
{ return basic::compare(*a.m_ptr, *b.m_ptr); }

template<class I, class M>
inline std::size_t
handle<I,M>::hash() const
{ return m_ptr->hash(); }

template<class I, class M>
inline std::size_t
handle<I,M>::coef_hash() const
{ return m_ptr->coef_hash(); }

template<class I, class M>
inline std::size_t
handle<I,M>::value_hash() const
{ return m_ptr->value_hash(); }

template<class I, class M>
inline void
handle<I,M>::sort_hash(std::size_t &high, std::size_t &low) const
{ return m_ptr->sort_hash( high, low ); }

// printing
template<class I, class M>
template<class S>
inline void
handle<I,M>::print(S &os) const
{ m_ptr->print(os); }

template<class I, class M>
inline typename handle<I,M>::const_pointer
handle<I,M>::get() const throw()
{ return static_cast< const_pointer >( m_ptr.get() ); }

// operations implementation
template<class I, class M>
inline typename handle<I,M>::eps_t*
handle<I,M>::chg_coef(const number &n) const throw()
{
  util::scoped_ptr< eps_t > retp ( new I( *get() ) );
  retp->coef() = n;
  return retp.release();
}

}} // namespace analysis::epseq

#endif
