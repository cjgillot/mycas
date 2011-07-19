/*
 * map.hpp
 *
 *  Created on: 11 juin 2011
 *      Author: k1000
 */

#ifndef STREAM_MAP_HPP_
#define STREAM_MAP_HPP_

#include "stream/iterator.hpp"
#include "imperative/iterator/map.hpp"

namespace streams {

template<class It, class F>
inline iterator_base<
  typename imperative::iterator::map_type<It, F>::value_type
>*
map_iter(const It &a, const F &f) {
  return detail::adapt(imperative::iterator::map(a,f));
}

template<class T, class F>
inline iterator_base<T>*
map_base(iterator_base<T>* a, const F &f) {
  return streams::map_iter(meta_iterator<T>(a),f);
}

template<class T, class Mem, class F>
inline stream_ptr<T>
map(const stream_ptr<T,Mem> &a, const F &f) {
  return stream_ptr<T,Mem>::from_iter(map_base(a.iter(), f));
}

} // namespace streams


#endif /* STREAM_MAP_HPP_ */
