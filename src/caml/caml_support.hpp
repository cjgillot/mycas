#ifndef CAML_SUPPORT_HPP
#define CAML_SUPPORT_HPP

namespace caml {

struct initializer_t
{
  initializer_t();
  ~initializer_t();

  inline void touch() const {}

  static initializer_t instance;
};

}

#endif
