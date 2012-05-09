#ifndef VECTORSEQ_HPP
#define VECTORSEQ_HPP

#include "analysis/vectorseq/vseqfwd.hpp"

#include "analysis/vectorseq/vectorseq_base.hpp"
#include "analysis/vectorseq/handle_concept.hpp" // for handle
#include "analysis/vectorseq/iterator.hpp" // for eps_iterator

#include "analysis/expr.hpp"

namespace analysis {

namespace vseq {
template<class> struct sort_pred;
}

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
class vectorseq
: public vectorseq_detail::vectorseq_base {

private: // These types shall never get outside vectorseq
  typedef typename Mono::handle EP;
  CONCEPT_ASSERT(( ExpairseqHandle<EP, Mono> ));

  typedef vectorseq_detail::vectorseq_base super;
  using super::poly_t;

  typedef typename poly_t::const_iterator         raw_const_iterator;
  typedef typename poly_t::const_reverse_iterator raw_const_reverse_iterator;

protected: // This type will be inherited and specialized by derived
  //! \brief Handle class
  typedef vseq::handle<Impl> handle;
  friend class vseq::handle<Impl>;

public: // simple cdtor
  explicit vectorseq(const number &n);
  virtual ~vectorseq();

  virtual vectorseq* clone() const = 0;

protected: // access from derived
  using super::is_monomial;
  const Mono* monomial() const { return static_cast<const Mono*>( super::monomial() ); }

protected:
  /*!\name Named constructors
   *
   * None of these constructors shall modifies the coefficient,
   * except \c construct_expr_range.
   *
   * \{
   */
  //@{
  void construct_monomial(const Mono*);

  void construct_add(const vectorseq &, const vectorseq &);
  void construct_sub(const vectorseq &, const vectorseq &);
  void construct_neg(const vectorseq &);

  void construct_sca(const number &, const vectorseq &);

  void rehash();

  typedef vseq::sort_pred<EP> sort_predicate;

  template<class Iter> void construct_mono_range       (const Iter &, const Iter &);
  template<class Iter> void construct_sorted_mono_range(const Iter &, const Iter &);

  // this one though does modify coef()
  template<class Iter, class EMono, class NAdd>
  void construct_expr_range(Iter beg, const Iter &en, EMono emono, NAdd nadd);
  //@}
  /*!\}*/

public: // range
  /*!\name Range operations
   *
   * All range operations are proxied by by an iterator adaptor
   * that converts <tt>const basic*</tt> to <tt>const Mono*</tt>.
   *
   * \{
   */
  //@{
  typedef vseq::eps_iterator< Mono, raw_const_iterator >         const_iterator;
  typedef vseq::eps_iterator< Mono, raw_const_reverse_iterator > const_reverse_iterator;

  typedef const_iterator         iterator;
  typedef const_reverse_iterator reverse_iterator;

  const_iterator          begin() const { ASSERT( poly() ); return const_iterator( poly()->begin() ); }
  const_iterator          end()   const { ASSERT( poly() ); return const_iterator( poly()->end() ); }

  const_reverse_iterator rbegin() const { ASSERT( poly() ); return const_reverse_iterator( poly()->rbegin() ); }
  const_reverse_iterator rend()   const { ASSERT( poly() ); return const_reverse_iterator( poly()->rend() ); }

  using super::size;
  using super::empty;
  using super::map_children;
  //@}
  /*!\}*/

private: // comparison
  using super::hash;
  using super::coef_hash;
  using super::value_hash;
  using super::sort_hash;

  friend class vseq::sort_pred<handle>;

  util::cmp_t partial_compare(const vectorseq &) const;
  util::cmp_t compare_same_type(const basic &) const;
};

namespace vseq {

template<class EP>
struct sort_pred
: std::binary_function<
  typename EP::const_pointer
, typename EP::const_pointer
, bool
>
{
  typedef typename EP::const_pointer ptr_t;

  inline bool
  operator()(ptr_t a, ptr_t b) const
  { return EP::compare( a, b ) < 0; }
};

} // namespace vseq

} // namespace analysis

#include "analysis/vectorseq/handle.ipp"
#include "analysis/vectorseq/vectorseq.ipp"

#endif /* EXPAIRSEQ_HPP_ */
