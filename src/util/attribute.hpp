#include <boost/config.hpp>

// allow room for some optimizations

#ifndef ATTRIBUTE_PURE
#if defined(__GNUC__)
#define ATTRIBUTE_PURE __attribute__((pure))
#else
#define ATTRIBUTE_PURE
#endif
#endif

#ifndef ATTRIBUTE_CONST
#if defined(__GNUC__)
#define ATTRIBUTE_CONST __attribute__((const))
#else
#define ATTRIBUTE_CONST
#endif
#endif

#ifndef ATTRIBUTE_NONNULL
#if defined(__GNUC__)
#define ATTRIBUTE_NONNULL( arg ) __attribute__((nonnull( arg )))
#else
#define ATTRIBUTE_NONNULL( arg )
#endif
#endif

#ifndef ATTRIBUTE_NORETURN
#if defined(__GNUC__)
#define ATTRIBUTE_NORETURN __attribute__((noreturn))
#elif defined(MSC_VER)
#define ATTRIBUTE_NORETURN __declspec(noreturn)
#else
#define ATTRIBUTE_NORETURN
#endif
#endif

#ifndef ATTRIBUTE_DEPRECATED
#if defined(__GNUC__)
#define ATTRIBUTE_DEPRECATED __attribute__((deprecated))
#elif defined(MSC_VER)
#define ATTRIBUTE_DEPRECATED __declspec(deprecated)
#else
#define ATTRIBUTE_DEPRECATED
#endif
#endif

#ifndef ATTRIBUTE_DELETED
#ifndef BOOST_NO_DELETED_FUNCTIONS
#define ATTRIBUTE_DELETED = delete
#else
#define ATTRIBUTE_DELETED
#endif
#endif
