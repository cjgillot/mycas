#ifndef EXPAIRSEQ_HANDLE_CONCEPT_HPP
#define EXPAIRSEQ_HANDLE_CONCEPT_HPP

#include "util/concept.hpp"

namespace analysis {

//! \brief Concept class for handle
template<class H, class Mono>
struct ExpairseqHandle
{
private:
  struct NegateOp
  {
    CONCEPT_USAGE(NegateOp) { (void)constraints_(); }
  private:
    H constraints_() { return - a; }
    H a;
  };

public:
  typedef typename H::const_pointer const_pointer;
  typedef typename H::monomial_type monomial_type;

  // const_pointer consistency
  STATIC_ASSERT (( boost::is_same< monomial_type, Mono >::value ));
  STATIC_ASSERT (( boost::is_same< const_pointer, const Mono* >::value ));

  CONCEPT_USAGE( ExpairseqHandle )
  {
    null = H::null( ptr );  // null testing

    // operations
    ptr = H::add( ptr, ptr );
    ptr = H::sub( ptr, ptr );
    ptr = H::neg( ptr );

    cmp = H::compare( ptr, ptr ); // comparison
    cmp = H::deep_compare( ptr, ptr ); // deep comparison

    // hashing, see hash.hpp
    hash = H::      hash( ptr );
    hash = H:: coef_hash( ptr );
    hash = H::value_hash( ptr );

    H::print( ptr, os ); // printing
  }

private:
  bool null;
  std::ostream os;
  util::cmp_t cmp;
  std::size_t hash;
  const_pointer ptr;

  template<typename T>
  void same_type( const T&, const T& );
};

} // namespace analysis

#endif
