#ifndef ANALYSIS_VECTORSEQ_HASH_IPP
#define ANALYSIS_VECTORSEQ_HASH_IPP

#include "analysis/vectorseq/vectorseq_base.hpp"

#include <boost/functional/hash.hpp>

namespace analysis {
namespace vectorseq_detail {

#define VSB vectorseq_base

inline std::size_t
VSB::coef_hash() const
{ return m_coef.hash(); }

inline std::size_t
VSB::value_hash() const
{
  std::size_t seed = 0;
  boost::hash_combine(seed, m_coefhash);
  boost::hash_combine(seed, m_seqhash);
  return seed;
}

inline void
VSB::sort_hash(std::size_t &high, std::size_t &low) const
{
  high = m_coefhash;
  low  = m_seqhash;
}

inline void
VSB::cons_hash(const std::size_t ch, const std::size_t vh)
{
  m_seqhash  += ch * vh;
  m_coefhash += ch;
}

#undef VSB

}} // namespace analysis::vectorseq_detail

#endif
