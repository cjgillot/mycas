#include "flint.hpp"

#include "number.hpp"

using numeric::number;
using numeric::detail::wrapper;

namespace {

struct hash_visitor
: public boost::static_visitor<size_t>
{
  template<class T>
  size_t operator()(const wrapper<T> &c) const
  { return hash( *c ); }
};

struct cmp_visitor
: public boost::static_visitor<util::cmp_t>
{
  template<class T, class U>
  util::cmp_t operator()(const wrapper<T> &a, const wrapper<U> &b) const
  { return cmp( *a, *b ); }
};

}

size_t
number::hash() const
{ return boost::apply_visitor( hash_visitor(), m_impl ); }

util::cmp_t
number::compare(const number &a, const number &b)
{ return boost::apply_visitor( cmp_visitor(), a.m_impl, b.m_impl ); }
