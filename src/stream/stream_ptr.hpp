/*
 * streamptr.hpp
 *
 *  Created on: 19 juin 2011
 *      Author: k1000
 */

#ifndef STREAM_PTR_HXX_
#define STREAM_PTR_HXX_

#include "utils.hpp"
#include "stream/streamfwd.hpp"
#include "stream/iterator.hpp"

#include "stream/stream_base.hpp"
#include "stream/iterator_stream.hpp"

namespace streams {

template<class T, class Mem=std::list<T> >
class stream_ptr {
  typedef stream_base<T, Mem> impl_t;
  boost::intrusive_ptr<impl_t> impl;

public:
  typedef typename impl_t::list_t list_t;

public:
  inline
  stream_ptr() {}
  inline
  stream_ptr(const stream_ptr &o)
  : impl(o.impl) {}
  inline stream_ptr &
  operator=(const stream_ptr &o) {
    impl = o.impl;
    return *this;
  }

  inline void
  swap(stream_ptr &o) {
    std::swap(impl, o.impl);
  }

  explicit
  stream_ptr(iterator_base<T>* i)
  : impl(new impl_t(i)) {}

  inline
  ~stream_ptr() {}

private:
  inline
  stream_ptr(impl_t* i)
  : impl(i) {}

public:
  template<class It>
  static inline stream_ptr
  from_iter(const It &it) {
    return stream_ptr(new iter_stream<T,Mem>(it));
  }

  template<class Range>
  static inline stream_ptr
  from_range(const Range &r) {
    return stream_ptr(iter_stream<T,Mem>::from_range(r));
  }

  static inline stream_ptr
  from_stream_base(impl_t* s) {
    return stream_ptr(s);
  }

public:
  list_t &
  values()
  { return impl->values(); }
  const list_t &
  values() const
  { return impl->values(); }

public:
  typedef stream_iterator<T, Mem> iterator;
  typedef stream_iterator<T, Mem> const_iterator;

  const_iterator    begin() const
  { return stream_iterator<T, Mem>(impl.get()); }
  iterator_base<T>* iter() const
  { return detail::adapt(begin()); }
};

} // namespace streams


#endif /* STREAM_PTR_HXX_ */
