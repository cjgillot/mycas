/*
 * stream_iterator.hxx
 *
 *  Created on: 12 juin 2011
 *      Author: k1000
 */

#ifndef STREAM_ITERATOR_HXX_
#define STREAM_ITERATOR_HXX_

#include "stream/streamfwd.hxx"
#include "stream/stream.hxx"

namespace streams {

template<class T>
struct stream_iterator
: boost::iterator_facade<
    stream_iterator<T>
  , const T
  , boost::incrementable_traversal_tag
  > {

  typedef stream_impl<T> stream_t;
  typedef typename stream_t::list_t::const_iterator iter_t;

  boost::intrusive_ptr<stream_t> str;
  iter_t it, end;

public:
  inline
  stream_iterator() {}
  inline
  stream_iterator(const stream_iterator &o)
  : str(o.str), it(o.it), end(o.end) {}
  inline stream_iterator &
  operator=(const stream_iterator &o) {
    str=o.str; it=o.it; end=o.end;
    return *this;
  }

  inline void
  swap(stream_iterator &o) {
    std::swap(str, o.str);
    std::swap(it, o.it);
    std::swap(end, o.end);
  }

  inline
  ~stream_iterator() {}

public:
  stream_iterator(stream_t* s)
  : str(s)
  , it(s->values.begin()), end(s->values.end())
  {}

private:
  friend class boost::iterator_core_access;

  inline void
  increment() {
    assert(it != end);
    if(++it == end)
      it = str->incr();
  }
  inline const T &
  dereference() const {
    assert(it != end);
    return *it;
  }

private:
  typedef util::safe_bool<void(stream_iterator::*)(stream_iterator&)> safe_bool;
  typedef typename safe_bool::unspecified_bool_type bool_t;
public:
  inline
  operator bool_t() const {
    return safe_bool::to_unspecified_bool(
        it != end,
        &stream_iterator::swap
    );
  }
  inline bool
  operator!() const
  { return it == end; }
};

} // namespace streams

#endif /* STREAM_ITERATOR_HXX_ */
