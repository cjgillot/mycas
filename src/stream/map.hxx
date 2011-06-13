/*
 * map.hxx
 *
 *  Created on: 11 juin 2011
 *      Author: k1000
 */

#ifndef STREAM_MAP_HXX_
#define STREAM_MAP_HXX_

#include "stream/iterator.hxx"
#include "imperative/iterator/map.hxx"

namespace streams {

template<class It, class F>
inline iterator_base<
  typename imperative::iterator::map_type<It, F>::value_type
>*
map(const It &a, const F &f) {
  return detail::adapt(imperative::iterator::map(a,f));
}

template<class T, class F>
inline iterator_base<T>*
map(iterator_base<T>* a, const F &f) {
  return map(meta_iterator<T>(a),f);
}

template<class T, class F>
inline stream<T>
map(const stream<T> &a, const F &f) {
  return stream<T>(map(a.iter(), f));
}

} // namespace streams


#endif /* STREAM_MAP_HXX_ */
