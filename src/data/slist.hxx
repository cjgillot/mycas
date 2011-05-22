/*
 * slist.hxx
 *
 *  Created on: 21 mai 2011
 *      Author: k1000
 */

#ifndef SLIST_HXX_
#define SLIST_HXX_

template<class T>
struct slist {
  T hd;
  slist<T>* tl;

  slist(const T &h, slist<T>* t):
    hd(h), tl(t)
  {}


};

#endif /* SLIST_HXX_ */
