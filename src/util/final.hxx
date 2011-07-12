#ifndef FINAL_HXX
#define FINAL_HXX

namespace util {

#ifndef NDEBUG

#define DECLARE_FINAL_CLASS(klass) \
  class klass; \
  class __finalize_##klass { \
  private: \
    ~__finalize_##klass() {} \
    friend class klass; \
  } /* missing ; */

// force final class via virtual inheritance
#define FINAL_CLASS(klass)  \
  public virtual __finalize_##klass

#else // defined NDEBUG

class empty_base {};

#define DECLARE_FINAL_CLASS(klass)

// no need of final class enforcing (DEBUG mode checked)
// we avoid the virtual inheritance overhead
#define FINAL_CLASS(klass)  \
  empty_base

#endif

}

#endif
