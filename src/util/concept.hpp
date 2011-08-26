#ifndef UTIL_CONCEPT_HPP
#define UTIL_CONCEPT_HPP

#include <boost/concept_check.hpp>
#define CONCEPT_ASSERT BOOST_CONCEPT_ASSERT
#define CONCEPT_USAGE  BOOST_CONCEPT_USAGE

#include "util/assert.hpp"

namespace util {

//! \brief Automatic concept checking at destruction
//! \warning Breaks POD-ness
template<class Concept>
struct implement_concept {

  ~implement_concept() throw()
  { check(); }

private:
  void check() const
  { CONCEPT_ASSERT(( Concept )); }
};

} // namespace util

#endif
