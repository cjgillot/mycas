#include<boost/preprocessor/seq/for_each.hpp>

#define REALFWD_NO_UNDEF

#include "numeric/real/realfwd.hpp"

#include "real_t.hpp"

namespace numeric {

struct real_t;

const real_t* real_t::zero()
{
  static boost::intrusive_ptr<const real_t> value ( new real_t( 0 ) );
  return value.get();
}
const real_t* real_t::one()
{
  static boost::intrusive_ptr<const real_t> value ( new real_t( 1 ) );
  return value.get();
}

#define FLINT_CTOR(_1, _2, type)  \
  real_t* make_repr(type i) \
  { return new real_t(i); }

BOOST_PP_SEQ_FOR_EACH(FLINT_CTOR, _, FLINTS)

}
