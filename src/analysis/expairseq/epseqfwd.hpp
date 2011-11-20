#ifndef ANALYSIS_EPSEQFWD_HPP
#define ANALYSIS_EPSEQFWD_HPP

namespace analysis {

template<class Impl, class Mono>
class expairseq;

//! \brief Concept class for handle
template<class H, class Mono>
struct ExpairseqHandle;

namespace epseq {

template<class I, class M>
struct handle;

//! \brief Polynomial container embedded in the \ref expairseq
template<class T, class Alloc = std::allocator<T> >
class poly;

//! \brief Construct a vector by negation
template<class epair>
poly<epair>* do_neg(const poly<epair> &a, std::size_t &hash);

//! \brief Construct a vector by scaling
template<class epair>
poly<epair>* do_sca(const poly<epair> &a, const number &n, std::size_t &hash);

//! \brief Construct a vector by addition
template<class epair>
poly<epair>* do_add(const poly<epair> &a, const poly<epair> &b, std::size_t &hash);

//! \brief Construct a vector by subtraction
template<class epair>
poly<epair>* do_sub(const poly<epair> &a, const poly<epair> &b, std::size_t &hash);

//! \brief Construct a vector from an unsorted range of \c epair
template<class epair, class Iter>
poly<epair>* do_range_const(const Iter &beg, const Iter &end, std::size_t &hash);

//! \brief Construct a vector from an unsorted range of \c epair, inplace version
template<class epair, class Iter>
poly<epair>* do_range_mutable(Iter beg, const Iter &end, std::size_t &hash);

} // namespace epseq

} // namespace analysis

// declare specialization
namespace std {

template<class I, class M>
inline void swap(
  analysis::epseq::handle<I,M> &a
, analysis::epseq::handle<I,M> &b
);

} // namespace std

#endif