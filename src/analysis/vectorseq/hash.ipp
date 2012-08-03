#ifndef ANALYSIS_VECTORSEQ_HASH_IPP
#define ANALYSIS_VECTORSEQ_HASH_IPP

#include "analysis/vectorseq/vectorseq_base.hpp"

#include <limits>
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
#define NBITS (std::numeric_limits<std::size_t>::digits)
#define SHIFT (3 * NBITS / 4)
#define MASK  ((std::size_t(1l) << SHIFT) - std::size_t(1l))
  return (m_coefhash << SHIFT) | (m_seqhash & MASK);
#undef MASK
#undef SHIFT
#undef NBITS
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
