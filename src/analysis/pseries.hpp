#ifndef ANALYSIS_PSERIES_HPP
#define ANALYSIS_PSERIES_HPP

#include "analysis/expr.hpp"
#include "analysis/basic.hpp"
#include "analysis/symbol.hpp"

namespace analysis {

class pseries
: public basic
{
  typedef std::deque<expr> seq_t;

  REGISTER_FINAL( pseries, basic )

public:
  pseries();
  pseries(const pseries &);
  ~pseries();

  pseries* clone() const
  { return new pseries(*this); }

public:
  typedef seq_t::const_iterator          const_iterator;
  typedef seq_t::const_reveerse_iterator const_reverse_iterator;

  const_iterator begin() const { return m_seq.begin(); }
  const_iterator   end() const { return m_seq.end(); }

  const_reverse_iterator rbegin() const { return m_seq.rbegin(); }
  const_reverse_iterator   rend() const { return m_seq.rend(); }

  std::size_t  size() const { return m_seq.size(); }
  std::size_t empty() const { return m_seq.empty(); }

private:
  seq_t  m_seq;
  symbol m_var;
};

} // namespace analysis

#endif
