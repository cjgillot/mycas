#ifndef NUMBER_HPP
#define NUMBER_HPP

#include "stdlib.hpp"

#include "operators.hpp"

namespace {

template<class I, class R = void>
struct enable_if_int
: boost::enable_if<
    typename boost::is_integral<I>::type
,   R
> {};

template<class I, class R = void>
struct enable_if_float
: boost::enable_if<
    typename boost::is_float<I>::type
,   R
> {};

}

namespace numeric {

class number
: private boost::field_operators1<number
, boost::incrementable<number
, operators::printable<number
> > > {

public:
  template<class Int>
  number(Int, typename enable_if_int<Int>::type* = 0);

  template<class Float>
  number(Float, typename enable_if_float<Float>::type* = 0);

  number(const number &);
  number &operator=(const number &);

  void swap(number &o)
  { m_repr.swap(o.m_repr); }

  ~number();

public:
  static number zero;
  static number one;

public:
  /*!
   * \name Modifying operators
   * \{
   */
  number &operator++();
  number &operator--();

  number &operator+=(const number &);
  number &operator-=(const number &);

  number &operator*=(const number &);
  number &operator/=(const number &);

  number &ineg();
  number &iinv();
  /*! \} */

  /*!
   * \name Persistent operators
   * \{
   */
  number neg() const;
  number inv() const;

  number pow(const number &) const;
  /*! \} */

public:
  bool null() const;
  bool unit() const;

public:
  void print(std::basic_ostream<char> &) const;

  static int
  compare(const number &, const number &);

  std::size_t
  get_hash() const;

public:
  struct repr_t;

private:
  explicit number(repr_t*);

  boost::intrusive_ptr<const repr_t> m_repr;
};

} // namespace numeric

#include "repr.hpp"

namespace numeric {

inline
number::number(const number &o)
: m_repr(o.m_repr) {}

inline number&
number::operator=(const number &o)
{
  m_repr = o.m_repr;
  return *this;
}

inline
number::~number() {}

inline
number::number(repr_t* p)
: m_repr(p) {}


template<class Int>
inline
number::number(Int i, typename enable_if_int<Int>::type*)
: m_repr(new repr_t(i)) {}

template<class Float>
inline
number::number(Float f, typename enable_if_float<Float>::type*)
: m_repr(new repr_t(f)) {}


}

#endif
