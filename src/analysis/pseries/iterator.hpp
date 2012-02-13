#ifndef PSERIES_ITERATOR_HPP
#define PSERIES_ITERATOR_HPP

#include "analysis/pseries/pseries.hpp"

#include "analysis/expr.hpp"

#include <boost/iterator/iterator_facade.hpp>

namespace analysis {

class expr;

namespace pseries_detail {

class iterator
: public boost::iterator_facade<
    iterator
  , const expr
  , std::forward_iterator_tag
  >
{
  void* m_rep;

public:
  iterator()
  : m_rep( 0u ) {}
  explicit iterator(const pseries::repr*);
  iterator(const iterator &);
  ~iterator();

  iterator& operator=(iterator o)
  { o.swap( *this ); return *this; }
  void swap(iterator &o)
  { std::swap( m_rep, o.m_rep ); }

  bool empty() const;
  bool forced() const;

private:
  friend class boost::iterator_core_access;

  const expr &dereference() const;
  void increment();

  bool equal(const iterator &j) const
  {
    if( j.m_rep == 0u )
    { // special case : match end of pseries
      return !forced() || empty();
    }

    return m_rep == j.m_rep;
  }
};

} // namespace pseries_detail

inline pseries::const_iterator
pseries::begin() const
{ return const_iterator( m_rep.get() ); }

inline pseries::const_iterator
pseries::end()   const
{ return const_iterator(); }

} // namespace analysis

#endif
