#ifndef FUNCTION_IXX
#define FUNCTION_IXX

#include "stdlib.hxx"

#include "analysis/function/function.hxx"

namespace analysis {

template<unsigned N>
template<class InputIterator>
function<N>::function(const std::string &n,
  const InputIterator &b,
  const InputIterator &e
)
: m_name(n), m_args(b,e)
{ assert(std::distance(b,e) == N); }

template<unsigned N>
function<N>::function(const function &o)
: m_name(o.m_name), m_args(o.m_args) {}

template<unsigned N>
function<N> &
function<N>::operator=(const function &o) {
  m_name = o.m_name;
  m_args = o.m_args;
  return *this;
}

template<unsigned N>
void
function<N>::swap(function &o) {
  m_name.swap(o.m_name);
  m_args.swap(o.m_args);
}


template<unsigned N>
inline void
function<N>::sort()
{ std::sort(m_args.begin(), m_args.end()); }

namespace {

// utility iterator class for sign_sort
template<class C>
struct back_inserter_count
: std::iterator<std::output_iterator_tag, void, void, void, void> {
  C &cont;
  unsigned index;

  explicit
  back_inserter_count(C &c)
  : cont(c), index(0) {}

  template<class T>
  back_inserter_count&
  operator=(const T &v) {
    cont.push_back(std::make_pair(v, index++));
    return *this;
  }

  back_inserter_count&
  operator++()
  { return *this; }

  back_inserter_count&
  operator*()
  { return *this; }
};

template<class C>
inline back_inserter_count<C>
back_insert_count(C &c)
{ return back_inserter_count<C>(c); }

// partial ordering on std::pair<expr, unsigned>
struct less_pair {

  typedef std::pair<expr, unsigned> arg_t;

  inline bool operator()(const arg_t &a, const arg_t &b)
  { return a.first < b.first; }

};

} // anonymous namespace

template<unsigned N>
inline bool
function<N>::sign_sort() {
  std::vector<unsigned> perm;
  perm.resize(N);

  {
    // sort an indexed vector
    std::vector<std::pair<expr, unsigned> > tmp;
    tmp.reserve(N);

    boost::copy(m_args, back_insert_count(tmp));

    std::sort(tmp.begin(), tmp.end(), less_pair());

    boost::copy(tmp,
      boost::make_zip_iterator(boost::make_tuple(m_args.begin(), perm.begin()))
    );
  }

  // now compute the sign, via the number of even cycles
  bool sign = 0; // false for 1, true for -1

  std::vector<unsigned>::iterator
    it = perm.begin(), end = perm.end();
  for(; it != end; ++it) {
    // already parsed cycle
    if(*it == -1) continue;

    unsigned start = *it,
      curr = start,
      length = 0;
    do {
      unsigned tmp = perm[curr];
      perm[curr] = -1; // flag traversed
      ++length;

      curr = tmp;
    }
    while(curr != start);

    // one more cycle
    // change {sign} if {length} is even
    if( !(length & 1) ) sign = ! sign;
  }

  return sign;
}

}

#endif
