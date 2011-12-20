#ifndef ANALYSIS_FUNCTION_DERIVATIVE_HPP
#define ANALYSIS_FUNCTION_DERIVATIVE_HPP

#include "analysis/function/function.hpp"

#include "util/assert.hpp"

#include <map>

namespace analysis {

template<unsigned N>
class derivative
: public function<N>
{
private:
  typedef derivative<N> self;
  typedef function<N> super;

public:
  REGISTER_FINAL( self, super )

private:
  typedef std::map<unsigned, unsigned> diff_map;
  using super::m_name;

public:
  typedef typename super::iterator_tag iterator_tag;

  template<class InputIterator>
  derivative(
    const InputIterator &b
  , const InputIterator &e
  , const diff_map &ds
  , const symbol &n
  , iterator_tag
  )
  : super( n, b, e, iterator_tag() )
  , m_map(ds) {}

#define CT_ARGS( z, n, data )  \
  , BOOST_PP_CAT( arg, n )

#define ARGS( z, n, data )  \
  , const expr &BOOST_PP_CAT( arg, n )

#define CT_ONE( z, n, data )  \
  op_t::construct( n, this->super::get_container(), arg##n );

#define CTOR( z, n, data )  \
  template<class Dummy>     \
  derivative( const Dummy &name, const diff_map &ds BOOST_PP_REPEAT_##z( n, ARGS, data ) ) \
  : super( name BOOST_PP_REPEAT_##z( n, CT_ARGS, data ) ) \
  , m_map(ds) {}

  BOOST_PP_REPEAT_FROM_TO( 1, 20, CTOR, () )

#undef CTOR
#undef CT_ONE
#undef ARGS
#undef CT_ARGS

private:
  derivative* clone() const
  { return new derivative( *this ); }

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

    util::cmp_t c = symbol::compare( m_name, o.m_name );
    if(c) return c;

    c = util::compare( m_map.size(), o.m_map.size() );
    if(c) return c;

    { // prefer do this here, since expr::compare can recurse
      typedef typename diff_map::const_iterator iter_t;

      const iter_t &e1 =   m_map.end();

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

    return super::compare_same_type(o);
  }

  void print(std::basic_ostream<char> &os) const
  {
    os << '(' << "D[";
    typedef std::pair<unsigned, unsigned> pair_t;
    foreach( const pair_t &d, m_map )
      os << d.first << '@' << d.second << ' ';
    os << m_name << "]" << ' ';
    super::print_children(os);
    os << ')';
  }

private:
  diff_map m_map;

  friend class function<N>;
};

template<unsigned N>
expr function<N>::differentiate(const symbol &s) const
{
  typedef derivative<N> fd_t;
  typedef typename fd_t::diff_map dm_t;

  typedef typename super::const_iterator iter_t;

  const iter_t &be = super::begin();
  const iter_t &en = super::end();

  iter_t it = be;

  expr ret ( number::zero() );

  for(unsigned n = 0; it != en; ++it, ++n)
  {
    const expr &dit = it->diff(s);

    if( dit.null() ) continue;

    const ptr<fd_t> &fd = new fd_t( be, en, dm_t(), m_name, iterator_tag() );
    ++fd->m_map[n];
    ret += dit * fd;
  }

  return ret;
}

template<unsigned N>
expr derivative<N>::differentiate(const symbol &s) const
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

    const ptr<derivative> &fd = new derivative( be, en, m_map, m_name, iterator_tag() );
    ++fd->m_map[n];
    ret += dit * expr(fd);
  }

  return ret;
}

} // namespace analysis

#endif
