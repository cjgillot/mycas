#ifndef VECTORSEQ_HASH_HPP
#define VECTORSEQ_HASH_HPP

/*!Hashing policy :
 *   in order to efficiently compute the hash,
 *   several variables are set in a \c vectorseq.
 *
 *   - sequence hash = sum( handle.coef.hash * handle.value.hash for handle in sequence )
 *   - coeffseq hash = sum( handle.coef.hash for handle in sequence )
 *
 * Thus, handle concept must provide three accesses :
 * - coefficient
 * - value
 * - sorting hash (used for sorting the sequence)
 *
 * A handle over vectorseq has its sorting hash designed like following (32 bit example)
 * - 8  bits of coeffseq hash
 * - 24 bits of sequence hash
 * This hashing emulates a grlex ordering, used for faster expansion code.
 */

#include <cstdlib>

namespace analysis {
namespace vectorseq_detail {

// vectorseq_base implementation
//@{
inline void vectorseq_base::construct_add(const vectorseq_base &a, const vectorseq_base &b)
{
  m_seqhash  = a.m_seqhash  + b.m_seqhash;
  m_coefhash = a.m_coefhash + b.m_coefhash;
}
inline void vectorseq_base::construct_sub(const vectorseq_base &a, const vectorseq_base &b)
{
  m_seqhash  = a.m_seqhash  - b.m_seqhash;
  m_coefhash = a.m_coefhash - b.m_coefhash;
}
inline void vectorseq_base::construct_neg(const vectorseq_base &a)
{
  m_seqhash  = - a.m_seqhash;
  m_coefhash = - a.m_coefhash;
}
inline void vectorseq_base::construct_sca(const number &n, const vectorseq_base &a)
{
  std::size_t nh = n.hash();
  m_seqhash  = nh * a.m_seqhash;
  m_coefhash = nh * a.m_coefhash;
}

inline std::size_t vectorseq_base::hash() const
{
  std::size_t seed = 0;
  boost::hash_combine(seed, m_coef.hash());
  boost::hash_combine(seed, m_hash);
  return seed;
}

inline std::size_t vectorseq_base::sort_hash() const
{
  std::size_t seed = 0;
  if( sizeof(std::size_t) == 64 )
  {
    seed |= ( m_coefhash & 0xffff ) << 48;
    seed |= m_seqhash & 0xffffffffffff;
  }
  else // if( sizeof(std::size_t) == 32 )
  {
    seed |= ( m_coefhash & 0xff ) << 24;
    seed |= m_seqhash & 0xffffff;
  }
  return seed;
}
//@}

}} // namespace analysis::vectorseq_detail

#endif // VECTORSEQ_HASH
