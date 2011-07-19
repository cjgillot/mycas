/*
 * utils/move.hpp
 *
 *  Created on: 14 juin 2011
 *      Author: k1000
 */

#ifndef UTILS_MOVE_HPP_
#define UTILS_MOVE_HPP_

namespace util {

/*!
 * \class move_ptr
 * \brief Utilitary class used as proxy for move semantics
 *
 * See usage in stream/iterator.hpp
 */
template<class T>
class move_ptr {
  mutable T* p;

  move_ptr();

public:
  //! \brief Constructor from pointer
  inline
  move_ptr(T* &p_)
  : p(p) { p_ = 0; }
  //! \brief Destructor
  inline
  ~move_ptr()
  { if(p) delete p; }

public:
  //! \brief Move copy constructor
  inline
  move_ptr(const move_ptr &o) {
    swap(o);
  }
  //! \brief Move assignment operator
  inline move_ptr &
  operator=(const move_ptr &o) {
    swap(o);
    return *this;
  }
  //! \brief Move non-throwing swap
  inline void
  swap(const move_ptr &o) const {
    std::swap(p, o.p);
  }

public:
  //! \brief Pointer access
  inline T*
  release() const {
    T* p_ = p;
    p = 0;
    return p_;
  }
};

} // namespace util

#endif /* UTILS_MOVE_HPP_ */
