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



inline bool number::fits_ulong_p() const
{
  typedef detail::wrapper<fmpz_t> int_type;

  if( !is_integer() ) // is not integer
    return false;

  const fmpz* p = boost::get<int_type>( m_impl )->get_fmpz_t();

  return fmpz_sgn( p ) >= 0 && fmpz_abs_fits_ui( p );
}
inline bool number::fits_slong_p() const
{
  typedef detail::wrapper<fmpz_t> int_type;

  if( !is_integer() ) // is not integer
    return false;

  const fmpz* p = boost::get<int_type>( m_impl )->get_fmpz_t();

  return fmpz_fits_si( p );
}


inline unsigned long number::get_ulong() const
{
  ASSERT( is_integer() );

  typedef detail::wrapper<fmpz_t> int_type;

  const fmpz* p = boost::get<int_type>( m_impl )->get_fmpz_t();
  return fmpz_get_ui( p );
}
inline   signed long number::get_slong() const
{
  ASSERT( is_integer() );

  typedef detail::wrapper<fmpz_t> int_type;

  const fmpz* p = boost::get<int_type>( m_impl )->get_fmpz_t();
  return fmpz_get_si( p );
}

}

#endif
