/*
 * utils/refcounted.hxx
 *
 *  Created on: 14 juin 2011
 *      Author: k1000
 */

#ifndef UTILS_REFCOUNTED_HXX_
#define UTILS_REFCOUNTED_HXX_

namespace util {

class refcounted {
  mutable unsigned char refcount;

protected:
  inline
  refcounted()
  : refcount(0) {}
  inline
  refcounted(const refcounted &o)
  : refcount(0) {}
  inline refcounted &
  operator=(const refcounted &o) {
    refcount = 0;
    return *this;
  }

  inline virtual
  ~refcounted() {}

public:
  friend inline void
  intrusive_ptr_add_ref(const refcounted* it) {
    assert(it != 0);
    ++it->refcount;
  }
  friend inline void
  intrusive_ptr_release(const refcounted* it) {
    assert(it != 0);
    if(--it->refcount == 0)
      delete it;
  }
};

}

#endif /* UTILS_REFCOUNTED_HXX_ */
