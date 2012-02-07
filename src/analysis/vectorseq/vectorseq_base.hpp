#ifndef VECTORSEQ_BASE_HPP
#define VECTORSEQ_BASE_HPP

#include "analysis/vectorseq/vseqfwd.hpp"

#include "analysis/vectorseq/handle_concept.hpp" // for handle
#include "analysis/vectorseq/iterator.hpp" // for eps_iterator

#include "analysis/expr.hpp"

namespace analysis {
namespace vectorseq_detail {

//!\brief Non template base for \c vectorseq<>
class vectorseq_base
: public basic {

public: // simple cdtor
  explicit vectorseq_base(const number &);
  vectorseq_base(const vectorseq_base&);

  virtual ~vectorseq_base();

  virtual vectorseq_base* clone() const = 0;

protected: // named constructors, set up hashing. Implemented in hash.hpp
  void construct_mon(const std::size_t, const std::size_t);
  void construct_add(const vectorseq_base &, const vectorseq_base &);
  void construct_sub(const vectorseq_base &, const vectorseq_base &);
  void construct_neg(const vectorseq_base &);
  void construct_sca(const number &, const vectorseq_base &);

  void cons_hash(const std::size_t, const std::size_t);

public: // access
  const number &coef() const  { return m_coef; }

protected: // access from derived
  number &coef() { return m_coef; }

public: // comparison
  std::size_t       hash() const;
  std::size_t  coef_hash() const;
  std::size_t value_hash() const;
  void         sort_hash(std::size_t &, std::size_t &) const;

protected:
  util::cmp_t partial_compare(const vectorseq_base &o) const
  {
    util::cmp_t c = util::compare( m_coefhash, o.m_coefhash );
    if(c) return c;

    c = util::compare( m_seqhash, o.m_seqhash );
    return c;
  }
  util::cmp_t compare_same_type(const basic &o_) const
  {
    const vectorseq_base &o = static_cast<const vectorseq_base&>(o_);

    util::cmp_t c = partial_compare(o);
    if(c) return c;

    c = number::compare( m_coef, o.m_coef );
    return c;
  }

private: // member data
  //! \brief Constant coefficient
  number m_coef;

  //! hashes, see hash.hpp
  std::size_t m_seqhash;
  std::size_t m_coefhash;
};

}} // namespace analysis::vectorseq_detail

#include "hash.hpp"

#endif // VECTORSEQ_BASE_HPP
