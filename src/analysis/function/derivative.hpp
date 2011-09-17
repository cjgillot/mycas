#ifndef ANALYSIS_FUNCTION_DERIVATIVE_HPP
#define ANALYSIS_FUNCTION_DERIVATIVE_HPP

#include "analysis/function/function.hpp"

#include "util/assert.hpp"

#include <map>

namespace analysis {

template<class Func, unsigned N>
class derivative
: public function<
    derivative<Func,N>
  , N
  >
{
private:
  typedef derivative<Func,N> self;
  typedef function<self,N> super;

public:
  REGISTER_FINAL( self, super )

private:
  typedef std::map<unsigned, unsigned> diff_map;

public:
  template<class InputIterator>
  derivative(
    const InputIterator &b
  , const InputIterator &e
  , const diff_map &ds
  )
  : super( b, e ), m_map(ds) {}

private:
  derivative* clone() const
  { return new derivative(this); }

private:
  expr differentiate(const symbol &s) const;
  std::size_t hash() const
  {
    std::size_t seed = super::hash();
    boost::hash_combine(seed, m_map.size());
    boost::hash_range(seed, m_map.begin(), m_map.end());
    return seed;
  }
  util::cmp_t compare_same_type(const basic &o_) const
  {
    const derivative &o = static_cast<const derivative&>(o_);

    util::cmp_t c = symbol::compare(m_name, o.m_name);
    if(c) return c;

    c = util::compare(m_map.size(), o.m_map.size());
    if(c) return c;

    { // prefer do this here, since expr::compare can recurse
      typedef typename diff_map::const_iterator iter_t;

      const iter_t &e1 =   m_map.end();
      const iter_t &e2 = o.m_map.end();

      iter_t i1 =   m_map.begin();
      iter_t i2 = o.m_map.begin();

      for(; i1 != e1; ++i1, ++i2)
      {
        c = util::compare(i1->first,  i2->first);
        if(c) return c;
        c = util::compare(i1->second, i2->second);
        if(c) return c;
      }
    }

    return expr::compare(m_arg, o.m_arg);
  }

private:
  diff_map m_map;

  friend expr function<D,N>::differentiate(const symbol &s) const;
};

template<class D, unsigned N>
expr function<D,N>::differentiate(const symbol &s) const
{
  typedef derivative<D,N> fd_t;
  typedef fd_t::diff_map dm_t;

  typedef typename super::const_iterator iter_t;

  const iter_t &be = super::begin();
  const iter_t &en = super::end();

  iter_t it = be;

  expr ret ( number::zero() );

  for(unsigned n = 0; it != en; ++it, ++n)
  {
    const expr &dit = it->diff(s);

    if( dit.null() ) continue;

    const expr &fd = new fd_t( be, en, dm_t(1, std::make_pair(n,0)) );
    ret += dit * fd;
  }

  return ret;
}

template<class D, unsigned N>
expr derivative<D,N>::differentiate(const symbol &s) const
{
  typedef typename super::const_iterator iter_t;

  const iter_t &be = super::begin();
  const iter_t &en = super::end();

  iter_t it = be;

  expr ret ( number::zero() );

  for(unsigned n = 0; it != en; ++it, ++n)
  {
    const expr &dit = it->diff(s);

    if( dit.null() ) continue;

    const ptr<derivative> &fd = new derivative( be, en, m_map );
    ++fd->m_map[n];
    ret += dit * expr(fd);
  }

  return ret;
}

template<class Func>
class derivative
: public function<
    derivative<Func,1>
  , 1
  >
{
private:
  typedef derivative<Func,1> self;
  typedef function<self,1> super;

public:
  REGISTER_FINAL( self, super )

public:
  template<class InputIterator>
  derivative(const expr &arg, const unsigned n)
  : super( arg ), m_order(n) {}

private:
  derivative* clone() const
  { return new derivative(this); }

private:
  expr differentiate(const symbol &s) const;
  std::size_t hash() const
  {
    std::size_t seed = super::hash();
    boost::hash_combine(seed, m_order);
    return seed;
  }
  util::cmp_t compare_same_type(const basic &o_) const
  {
    const derivative &o = static_cast<const derivative&>(o_);

    util::cmp_t c = util::compare(m_order, o.m_order);
    if(c) return c;

    return super::compare_same_type(o_);
  }

private:
  unsigned m_order;

  friend expr function<D,1>::differentiate(const symbol &s) const;
};

template<class D>
expr function<D,1>::differentiate(const symbol &s) const
{
  typedef derivative<D,N> fd_t;

  typedef typename super::const_iterator iter_t;

  const iter_t &be = super::begin();
  const iter_t &en = super::end();

  iter_t it = be;

  const expr &da = arg<0>().diff(s);

  if( dit.null() )
    return number::zero();

  const expr &fd = new fd_t( arg, 1 );
  return da * fd;
}

template<class D>
expr derivative<D,1>::differentiate(const symbol &s) const
{
  typedef derivative<D,N> fd_t;

  typedef typename super::const_iterator iter_t;

  const iter_t &be = super::begin();
  const iter_t &en = super::end();

  iter_t it = be;

  const expr &da = super::arg<0>().diff(s);

  if( dit.null() )
    return number::zero();

  const expr &fd = new fd_t( arg, 1 + m_order );
  return da * fd;
}

} // namespace analysis

#endif
