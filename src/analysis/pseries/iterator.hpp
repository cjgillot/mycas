#ifndef PSERIES_ITERATOR_HPP
#define PSERIES_ITERATOR_HPP

#include "analysis/expr.hpp"

namespace analysis {
namespace pseries_detail {

class iterator
{
  void* m_rep;

public:
  explicit iterator(void*);
  iterator(const iterator &);
  ~iterator();

  iterator& operator=(iterator o)
  { o.swap( *this ); return *this; }
  void swap(iterator &o)
  { std::swap( m_rep, o.m_rep ); }

public:
  iterator &operator++();

  const expr &operator*() const;
  const expr *operator->() const;

  bool empty() const;
  bool forced() const;
};

}} // namespace analysis::pseries_detail

#endif
