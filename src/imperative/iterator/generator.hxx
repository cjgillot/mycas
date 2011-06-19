/*
 * generator.hxx
 *
 *  Created on: 13 juin 2011
 *      Author: k1000
 */

#ifndef GENERATOR_HXX_
#define GENERATOR_HXX_

#include "stdlib.hxx"

namespace imperative {
namespace iterator {

namespace detail {

/*!
 * \class gen_iterator
 *
 * \brief helper iterator class for generate
 */
template<class F>
class gen_iterator
: public boost::iterator_facade<
    gen_iterator<It, F>
  , typename boost::result_of<F()>::type const
  , boost::incrementable_traversal_tag
  >
, public operators::testable<gen_iterator<F> > {

  typedef typename boost::iterator_value<It>::type T;
  typedef typename boost::result_of<F(T)>::type res_t;

  F f;

private:
  gen_iterator();

public:
  /*!
   * \brief Copy constructor
   */
  inline
  gen_iterator(const gen_iterator &o)
  : f(o.f) {}
  /*!
   * \brief Assignment operator
   */
  inline gen_iterator &
  operator=(const gen_iterator &o) {
    f=o.f;
    return *this;
  }
  /*!
   * \brief Non-throwing swap
   */
  inline void
  swap(gen_iterator &o) {
    std::swap(f, o.f);
  }

  /*!
   * \brief Constructor from function
   */
  inline explicit
  gen_iterator(const F &f)
  : f(f) {}

  /*!
   * \brief Destructor
   */
  inline
  ~gen_iterator() {}

private:
  friend class boost::iterator_core_access;

  inline void
  increment() {}
  inline const T &
  dereference() const {
    return f();
  }

public:
  //! \brief Validity test
  inline bool
  valid() const
  { return true; }
};

} // namespace detail

/*!
 * \struct gen_type
 * \brief Helper class for typing generate
 */
template<class F>
struct gen_type {
  //! \brief Iterator type
  typedef detail::gen_iterator<F> iterator;
  //! \brief Value type
  typedef typename iterator::value_type         value_type;
  //! \brief Reference type
  typedef typename iterator::reference          reference;
  //! \brief Pointer type
  typedef typename iterator::pointer            pointer;
  //! \brief Iterator difference type
  typedef typename iterator::difference_type    difference_type;
  //! \brief Iterator category
  typedef typename iterator::iterator_category  iterator_category;
};

/*!
 * \fn gen_iterator<F> generate(F)
 * \brief Creates a generator iterator
 */
template<class F>
inline detail::gen_iterator<F>
generate(F f) {
  return detail::gen_iterator<F>(f);
}

}} // imperative::iterator


#endif /* GENERATOR_HXX_ */
