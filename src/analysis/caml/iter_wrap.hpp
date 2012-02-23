#ifndef ANALYSIS_CAML_ITERWRAP_HPP
#define ANALYSIS_CAML_ITERWRAP_HPP

#include "caml.hpp"

namespace _caml_expr {

class iterator
{
private:
  iterator& operator=(const iterator&);

protected:
  iterator(const iterator&) {}

public:
  iterator() {}
  virtual ~iterator() {}

  virtual std::size_t size() = 0;
  virtual void clone(void*) = 0;

  virtual void advance(std::size_t) = 0;
  virtual value deref() = 0;

  virtual util::cmp_t compare(iterator &);
};

value iter_allocate(std::size_t sz);

} // namespace _caml_expr

#define Iter_val( iter )  \
  reinterpret_cast<_caml_expr::iterator*>( Data_custom_val( iter ) )

#endif
