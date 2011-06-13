/*
 * range.hxx
 *
 *  Created on: 13 juin 2011
 *      Author: k1000
 */

#ifndef RANGE_HXX_
#define RANGE_HXX_

#include "stdlib.hxx"

namespace imperative {
namespace iterator {

namespace detail {

template<class Range>
class range_iterator
: public boost::iterator_facade<
    range_iterator<Range>
  , typename boost::range_value<Range>::type
  , boost::incrementable_traversal_tag
  >
{
  typedef typename boost::range_value<Range>::type T;

  typedef typename boost::range_iterator<const Range>::type iter;

  iter it, end;

public:
  inline
  range_iterator() {}
  inline
  range_iterator(const range_iterator &o)
  : it(o.it), end(o.end) {}
  inline range_iterator &
  operator=(const range_iterator &o) {
    it=o.it; end=o.end;
    return *this;
  }

  inline void
  swap(range_iterator &o) {
    std::swap(it, o.it);
    std::swap(end,o.end);
  }

  inline explicit
  range_iterator(const Range &r)
  : it(boost::begin(r)), end(boost::end(r)) {}

  inline virtual
  ~range_iterator() {}

private:
  friend class boost::iterator_core_access;

  inline void
  increment() {
    assert(it != end);
    ++it;
  }
  inline const T &
  dereference() const {
    assert(it != end);
    return *it;
  }

private:
  typedef util::safe_bool<void(range_iterator::*)(range_iterator&)> safe_bool;
  typedef typename safe_bool::unspecified_bool_type bool_t;
public:
  inline
  operator bool_t() const {
    return safe_bool::to_unspecified_bool(
        it != end,
        &range_iterator::swap
    );
  }
  inline bool
  operator!() const
  { return it == end; }
};

} // namespace detail

template<class F>
struct gen_type {
  typedef detail::gen_iterator<It,F> iterator;
  typedef typename iterator::value_type         value_type;
  typedef typename iterator::reference          reference;
  typedef typename iterator::pointer            pointer;
  typedef typename iterator::difference_type    difference_type;
  typedef typename iterator::iterator_category  iterator_category;
};

template<class It, class F>
inline detail::map_iterator<It, F>
map(const It &it, const F &f) {
  return detail::map_iterator<It,F>(it, f);
}

}} // imperative::iterator


#endif /* RANGE_HXX_ */
