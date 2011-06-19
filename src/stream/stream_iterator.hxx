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
#include "stream/stream_base.hxx"

namespace streams {

template<class T, class Mem>
struct stream_iterator
: boost::iterator_facade<
    stream_iterator<T, Mem>
  , const T
  , boost::incrementable_traversal_tag
  > {

  typedef stream_base<T, Mem> stream_t;
  typedef typename stream_t::list_t::const_iterator iter_t;

  boost::intrusive_ptr<stream_t> str;
  iter_t it;

private:
  inline iter_t
  end() const
  { return str->end(); }

public:
  inline
  stream_iterator() {}
  inline
  stream_iterator(const stream_iterator &o)
  : str(o.str), it(o.it) {}
  inline stream_iterator &
  operator=(const stream_iterator &o) {
    str=o.str; it=o.it;
    return *this;
  }

  inline void
  swap(stream_iterator &o) {
    std::swap(str, o.str);
    std::swap(it, o.it);
  }

  inline
  ~stream_iterator() {}

public:
  stream_iterator(stream_t* s)
  : str(s)
  , it(s->values().begin())
  { if(it == end()) it=s->incr(); }

private:
  friend class boost::iterator_core_access;

  inline void
  increment() {
    assert(*this);
    if(++it == end()) // no memoized left
      it=str->incr();
  }
  inline const T &
  dereference() const {
    assert(*this);
    return *it;
  }

private:
  typedef util::safe_bool<void(stream_iterator::*)(stream_iterator&)> safe_bool;
  typedef typename safe_bool::unspecified_bool_type bool_t;
public:
  inline
  operator bool_t() const {
    return safe_bool::to_unspecified_bool(
        it != end(),
        &stream_iterator::swap
    );
  }
  inline bool
  operator!() const
  { return it == end(); }
};

} // namespace streams

#endif /* STREAM_ITERATOR_HXX_ */
