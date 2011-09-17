#ifndef EXPAIRSEQ_HPP_
#define EXPAIRSEQ_HPP_

#include "analysis/expairseq/epseqfwd.hpp"

#include "analysis/number.hpp" // for number

#include "analysis/expairseq/handle_concept.hpp" // for handle
#include "analysis/expairseq/iterator.hpp" // for eps_iterator

#include "analysis/ptr.hpp"

namespace analysis {

/*!\brief Implementation class for \c sum and \c mul
 *
 * This CRTP abstract class is used as a common implementation for
 * both \c sum and \c mul.
 *
 * The object is represented as a pair coefficient-polynomial,
 * with a scalar coefficient and a polynomial of \c Mono over the scalar ring.
 *
 * The polynomial is shared and its hash value is computed during
 * construction, in order to save comparison times.
 *
 * \param Impl : the implemented class
 * \param Mono : the monomial class
 */
template<class Impl, class Mono>
class expairseq
: public basic {

private: // These types shall never get outside expairseq
  typedef typename Mono::handle epair;
  CONCEPT_ASSERT(( ExpairseqHandle<epair, Mono> ));

  typedef epseq::poly<epair> poly_t;

  typedef typename poly_t::const_iterator         raw_const_iterator;
  typedef typename poly_t::const_reverse_iterator raw_const_reverse_iterator;

protected: // This type will be inherited and specialized by derived
  //! \brief Handle class
  typedef epseq::handle<Impl, Mono> handle;
  friend class epseq::handle<Impl, Mono>;

public: // simple cdtor
  explicit expairseq(const number &n);
  virtual ~expairseq();

  virtual expairseq* clone() const = 0;

protected: // named constructors, none modifies the coefficient
  void construct_monomial(const Mono*);

  void construct_add(const expairseq &, const expairseq &);
  void construct_sub(const expairseq &, const expairseq &);
  void construct_neg(const expairseq &);

  void construct_sca(const number &, const expairseq &);

  template<class Iter> void construct_mono_range        (const Iter &, const Iter &);
  template<class Iter> void construct_mutable_mono_range(const Iter &, const Iter &);

  // this one though does modify m_coef
  template<class Iter, class EMono, class NAdd>
  void construct_expr_range(Iter beg, const Iter &en, EMono emono, NAdd nadd);

public: // access
  const number &coef() const  { return m_coef; }

protected: // access from derived
  bool     is_monomial() const    { return m_poly && m_poly->size() == 1; }
  const Mono* monomial() const    { ASSERT( is_monomial() ); return m_poly->begin()->get(); }

  number &coef() { return m_coef; }

public: // range
  /*!\name Range operations
   *
   * All range operations are proxied by by an iterator adaptor
   * that converts \ref epair to <tt>const Mono*</tt>.
   *
   * \{
   */
  //@{
  typedef epseq::eps_iterator< Mono, raw_const_iterator >         const_iterator;
  typedef epseq::eps_iterator< Mono, raw_const_reverse_iterator > const_reverse_iterator;

  typedef const_iterator         iterator;
  typedef const_reverse_iterator reverse_iterator;

  const_iterator          begin() const { ASSERT( m_poly ); return const_iterator( m_poly->begin() ); }
  const_iterator          end()   const { ASSERT( m_poly ); return const_iterator( m_poly->end() ); }

  const_reverse_iterator rbegin() const { ASSERT( m_poly ); return const_reverse_iterator( m_poly->rbegin() ); }
  const_reverse_iterator rend()   const { ASSERT( m_poly ); return const_reverse_iterator( m_poly->rend() ); }

  std::size_t             size()  const { return m_poly ? m_poly->size() : 0; }
  bool                    empty() const { return ! m_poly; }

  template< class F >
  std::vector< expr >* map_children(F f) const;
  //@}
  /*!\}*/

private: // comparison
  std::size_t hash() const;

  util::cmp_t partial_compare(const expairseq &) const;
  util::cmp_t compare_same_type(const basic &) const;

public:
  void print(std::basic_ostream<char> &os) const;
  bool has(const symbol &) const;

private: // member data
  //! \brief Constant coefficient
  number m_coef;

  //! \brief Shared polynomial vector
  //! \invariant an empty polynomial is represented by \c nullptr
  ptr<const poly_t> m_poly;

  //! \brief Hash value of \c m_poly
  std::size_t m_hash;
};

} // namespace analysis

#include "analysis/expairseq/handle.ipp"
#include "analysis/expairseq.ipp"

#endif /* EXPAIRSEQ_HPP_ */
