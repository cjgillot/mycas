/*
 * data.hpp
 *
 *  Created on: 30 juin 2011
 *      Author: k1000
 */

#ifndef MAGMA_DATA_HXX_
#define MAGMA_DATA_HXX_

#include "utils/refcounted.hpp"

namespace structures {
namespace magma {

class tree
: public util::refcounted {

protected:
  tree() {}
  virtual ~tree() {}

public:
  virtual bool  is_leaf() const = 0;
  virtual tree* left () const = 0;
  virtual tree* right() const = 0;
  virtual void* value() const = 0;
};

class node
: public tree {

  boost::intrusive_ptr<tree> m_left, m_right;

private:
  node();

public:
  node(tree* l, tree* r)
  : m_left(l), m_right(r)
  { assert(l && r); }

  ~node() {}

public:
  bool  is_leaf() const { return false; }
  tree* left()  const { return m_left .get(); }
  tree* right() const { return m_right.get(); }
  void* value() const { return 0; }
};

template<class T>
class leaf
: public tree {

  T m_val;

private:
  leaf();

public:
  leaf(const T &a)
  : m_val(a) {}

  ~leaf() {}

public:
  bool  is_leaf() const { return true; }
  tree* left()  const { return 0; }
  tree* right() const { return 0; }
  void* value() const { return static_cast<void*>(&m_val); }
};

}} // namespace structures::magma

#endif /* MAGMA_DATA_HXX_ */
