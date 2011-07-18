/*
 * ptr.hpp
 *
 *  Created on: 4 juil. 2011
 *      Author: k1000
 */

#ifndef PTR_HXX_
#define PTR_HXX_

#include "stdlib.hpp"

namespace analysis {

// uses Copy On Write idiom
template<class T>
class ptr {

private:
  //! \invariant m_ptr != 0
  mutable boost::intrusive_ptr<const T> m_ptr;

private:
  //! \brief Disabled default constructor
  ptr();

public:
  //! \brief Copy constructor
  ptr(const ptr &);

  //! \brief Assignement operator
  ptr &operator=(const ptr&);

  //! \brief Non-throwing swap
  void swap(ptr &);

  //! \brief Destructor
  ~ptr();

  //! \brief Explicit pointer constructor
  explicit
  ptr(const T*);

public:
  /*!
   * \brief Copy-On-Write function
   *
   * This function shall be called before any
   * modifying operation on the pointed object.
   * This call is the only way to get a non-const pointer.
   */
  T* cow();

  //! \brief Pointer access
  const T* get() const
  { return m_ptr.get(); }

  //! \brief Pointer modification
  void reset(const T* p) // const
  { assert(p); m_ptr.reset(p); }


  //! \brief Nullity test
  bool null() const;
  //! \brief Unity test
  bool unit() const;

  //! \brief Evaluation function
  const T*
  eval(unsigned lv) const;


  //! \brief Printing function
  void print(std::basic_ostream<char> &os) const;


  //! \brief Comparison template function
  template<class Compare>
  static int
  compare(const ptr &a, const ptr &b, Compare cmp);
};

}


#endif /* PTR_HXX_ */
