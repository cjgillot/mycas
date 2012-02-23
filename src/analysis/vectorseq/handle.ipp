#ifndef EXPAIRSEQ_HANDLE_IPP
#define EXPAIRSEQ_HANDLE_IPP

#include "analysis/vectorseq/handle.hpp"

#include "analysis/expr.hpp"

#include "util/assert.hpp"
#include "util/move.hpp"

namespace analysis {
namespace vseq {

#define HNDL handle<I,M>
#define PTR  typename HNDL::const_pointer

// operations
template<class I, class M>
inline PTR
HNDL::add(PTR a, PTR b)
{
  ASSERT( handle::compare( a, b ) == 0 );
  return handle::chg_coef( a, a->coef() + b->coef() );
}

template<class I, class M>
inline PTR
HNDL::sub(PTR a, PTR b)
{
  ASSERT( handle::compare( a, b ) == 0 );
  return handle::chg_coef( a, a->coef() - b->coef() );
}

template<class I, class M>
inline PTR
HNDL::neg(PTR p)
{
  return handle::chg_coef( p, -p->coef() );
}
template<class I, class M>
inline PTR
HNDL::sca(PTR p, const number &n)
{
  return handle::chg_coef( p, n * p->coef() );
}

template<class I, class M>
inline PTR
HNDL::mul(PTR a, PTR b)
{
  util::scoped_ptr<I> ret ( I::from_number( a->coef() * b->coef() ) );
  ret->construct_add( *a, *b );
  return ret.release();
}

// tests & comparison
template<class I, class M>
inline bool
HNDL::null(PTR p)
{ return p->coef().null(); }

template<class I, class M>
inline util::cmp_t
HNDL::compare(PTR a, PTR b)
{ return a->partial_compare( *b ); }

template<class I, class M>
inline util::cmp_t
HNDL::deep_compare(PTR a, PTR b)
{ return basic::compare( *a, *b ); }

template<class I, class M>
inline std::size_t
HNDL::hash(PTR p)
{ return p->hash(); }

template<class I, class M>
inline std::size_t
HNDL::coef_hash(PTR p)
{ return p->coef_hash(); }

template<class I, class M>
inline std::size_t
HNDL::value_hash(PTR p)
{ return p->value_hash(); }

template<class I, class M>
inline void
HNDL::sort_hash(PTR p, std::size_t &high, std::size_t &low)
{ return p->sort_hash( high, low ); }

// printing
template<class I, class M>
template<class S>
inline void
HNDL::print(PTR p, S &os)
{ p->print(os); }

// operations implementation
template<class I, class M>
inline PTR
HNDL::chg_coef(PTR p, const number &n) throw()
{
  util::scoped_ptr< eps_t > retp ( new I( *p ) );
  retp->coef() = n;
  return static_cast<PTR>( retp.release() );
}

#undef PTR
#undef HNDL

}} // namespace analysis::vseq

#endif
