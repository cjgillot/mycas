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
 * This hashing emulates a glex ordering, used for faster expansion code.
 */

#include <cstdlib>
#include <boost/functional/hash.hpp>

#include "analysis/vectorseq/vectorseq_base.hpp"

using namespace analysis;
using namespace vectorseq_detail;

#define VSB vectorseq_base

void VSB::construct_mon(const std::size_t ch, const std::size_t vh)
{
  m_seqhash  = ch * vh;
  m_coefhash = ch;
}
void VSB::construct_add(const VSB &a, const VSB &b)
{
  m_seqhash  = a.m_seqhash  + b.m_seqhash;
  m_coefhash = a.m_coefhash + b.m_coefhash;
}
void VSB::construct_sub(const VSB &a, const VSB &b)
{
  m_seqhash  = a.m_seqhash  - b.m_seqhash;
  m_coefhash = a.m_coefhash - b.m_coefhash;
}
void VSB::construct_neg(const VSB &a)
{
  m_seqhash  = - a.m_seqhash;
  m_coefhash = - a.m_coefhash;
}
void VSB::construct_sca(const number &n, const VSB &a)
{
  std::size_t nh = n.hash();
  m_seqhash  = nh * a.m_seqhash;
  m_coefhash = nh * a.m_coefhash;
}

std::size_t VSB::hash() const
{
  std::size_t seed = 0;
  boost::hash_combine(seed, coef_hash());
  boost::hash_combine(seed, value_hash());
  return seed;
}
