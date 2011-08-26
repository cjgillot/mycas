#ifndef EXPAIRSEQ_HPP_
#define EXPAIRSEQ_HPP_

namespace analysis {

template<class Impl, class Mono>
class expairseq;

} // namespace analysis

#include "analysis/number.hpp" // for number

#include "analysis/expairseq/iterator.hpp" // for eps_iterator
#include "analysis/expairseq/handle.hpp" // for handle
#include "analysis/expairseq/poly.hpp" // for poly

#include <boost/intrusive_ptr.hpp>

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

protected: // This type will be inherited and specialized by derived
  //! \brief Handle class
  typedef epseq::handle<Impl, Mono> handle;
  friend class epseq::handle<Impl, Mono>;

protected: // simple cdtor
  explicit expairseq(const number &n);
  ~expairseq();

  virtual expairseq* clone() const = 0;

protected: // named constructors
  //!\brief Monomial construction
  void construct_monomial( const Mono* p );

  /*!\name Operation constructors
   *
   * Given two \c expairseq, these constructors build
   * a merge depending on the call tag.
   *
   * Complexity : linear
   *   (both in \ref epair copies and comparisons)
   *
   * \{
   */
  //@{
  void construct_add(const expairseq &, const expairseq &);
  void construct_sub(const expairseq &, const expairseq &);
  void construct_neg(const expairseq &);

  void construct_sca(const number &, const expairseq &);
  //@}
  /*!\}*/

  /*!\name Monomial-range construction
   *
   * Given a number and an unsorted container of <tt>const Mono*</tt>,
   * construct a new expairseq.
   *
   * Complexity : linearithmic in the range size
   *
   * \{
   */
  //@{
  template<class Iter> void construct_mono_range        (const Iter &beg, const Iter &en);
  template<class Iter> void construct_mutable_mono_range(const Iter &beg, const Iter &en);
  //@}
  /*!\}*/

  /*!\brief Expression range construction
   *
   * Given a range of expression, construct an \c expairseq from it
   *
   * \param beg the first iterator
   * \param end the last iterator
   * \param emono a unary functor converting expression to monomial
   * \param nadd a binary functor merging numbers
   */
  template<class Iter, class EMono, class NAdd>
  void construct_expr_range(Iter beg, const Iter &en, EMono emono, NAdd nadd);

public: // access
  const number &coef() const  { return m_coef; }

protected: // access from derived
  bool     is_mono() const    { return m_poly && m_poly->size() == 1; }
  const Mono* mono() const    { ASSERT( is_mono() ); return m_poly->begin()->ptr(); }

  number &coef() { return m_coef; }

private:
  /*!\name Raw range operations
   *
   * Only const iterators are provided since usage of
   * non-const one could break the invariants.
   *
   * \{
   */
  //@{
  typedef typename poly_t::const_iterator         raw_const_iterator;
  typedef typename poly_t::const_reverse_iterator raw_const_reverse_iterator;

  raw_const_iterator raw_begin() const { return m_poly ? m_poly->begin() : raw_const_iterator(); }
  raw_const_iterator raw_end()   const { return m_poly ? m_poly->end()   : raw_const_iterator(); }

  raw_const_reverse_iterator raw_rbegin() const { return m_poly ? m_poly->rbegin() : raw_const_reverse_iterator(); }
  raw_const_reverse_iterator raw_rend()   const { return m_poly ? m_poly->rend()   : raw_const_reverse_iterator(); }
  //@}
  /*!\}*/

public:
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

  const_iterator begin() const { return const_iterator( raw_begin() ); }
  const_iterator end()   const { return const_iterator( raw_end() ); }

  const_reverse_iterator rbegin() const { return const_iterator( raw_rbegin() ); }
  const_reverse_iterator rend()   const { return const_iterator( raw_rend() ); }

  std::size_t size()  const { return m_poly ? m_poly->size() : 0; }
  bool        empty() const { return ! m_poly; }
  //@}
  /*!\}*/

private: // comparison
  std::size_t hash() const;

  util::cmp_t partial_compare(const expairseq &) const;
  util::cmp_t compare_same_type(const basic &) const;

public:
  void print(std::basic_ostream<char> &os) const;
  bool has(const symbol &) const;

protected:
  /*!\brief Function mapping
   *
   * \param f a unary function mapping <tt>const Mono*</tt> to \c expr
   *
   * \return \c nullptr if nothing has changed, the mapped vector else
   *
   * Nothing has changed case includes :
   * - empty polynomial
   * - pointer equality of original \c epair and result \c expr
   */
  template< class F >
  std::vector< expr >* map_children(F f) const;

private: // member data
  //! \brief Constant coefficient
  number m_coef;

  //! \brief Shared polynomial vector
  //! \invariant an empty polynomial is represented by \c nullptr
  boost::intrusive_ptr<const poly_t> m_poly;

  //! \brief Hash value of \c m_poly
  std::size_t m_hash;
};

} // namespace analysis

#endif /* EXPAIRSEQ_HPP_ */

#include "analysis/expairseq/handle.ipp"
#include "analysis/expairseq.ipp"
