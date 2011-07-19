#ifndef FINAL_HPP
#define FINAL_HPP

namespace util {

#ifndef NDEBUG

#define FINAL_NAME(klass) \
  BOOST_PP_CAT(__finalize_, klass)

#define DECLARE_FINAL_CLASS(klass) \
  class klass; \
  class FINAL_NAME(klass) { \
  private: \
    virtual ~FINAL_NAME(klass)() {} \
    friend class klass; \
  };

// force final class via virtual inheritance
#define FINAL_CLASS(klass)  \
  public virtual FINAL_NAME(klass)

#else // defined NDEBUG

class empty_base {};

#define DECLARE_FINAL_CLASS(klass)
  // nothing

// no need of final class enforcing (DEBUG mode checked)
// we avoid the virtual inheritance overhead
#define FINAL_CLASS(klass)  \
  ::util::empty_base

#endif

}

#endif
