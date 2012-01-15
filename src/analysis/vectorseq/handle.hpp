#ifndef EXPAIRSEQ_HANDLE_HPP
#define EXPAIRSEQ_HANDLE_HPP

#include "analysis/vectorseq/handle_concept.hpp"

#include "analysis/ptr.hpp"

#include "util/concept.hpp"
#include "util/compare.hpp"

namespace analysis {
namespace epseq {

template<class I, class M>
struct handle
: private util::implement_concept< ExpairseqHandle< handle<I,M>, I > >
{
private:
  typedef vectorseq<I, M> eps_t;

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

  // from expr coercion implementation
  // shall be specialized in derived classes
  static const_pointer from_expr(const expr &);

public: // operations
  handle operator+(const handle &o) const;
  handle operator-(const handle &o) const;
  handle operator-() const;
  handle sca(const number &n) const;

  handle operator*(const handle &o) const;

public: // tests & comparison
  bool null() const;

  static util::cmp_t compare(const handle &a, const handle &b);
  static util::cmp_t deep_compare(const handle &a, const handle &b);

  std::size_t       hash() const;
  std::size_t  coef_hash() const;
  std::size_t value_hash() const;
  void         sort_hash(std::size_t &, std::size_t &) const;

public: // printing
  template<class S>
  void print(S &os) const;

  const_pointer get() const throw();

private:
  // operations implementation
  eps_t* chg_coef(const number &n) const throw();

private: // member data
  //! \invariant Non null pointer
  ptr<const eps_t> m_ptr;
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
