#ifndef REALFWD_HPP
#define REALFWD_HPP

#include<boost/utility/enable_if.hpp>
#include<boost/type_traits/is_float.hpp>
#include<boost/type_traits/is_integral.hpp>

#include<boost/preprocessor/seq/for_each.hpp>

namespace numeric {

namespace {

struct type {};

template<typename T>
struct enable_if_int
: boost::enable_if_c<
    boost::is_integral<T>::value
  , void(type::*)()
> {};

template<typename T>
struct enable_if_float
: boost::enable_if_c<
    boost::is_float<T>::value
  , void(type::*)()
> {};

template<typename T>
struct enable_if_flint
: boost::enable_if_c<
    boost::is_integral<T>::value
 || boost::is_float<T>::value
  , void(type::*)()
> {};

}

struct real_t;

void intrusive_ptr_add_ref(const real_t*);
void intrusive_ptr_release(const real_t*);

#define FLINTS \
  (signed short)(unsigned short) \
  (signed int)  (unsigned int)   \
  (signed long) (unsigned long)  \
  (float)(double)  \
/*  (long long)(signed long long)(unsigned long long) */ \
/*  (long double) */

#define FLINT_CTOR(_1, _2, type)  \
  real_t* make_repr(type);

BOOST_PP_SEQ_FOR_EACH(FLINT_CTOR, _, FLINTS)

#undef FLINT_CTOR

#ifndef REALFWD_NO_UNDEF
#  undef FLINTS
#endif

}

#endif
