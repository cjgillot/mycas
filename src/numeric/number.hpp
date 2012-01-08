#ifndef NUMERIC_NUMBER_HPP
#define NUMERIC_NUMBER_HPP

#include "operators.hpp"

#include "util/compare.hpp"

#include "boost/variant.hpp"

#include "mp_operators.hpp"

namespace numeric {

namespace detail {

template<class T>
struct wrapper {
  typedef __gmp_expr<T,T> value_type;
  value_type value;

  wrapper()
  : value() {}

  template<class U>
  wrapper(const __gmp_expr<T,U> &o)
  : value( o ) {}

  const value_type& operator*() const { return value; }
  value_type& operator*() { return value; }

  const value_type* operator->() const { return &value; }
  value_type* operator->() { return &value; }
};

typedef boost::variant<
  wrapper<fmpz_t>,
  wrapper<fmpq_t>,
  wrapper<mpfr_t>
> impl_t;

}

/*!\brief Number class
 */
class number
: public boost::field_operators1<number
, operators::printable<number
, operators::ordered<number
> > > {

public:
  number()
  : m_impl() {}

  number(const number &o)
  : m_impl( o.m_impl ) {}

  number &operator=(const number &o)
  {
    m_impl = o.m_impl;
    return *this;
  }

  //! \brief Non-throwing swap
  void swap(number &o)
  { m_impl.swap( o.m_impl ); }

  //! \brief Non-virtual destructor
  ~number() throw() {}

public:
  /*! \name Coercion constructors
   * \{ */
  //@{
  template<class T, class U>
  number(const __gmp_expr<T,U> &ex)
  : m_impl( ex ) {}
  template<class T, class U>
  number& operator=(const __gmp_expr<T,U> &ex)
  { m_impl = ex; return *this; }

#define CTOR_INT( type )          \
  number(type i)                  \
  : m_impl( fmpz_class( i ) ) {}  \
  number &operator=( type i )     \
  { m_impl = fmpz_class( i ); return *this; }

  CTOR_INT(int)       CTOR_INT(unsigned int)
  CTOR_INT(long)      CTOR_INT(unsigned long)
//   CTOR_INT(long long) CTOR_INT(unsigned long long)

#undef CTOR_INT

#define CTOR_FLT( type )          \
  number(type i)                  \
  : m_impl( mpfr_class( i ) ) {}  \
  number &operator=( type i )     \
  { m_impl = mpfr_class( i ); return *this; }

  CTOR_FLT( float )
  CTOR_FLT( double )
  CTOR_FLT( long double )

#undef CTOR_FLT
  //@}
  /*! \} */

public:
  //! \brief Nullity test
  bool null() const;
  //! \brief Unity test
  bool unit() const;

  //! \brief Printing
  void print(std::ostream &os) const;

public:
  /*! \name Type query
   * \}*/
  //@{
  bool is_integer() const;
  bool is_rational() const;
  bool is_floating() const;
  //@}
  /*!\}*/

public:
  /*! \name Integer conversion
   * \{ */
  //@{
  bool fits_ulong_p() const;
  bool fits_slong_p() const;

  unsigned long get_ulong() const;
    signed long get_slong() const;
  //@}
  /*!\}*/

  //@{
public:
  number neg() const { return number(*this).ineg(); }
  number inv() const;

  static number add(const number&, const number&);
  static number sub(const number&, const number&);

  static number mul(const number&, const number&);
  static number div(const number&, const number&);

//   static number quo(const number&, const number&);
//   static number mod(const number&, const number&);

  number pow(const number &) const;

public:
  number &ineg();
  number &iinv() {
    inv().swap( *this );
    return *this;
  }

  number operator-() const
  { return neg(); }

#define OP_EQ( op, name )       \
  number &                        \
  operator op##=(const number &o) \
  {                             \
    number::name( *this, o )      \
      .swap( *this );           \
    return *this;               \
  }

  OP_EQ( +, add )
  OP_EQ( -, sub )
  OP_EQ( *, mul )
  OP_EQ( /, div )

#undef OP_EQ
  //@}

public:
  static util::cmp_t
  compare(const number &a, const number &b);

  std::size_t hash() const;

private:
  detail::impl_t m_impl;
};

inline std::ostream &
operator<<(std::ostream &os, const number &n)
{
  n.print(os);
  return os;
}


} // namespace numeric

namespace std {

template<>
inline void
swap<numeric::number>(numeric::number &a, numeric::number &b)
{ a.swap( b ); }

}

#include "convert.ipp"

#endif
