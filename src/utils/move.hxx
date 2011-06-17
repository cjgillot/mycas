/*
 * utils/move.hxx
 *
 *  Created on: 14 juin 2011
 *      Author: k1000
 */

#ifndef UTILS_MOVE_HXX_
#define UTILS_MOVE_HXX_

namespace util {

template<class T>
class move_ptr {
  mutable T* p;

  inline
  move_ptr()
  : p(0) {}

public: /// ctor & dtor
  inline
  move_ptr(T* &p_)
  : p(p) { p_ = 0; }
  inline
  ~move_ptr()
  { if(p) delete p; }

public: /// copying
  inline
  move_ptr(const move_ptr &o) {
    swap(o);
  }
  inline move_ptr &
  operator=(const move_ptr &o) {
    swap(o);
    return *this;
  }

  inline void
  swap(const move_ptr &o) const {
    std::swap(p, o.p);
  }

public:
  inline T*
  release() const {
    T* p_ = p;
    p = 0;
    return p_;
  }
};

} // namespace util

#endif /* UTILS_MOVE_HXX_ */
