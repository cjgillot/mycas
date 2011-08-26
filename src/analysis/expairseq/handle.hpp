#ifndef EXPAIRSEQ_HANDLE_HPP
#define EXPAIRSEQ_HANDLE_HPP

#include <boost/intrusive_ptr.hpp>

#include "util/concept.hpp"
#include "util/compare.hpp"

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

    same_type( h.ptr(), ptr ); // pointer access

    null = h.null();  // null testing

    cmp = H::compare( h, h ); // comparison
    cmp = H::deep_compare( h, h ); // deep comparison

    hash = h.hash(); // hashing

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

namespace epseq {

template<class I, class M>
struct handle
: private util::implement_concept< ExpairseqHandle< handle<I,M>, I > >
{
private:
  typedef expairseq<I, M> eps_t;

public:
  typedef const I* const_pointer;

public: // cdtor
  handle(const eps_t*);

  handle(const handle &);
  handle &operator=(const handle &);

  ~handle() throw();

  void swap(handle &) throw();

public: // coercion
  handle(const expr&);
  operator expr() const;

public: // operations
  handle operator+(const handle &o) const;
  handle operator-(const handle &o) const;
  handle operator-() const;
  handle sca(const number &n) const;

public: // tests & comparison
  bool null() const;

  static util::cmp_t compare(const handle &a, const handle &b);
  static util::cmp_t deep_compare(const handle &a, const handle &b);

  std::size_t hash() const;

public: // printing
  template<class S>
  void print(S &os) const;

  const_pointer ptr() const throw();

private:
  // operations implementation
  eps_t* chg_coef(const number &n) const throw();

  // from expr coercion implementation
  static const_pointer from_expr( const expr & );

private: // member data
  //! \invariant Non null pointer
  boost::intrusive_ptr<const eps_t> m_ptr;
};

}} // namespace analysis::epseq

namespace std {

template<class I, class M>
inline void swap(
  analysis::epseq::handle<I,M> &a
, analysis::epseq::handle<I,M> &b
) {
  a.swap( b );
}

} // namespace std

#endif
