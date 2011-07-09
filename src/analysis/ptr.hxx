/*
 * ptr.hxx
 *
 *  Created on: 4 juil. 2011
 *      Author: k1000
 */

#ifndef PTR_HXX_
#define PTR_HXX_

#include "stdlib.hxx"
#include "utils/refcounted.hxx"

namespace analysis {

// uses Copy On Write idiom
template<class T>
class ptr {

private:
  mutable boost::intrusive_ptr<const T> m_ptr;

public:
  ptr(): m_ptr(0) {}

  ptr(const ptr &o)
  : m_ptr(o.m_ptr) {}
  ptr &operator=(const ptr &o) {
    m_ptr=o.m_ptr;
    return *this;
  }

  void swap(ptr &o)
  { m_ptr.swap(o.m_ptr); }

  virtual ~ptr() {}

  explicit
  ptr(const T* p)
  : m_ptr(p) {}

protected:
  // copy before write function
  T* cow() {
    if(!util::unique(m_ptr.get()))
      m_ptr = m_ptr->clone();
    return const_cast<T*>(m_ptr.get());
  }

  const T* get() const {
    return m_ptr.get();
  }

  void reset(const T* p) const {
    m_ptr.reset(p);
  }

public:
  bool null() const {
    if(!m_ptr)
      return true;
    if(m_ptr->null()) {
      m_ptr.reset();
      return true;
    }
    return false;
  }
  bool unit() const {
    return m_ptr && m_ptr->unit();
  }

  const T*
  eval(unsigned lv) const {
    const T* p = static_cast<const T*>(m_ptr->eval(lv));
    if(m_ptr.get() != p)
      m_ptr.reset(p);
    return p;
  }

public:
  void print(std::basic_ostream<char> &os) const {
    if(m_ptr)
      m_ptr->print(os);
    else
      os << 0;
  }

protected:
  virtual int
  do_compare(const T&)
    const = 0;

public:
  static int
  compare(const ptr &a, const ptr &b) {
    if(a.m_ptr == b.m_ptr)
      return 0;

    if(! b.m_ptr)
      return (bool)a.m_ptr;

    if(! a.m_ptr)
      return -1;

    int c = a.do_compare(*b.m_ptr);

    if(c) return c;

    // here, a == b
    util::unify_ptr(a.m_ptr, b.m_ptr);

    return 0;
  }
};

}


#endif /* PTR_HXX_ */
