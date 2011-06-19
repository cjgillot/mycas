/*
 * tuple.ixx
 *
 *  Created on: 13 juin 2011
 *      Author: k1000
 */

#ifndef TUPLE_IXX_
#define TUPLE_IXX_

#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_shifted.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>

#ifndef MAX_TUPLE_ARITY
#define MAX_TUPLE_ARITY 5
#endif

namespace tuple {

// tuples

#define ARGS(z, n, _) BOOST_PP_CAT(arg,n)
#define DECL(z, n, _) const T &ARGS(z, n, _)

#define TUPLE(z, N, _)                  \
template<class T>                       \
inline typename tuple_t<N, T>::type     \
tuple(BOOST_PP_ENUM_##z(N, DECL, _)) {  \
  return cons(arg0, tuple(              \
    BOOST_PP_ENUM_SHIFTED_##z(          \
      N, ARGS, _                        \
    )                                   \
  ));                                   \
}

//! \brief Empty case {tuple()} maker
inline nil_t
tuple() {
  return nil;
}

//! \brief Preprocessor loop declaring {tuple(...)} makers
BOOST_PP_REPEAT_FROM_TO(1, MAX_TUPLE_ARITY, TUPLE, _)

#undef TUPLE
#undef ARGS
#undef DECL

// ties

#define ARGS(z, n, _) BOOST_PP_CAT(arg,n)
#define DECL(z, n, _) T &ARGS(z, n, _)

#define TIE(z, N, _)                    \
template<class T>                       \
inline typename tuple_t<N, T&>::type    \
tie(BOOST_PP_ENUM_##z(N, DECL, _)) {    \
  return cons_ref(arg0, tie(            \
    BOOST_PP_ENUM_SHIFTED_##z(          \
      N, ARGS, _                        \
    )                                   \
  ));                                   \
}

//! \brief Empty case {tie()} maker
inline nil_t
tie() {
  return nil;
}

//! \brief Preprocessor loop declaring {tie(...)} makers
BOOST_PP_REPEAT_FROM_TO(1, MAX_TUPLE_ARITY, TIE, _)

#undef TIE
#undef ARGS
#undef DECL

}

#endif // TUPLE_IXX_
