#ifndef ANALYSIS_FUNCTION_EXPRSEQ_HPP
#define ANALYSIS_FUNCTION_EXPRSEQ_HPP

#include "analysis/basic.hpp"

#include "util/foreach.hpp"
#include "util/assert.hpp"

#include "algebra/compare.hpp"

namespace analysis {

template<class Cont>
class exprseq
: public basic
{
  Cont m_container;

protected:
  typedef Cont container_type;
  typedef exprseq exprseq_;

protected:
  template<class A1>
  exprseq(const A1 &a1)
  : m_container( a1 ) {}
  template<class A1, class A2>
  exprseq(const A1 &a1, const A2 &a2)
  : m_container( a1, a2 ) {}
  template<class A1, class A2, class A3>
  exprseq(const A1 &a1, const A2 &a2, const A3 &a3)
  : m_container( a1, a2, a3 ) {}

  // default copy ctor

  // default dtor

public: // range
  typedef typename container_type::iterator               iterator;
  typedef typename container_type::const_iterator         const_iterator;
  typedef typename container_type::reverse_iterator       reverse_iterator;
  typedef typename container_type::const_reverse_iterator const_reverse_iterator;

  iterator begin() { return m_container.begin(); }
  iterator end  () { return m_container.end  (); }

  const_iterator begin() const { return m_container.begin(); }
  const_iterator end  () const { return m_container.end  (); }

  reverse_iterator rbegin() { return m_container.rbegin(); }
  reverse_iterator rend  () { return m_container.rend  (); }

  const_reverse_iterator rbegin() const { return m_container.rbegin(); }
  const_reverse_iterator rend  () const { return m_container.rend  (); }

  std::size_t size()  const { return m_container.size(); }
  bool        empty() const { return m_container.empty(); }

  typename container_type::reference       operator[](std::size_t i)       { return m_container[i]; }
  typename container_type::const_reference operator[](std::size_t i) const { return m_container[i]; }

  typename container_type::reference       at(std::size_t i)       { return m_container.at(i); }
  typename container_type::const_reference at(std::size_t i) const { return m_container.at(i); }

protected:
  std::size_t hash() const
  {
    std::size_t seed = 0u;
    foreach( const expr &a, m_container )
      boost::hash_combine( seed, a.hash() );
    return seed;
  }

  util::cmp_t compare_same_type(const basic &o_) const
  {
    const exprseq &o = static_cast<const exprseq&>(o_);

    return algebra::range_compare( m_container, o.m_container, expr::compare );
  }

  bool has(const symbol &s) const
  {
    foreach(const expr &e, m_container)
      if( e.has(s) )
        return true;
  }

  void print_children(std::basic_ostream<char> &os) const
  {
    os << '(';
    if( size() > 0 )
    {
      const_iterator
        it = begin()
      , en = end();

      os << *it;
      while( ++it != en )
        os << ' ' << *it;
    }
    os << ')';
  }

  expr subs(const exmap &) const;
};

} // namespace analysis

#endif
