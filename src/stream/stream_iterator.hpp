/*
 * stream_iterator.hpp
 *
 *  Created on: 12 juin 2011
 *      Author: k1000
 */

#ifndef STREAM_ITERATOR_HXX_
#define STREAM_ITERATOR_HXX_

#include "stream/streamfwd.hpp"
#include "stream/stream.hpp"
#include "stream/stream_base.hpp"

namespace streams {

template<class T, class Mem>
struct stream_iterator
: public boost::iterator_facade<
    stream_iterator<T, Mem>
  , const T
  , boost::incrementable_traversal_tag
  >
, public operators::testable<stream_iterator<T, Mem> > {

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

public:
  inline bool
  valid() const
  { return it != end(); }
};

} // namespace streams

#endif /* STREAM_ITERATOR_HXX_ */
