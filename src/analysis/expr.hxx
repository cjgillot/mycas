/*
 * expr.hxx
 *
 *  Created on: 19 juin 2011
 *      Author: k1000
 */

#ifndef EXPR_HXX_
#define EXPR_HXX_

#include "stdlib.hxx"
#include "analysis/basic.hxx"

#include "operators.hxx"

namespace analysis {

// uses Copy On Write idiom
class expr {
  boost::intrusive_ptr<basic> impl;

public:
  inline
  expr()
  : impl(0) {}

  inline
  expr(const expr &o)
  : impl(o.impl) {}

  inline expr&
  operator=(const expr &o) {
    impl=o.impl;
    return *this;
  }

  inline void
  swap(expr &o) {
    std::swap(impl, o.impl);
  }

  inline
  ~expr()
  {}

private:
  inline
  expr(basic* p)
  : impl(p) {}

  // copy before write function
  inline void
  cow() {
    if(!util::unique(impl.get()))
      impl=impl->clone();
  }

public:
  friend inline expr
  operator+(const expr &a, const expr &b) {
    return a.impl->add(b);
  }
  inline expr&
  operator+=(const expr &o) {
    cow(); a.impl->iadd(o);
    return *this;
  }
};

}

#endif /* EXPR_HXX_ */
