#ifndef VECTORSEQ_BASE_HPP
#define VECTORSEQ_BASE_HPP

#include "analysis/vectorseq/vseqfwd.hpp"

#include "analysis/vectorseq/handle_concept.hpp" // for handle
#include "analysis/vectorseq/iterator.hpp" // for eps_iterator

#include "analysis/vectorseq/poly.hpp"

#include "analysis/expr.hpp"

namespace analysis {
namespace vectorseq_detail {

//!\brief Non template base for \c vectorseq<>
class vectorseq_base
: public basic
{
protected: // polynomial types used by vectorseq<>
  typedef vseq::poly<const basic> poly_t;
  typedef boost::intrusive_ptr<const poly_t> polyptr_t;

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

public: // range
  typedef typename poly_t::const_iterator         const_piterator;

  const_piterator pbegin() const { return poly() ? poly()->begin() : nullptr; }
  const_piterator pend  () const { return poly() ? poly()->end  () : nullptr; }

  std::size_t size()  const { return poly() ? poly()->size() : 0ul; }
  bool        empty() const { return ! poly(); }

  template< class F >
  std::vector< expr >* map_children(F f) const;

protected: // access from derived
  number &coef() { return m_coef; }
  polyptr_t &poly() { return m_poly; }
  const polyptr_t &poly() const { return m_poly; }

  bool      is_monomial() const    { return poly() && poly()->size() == 1; }
  const basic* monomial() const    { ASSERT( is_monomial() ); return *poly()->begin(); }

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

public:
  bool has(const symbol &) const;
  void print(std::basic_ostream<char> &os) const;
private:
  virtual void print_base(std::basic_ostream<char> &os) const = 0;

private: // member data
  //! \brief Constant coefficient
  number m_coef;
  //! \brief Shared polynomial vector
  //! \invariant an empty polynomial is represented by \c nullptr
  polyptr_t m_poly;

  //! hashes, see hash.hpp
  std::size_t m_seqhash;
  std::size_t m_coefhash;
};

}} // namespace analysis::vectorseq_detail

#endif // VECTORSEQ_BASE_HPP

#include "vectorseq_base.ipp"
