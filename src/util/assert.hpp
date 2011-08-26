#ifndef UTIL_ASSERT_HPP
#define UTIL_ASSERT_HPP

#include <boost/assert.hpp>

#ifndef NDEBUG
# define ASSERT               BOOST_ASSERT
# define ASSERT_MSG           BOOST_ASSERT_MSG
#else
# define ASSERT( e )          (void)0
# define ASSERT_MSG( e, m )   (void)0
#endif

#include <boost/static_assert.hpp>

#define STATIC_ASSERT BOOST_STATIC_ASSERT

#endif
