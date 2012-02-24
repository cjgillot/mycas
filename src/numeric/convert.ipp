#ifndef NUMERIC_CONVERT_IPP
#define NUMERIC_CONVERT_IPP

#include "number.hpp"

namespace numeric {

inline bool number::is_integer() const
{ return m_impl.which() == 0; }

// integer or rational
inline bool number::is_rational() const
{ return m_impl.which() < 2; }

inline bool number::is_floating() const
{ return m_impl.which() == 2; }

// representation access
inline const fmpz* number::get_fmpz_t() const
{
  ASSERT( is_integer() );

  typedef detail::wrapper<fmpz_t> int_type;
  return boost::get<int_type>( m_impl )->get_fmpz_t();
}
inline const fmpq* number::get_fmpq_t() const
{
  ASSERT( is_rational() );

  typedef detail::wrapper<fmpq_t> rat_type;
  return boost::get<rat_type>( m_impl )->get_fmpq_t();
}
inline mpfr_srcptr number::get_mpfr_t() const
{
  ASSERT( is_floating() );

  typedef detail::wrapper<mpfr_t> flt_type;
  return boost::get<flt_type>( m_impl )->get_mpfr_t();
}

// machine integer query
inline bool number::fits_ulong_p() const
{
  if( !is_integer() ) // is not integer
    return false;

  const fmpz* p = this->get_fmpz_t();
  return fmpz_sgn( p ) >= 0 && fmpz_abs_fits_ui( p );
}
inline bool number::fits_slong_p() const
{
  if( !is_integer() ) // is not integer
    return false;

  const fmpz* p = this->get_fmpz_t();
  return fmpz_fits_si( p );
}

inline unsigned long number::get_ulong() const
{
  const fmpz* p = this->get_fmpz_t();
  return fmpz_get_ui( p );
}
inline   signed long number::get_slong() const
{
  const fmpz* p = this->get_fmpz_t();
  return fmpz_get_si( p );
}

} // namespace numeric

#endif
