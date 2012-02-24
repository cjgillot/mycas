#ifndef VECTORSEQ_BASE_IPP
#define VECTORSEQ_BASE_IPP

#include "analysis/vectorseq/vseqfwd.hpp"
#include "analysis/vectorseq/vectorseq_base.hpp"
#include "analysis/expr.hpp"

#include "util/move.hpp"

#include <boost/mpl/if.hpp>

namespace analysis {
namespace vectorseq_detail {

/*!\brief Function mapping
  *
  * \param f a unary function mapping <tt>const Mono*</tt> to \c expr
  *
  * \return \c nullptr if nothing has changed, the mapped vector else
  *
  * Nothing has changed case includes :
  * - empty polynomial
  * - pointer equality of original \c Mono and result \c expr
  */
template<class F>
inline std::vector<expr>*
vectorseq_base::map_children( F f ) const
{
  if( ! m_poly )
    return nullptr;

  const_piterator
    it = m_poly->begin(),
    en = m_poly->end();

  for( ; it != en; ++it )
  {
    // not expr to avoid automatic evaluation
    typedef typename boost::mpl::if_<
      boost::is_same<expr, typename F::result_type>
    , expr, ptr<const basic>
    >::type buf_t;
    buf_t r = f( *it );

    if( r.get() != *it )
    {
      // something has changed
      // create a new vector
      typedef std::vector< expr > vec_t;
      util::scoped_ptr< vec_t > retp ( new vec_t );
      vec_t &ret = *retp;
      ret.reserve( m_poly->size() );

      // unchanged terms
      for( const_piterator i2 = m_poly->begin(); i2 != it; ++i2 )
        ret.push_back( *i2 );

      // current term
      ret.push_back( r );
      ++it;

      // remaining terms
      for( ; it != en; ++it )
        ret.push_back( f( *it ) );

      return retp.release();
    }
  }

  // nothing has changed
  return nullptr;
}

}} // namespace analysis::vectorseq_detail

#include "hash.ipp"

#endif // VECTORSEQ_BASE_HPP
