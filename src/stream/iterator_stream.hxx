/*
 * iterator_stream.hxx
 *
 *  Created on: 19 juin 2011
 *      Author: k1000
 */

#ifndef ITERATOR_STREAM_HXX_
#define ITERATOR_STREAM_HXX_

#include "utils.hxx"
#include "stream/streamfwd.hxx"
#include "stream/iterator.hxx"

namespace streams {

template<class T, class Mem>
class iter_stream
: public stream_base<T, Mem> {

public:
  typedef stream_base<T,Mem> super;
  typedef typename super::list_t list_t;

private:
  mutable meta_iterator<T> m_it;
  using super::m_val;
  using super::m_end;

private:
  iter_stream();

  template<class Range>
  inline
  iter_stream(iterator_base<T>* i, const Range &r)
  : super(r), m_it(i) {
    if(m_val.empty() && m_it)
      incr();
  }

public:
  inline explicit
  iter_stream(iterator_base<T>* i)
  : m_it(i) { incr(); }
  inline
  ~iter_stream() {}

public:
  template<class It>
  static inline iter_stream*
  from_iter(const It &it) {
    iterator_base<T>* a = detail::adapt(it);
    return new iter_stream(a);
  }

  template<class Range>
  static inline iter_stream*
  from_range(const Range &r) {
    return new iter_stream(0, r);
  }

private:
  friend class stream_iterator<T, Mem>;

  virtual
  typename list_t::iterator
  incr() {
    if(!m_it) return m_end=m_val.end();

    m_val.push_back(*m_it); ++m_it;
    m_end=--m_val.end();
    return m_end++;
  }
};


} // namespace streams

#include "stream/stream_iterator.hxx"


#endif /* ITERATOR_STREAM_HXX_ */
