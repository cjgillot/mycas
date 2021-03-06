#ifndef EXPAIRSEQ_ITERATOR_HPP
#define EXPAIRSEQ_ITERATOR_HPP

#include <boost/iterator/iterator_adaptor.hpp>

namespace analysis {
namespace vseq {

template< class Mono, class Base >
struct eps_iterator
: public boost::iterator_adaptor<
    eps_iterator< Mono, Base > // Derived
  , Base // Base
  , const Mono* // Value
  , boost::use_default // CategoryOrTraversal
  , const Mono* // Reference
  >
{
private:
  typedef typename eps_iterator::iterator_adaptor_ super;

public:
  eps_iterator() = default;
  eps_iterator( const Base &it )
  : super( it ) {}

private:
  friend class boost::iterator_core_access;

  const Mono* dereference() const
  { return static_cast<const Mono*>( *super::base_reference() ); }
};

}} // namespace analysis::vseq

#endif
