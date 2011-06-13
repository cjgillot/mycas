/*
 * stream.hxx
 *
 *  Created on: 12 juin 2011
 *      Author: k1000
 */

#ifndef STREAM_STREAM_HXX_
#define STREAM_STREAM_HXX_

#include "utils.hxx"
#include "stream/streamfwd.hxx"
#include "stream/iterator.hxx"

namespace streams {

template<class T>
class stream_impl
// FIXME is this useful ?
: public util::refcounted
, protected boost::noncopyable {
  typedef std::list<T> list_t;

  mutable meta_iterator<T> it;
  mutable list_t values;

private:
  stream_impl();

public:
  inline explicit
  stream_impl(iterator_base<T>* i)
  : it(i) {}
  inline
  ~stream_impl() {}

private:
  friend class stream_iterator<T>;

  typename list_t::iterator
  incr() const{
    if(! it) return values.end();

    values.push_back(*it++);
    return --values.end();
  }
};

} // namespace streams

#include "stream/stream_iterator.hxx"

namespace streams {

template<class T>
struct stream {
  typedef stream_impl<T> impl_t;
  boost::intrusive_ptr<impl_t> impl;

public:
  inline
  stream() {}
  inline
  stream(const stream &o)
  : impl(o.impl) {}
  inline stream &
  operator=(const stream &o) {
    impl = o.impl;
    return *this;
  }

  inline void
  swap(stream &o) {
    std::swap(impl, o.impl);
  }

  explicit
  stream(iterator_base<T>* i)
  : impl(new impl_t(i)) {}

  inline
  ~stream() {}

public:
  typedef stream_iterator<T> iterator;
  typedef stream_iterator<T> const_iterator;

  const_iterator    begin() const
  { return stream_iterator<T>(impl.get()); }
  iterator_base<T>* iter() const
  { return detail::adapt(begin()); }
};

} // namespace streams

#endif /* STREAM_STREAM_HXX_ */
