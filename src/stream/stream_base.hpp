/*
 * stream_base.hpp
 *
 *  Created on: 19 juin 2011
 *      Author: k1000
 */

#ifndef STREAM_BASE_HXX_
#define STREAM_BASE_HXX_

#include "utils.hpp"
#include "stream/streamfwd.hpp"
#include "stream/iterator.hpp"

namespace streams {

template<class T, class Mem>
class stream_base
: public util::refcounted
, protected boost::noncopyable {

public:
  typedef Mem list_t;
  typedef typename list_t::iterator iterator;

protected:
  list_t m_val;
  iterator m_end;

protected:
  inline
  stream_base()
  {}

  template<class Range>
  inline explicit
  stream_base(const Range &r)
  : m_val(boost::begin(r), boost::end(r))
  , m_end(boost::end(m_val))
  {}

public:
  inline virtual
  ~stream_base() {}

public:
  list_t &
  values()
  { return m_val; }
  const list_t &
  values() const
  { return m_val; }

  const iterator &
  end() const
  { return m_end; }

protected:
  friend class stream_iterator<T, Mem>;

  virtual iterator
  incr() = 0;
};

} // namespace streams

#include "stream/stream_iterator.hpp"


#endif /* STREAM_BASE_HXX_ */
