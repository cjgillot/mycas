#include "vectorseq_base.hpp"
#include <boost/functional/hash.hpp>

using namespace analysis;
using namespace vectorseq_detail;

vectorseq_base::vectorseq_base(const number &n)
: m_coef( n )
, m_seqhash( 0ul ), m_coefhash( 0ul )
{}

vectorseq_base::vectorseq_base(const vectorseq_base &o)
: basic(o)
, m_coef(o.m_coef)
, m_seqhash(o.m_seqhash), m_coefhash(o.m_coefhash)
{}

vectorseq_base::~vectorseq_base() {}
