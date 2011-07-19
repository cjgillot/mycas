#ifndef UTILS_VISITOR_HPP
#define UTILs_VISITOR_HPP

namespace util {

class base_visitor {
  virtual ~base_visitor() {}
};

// T may be a class, or const class
template<class T, class Ret = void>
struct visitor {
  typedef Ret return_type;
  typedef T param_type;

  virtual ~visitor() {}
  virtual return_type visit(param_type&) = 0;
};

template<typename R, class Visited>
struct default_catch_all {
  static inline R
  on_unknown(Visited &, base_visitor &)
  { return R(); }
};

template<typename R, template<typename, class> class CatchAll = default_catch_all>
class base_visitable
{
public:
  typedef R return_type;
  virtual ~base_visitable() {}
  virtual return_type accept(base_visitor&) = 0;

protected:
  // give access only to the hierarchy
  template<class T>
  static return_type accept_impl(T& visited, base_visitor& guest)
  {
    // Apply the Acyclic Visitor
    if (visitor<T,R>* p = dynamic_cast<visitor<T,R>*>(&guest))
    {
        return p->visit(visited);
    }
    return CatchAll<R, T>::on_unknown(visited, guest);
  }
};

template<typename R, template<typename, class> class CatchAll = default_catch_all>
class base_const_visitable
{
public:
  typedef R return_type;
  virtual ~base_const_visitable() {}
  virtual return_type accept(base_visitor&) const = 0;

protected:
  // give access only to the hierarchy
  template<class T>
  static return_type accept_impl(T& visited, base_visitor& guest)
  {
    // Apply the Acyclic Visitor
    if (visitor<T,R>* p = dynamic_cast<visitor<T,R>*>(&guest))
    {
        return p->visit(visited);
    }
    return CatchAll<R, T>::on_unknown(visited, guest);
  }
};


#define DEFINE_VISITABLE() \
public: \
  virtual return_type accept(::util::base_visitor& guest) \
  { return accept_impl(*this, guest); }

#define DEFINE_CONST_VISITABLE() \
public: \
  virtual return_type accept(::util::base_visitor& guest) const \
  { return accept_impl(*this, guest); }

}

#endif
