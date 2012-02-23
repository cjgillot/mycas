#ifndef EXPAIRSEQ_HANDLE_HPP
#define EXPAIRSEQ_HANDLE_HPP

#include "analysis/vectorseq/handle_concept.hpp"

#include "analysis/expr.hpp"

#include "util/concept.hpp"
#include "util/compare.hpp"

namespace analysis {
namespace vseq {

template<class I, class M>
struct handle
: private util::implement_concept< ExpairseqHandle< handle<I,M>, I > >
{
private:
  typedef vectorseq<I, M> eps_t;

public:
  typedef I monomial_type;
  typedef const I* const_pointer;

public:
  // from expr coercion implementation
  // shall be specialized in derived classes
  static const_pointer from_expr(const expr &);

public: // operations
  static const_pointer add(const_pointer, const_pointer);
  static const_pointer sub(const_pointer, const_pointer);
  static const_pointer neg(const_pointer);

  static const_pointer sca(const_pointer, const number &);

  static const_pointer mul(const_pointer, const_pointer);

public: // tests & comparison
  static bool null(const_pointer);

  static util::cmp_t      compare(const_pointer, const_pointer);
  static util::cmp_t deep_compare(const_pointer, const_pointer);

  static std::size_t       hash(const_pointer);
  static std::size_t  coef_hash(const_pointer);
  static std::size_t value_hash(const_pointer);
  static void         sort_hash(const_pointer, std::size_t &, std::size_t &);

public: // printing
  template<class S>
  static void print(const_pointer, S &os);

private:
  // operations implementation
  static const_pointer chg_coef(const_pointer, const number &n) throw();
};

}} // namespace analysis::vseq

#endif
