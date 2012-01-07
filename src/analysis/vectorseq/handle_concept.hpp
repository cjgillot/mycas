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

  // const_pointer consistency
  STATIC_ASSERT (( boost::is_same< const_pointer, const Mono* >::value ));

  // construction
  CONCEPT_ASSERT(( boost::CopyConstructible<H> ));
  CONCEPT_ASSERT(( boost::Assignable<H> ));

  // coercion
  CONCEPT_ASSERT(( boost::Convertible<expr, H> ));
  CONCEPT_ASSERT(( boost::Convertible<H, expr> ));

  // operations
  CONCEPT_ASSERT(( boost::PlusOp<H, H, H> ));
  CONCEPT_ASSERT(( boost::SubtractOp<H, H, H> ));
  CONCEPT_ASSERT((        NegateOp ));

  CONCEPT_USAGE( ExpairseqHandle )
  {
    H h ( ptr ); // construction from pointer
    h.swap( h ); // swap

    same_type( h.get(), ptr ); // pointer access

    null = h.null();  // null testing

    cmp = H::compare( h, h ); // comparison
    cmp = H::deep_compare( h, h ); // deep comparison

    // hashing, see hash.hpp
    hash = h.      hash();
    hash = h. coef_hash();
    hash = h.value_hash();

    h.print( os ); // printing
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
