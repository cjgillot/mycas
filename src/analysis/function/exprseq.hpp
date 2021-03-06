#ifndef ANALYSIS_FUNCTION_EXPRSEQ_HPP
#define ANALYSIS_FUNCTION_EXPRSEQ_HPP

#include "analysis/expr.hpp"

#include "analysis/function/exprarray.hpp"

namespace analysis {

template<class Cont, bool InheritArray = false>
class exprseq;

template<class Cont>
class exprseq<Cont, false>
: public basic
{
  Cont m_container;

protected:
  typedef Cont container_type;
  typedef exprseq exprseq_;

protected:
  template<typename... Args>
  exprseq(Args&& ...args)
  : m_container( std::forward<Args>(args)... ) {}

  exprseq(const exprseq&) = default;
  ~exprseq() = delete;

public: // range
  container_type &get_container() { return m_container; }
  const container_type &get_container() const { return m_container; }

  typedef typename container_type::iterator               iterator;
  typedef typename container_type::const_iterator         const_iterator;
  typedef typename container_type::reverse_iterator       reverse_iterator;
  typedef typename container_type::const_reverse_iterator const_reverse_iterator;

  iterator begin() { return m_container.begin(); }
  iterator end  () { return m_container.end  (); }

  const_iterator begin() const { return m_container.begin(); }
  const_iterator end  () const { return m_container.end  (); }

  reverse_iterator rbegin() { return m_container.rbegin(); }
  reverse_iterator rend  () { return m_container.rend  (); }

  const_reverse_iterator rbegin() const { return m_container.rbegin(); }
  const_reverse_iterator rend  () const { return m_container.rend  (); }

  std::size_t size()  const { return m_container.size(); }
  bool        empty() const { return m_container.empty(); }

  typename container_type::reference       operator[](std::size_t i)       { return m_container[i]; }
  typename container_type::const_reference operator[](std::size_t i) const { return m_container[i]; }

  typename container_type::reference       at(std::size_t i)       { return m_container.at(i); }
  typename container_type::const_reference at(std::size_t i) const { return m_container.at(i); }

protected:
  std::size_t hash() const;

  util::cmp_t compare_same_type(const basic &o_) const;

  bool has(const symbol &s) const;

  void print_children(std::basic_ostream<char> &os) const;

  bool match_same_type(const basic &, match_state &) const;
  expr subs(const exmap &) const;
};

template<class Cont>
class exprseq<Cont, true>
: public exprarray
{
  Cont m_container;

protected:
  typedef Cont container_type;
  typedef exprseq exprseq_;

protected:
  template<typename... Args>
  exprseq(Args&& ...args)
  : m_container( std::forward<Args>(args)... ) {}

  exprseq(const exprseq&) = default;
  ~exprseq() = default;

public: // exprarray
  /*virtual*/      expr* pbegin()       { return   &*  m_container.begin(); }
  /*virtual*/      expr* pend  ()       { iterator e = m_container.end(); return 1+&*--e; }

  /*virtual*/const expr* pbegin() const { return   &*  m_container.begin(); }
  /*virtual*/const expr* pend  () const { const_iterator e = m_container.end(); return 1+&*--e; }

public: // range
  container_type &get_container() { return m_container; }
  const container_type &get_container() const { return m_container; }

  typedef typename container_type::iterator               iterator;
  typedef typename container_type::const_iterator         const_iterator;
  typedef typename container_type::reverse_iterator       reverse_iterator;
  typedef typename container_type::const_reverse_iterator const_reverse_iterator;

  iterator begin() { return m_container.begin(); }
  iterator end  () { return m_container.end  (); }

  const_iterator begin() const { return m_container.begin(); }
  const_iterator end  () const { return m_container.end  (); }

  reverse_iterator rbegin() { return m_container.rbegin(); }
  reverse_iterator rend  () { return m_container.rend  (); }

  const_reverse_iterator rbegin() const { return m_container.rbegin(); }
  const_reverse_iterator rend  () const { return m_container.rend  (); }

  std::size_t size()  const { return m_container.size(); }
  bool        empty() const { return m_container.empty(); }

  typename container_type::reference       operator[](std::size_t i)       { return m_container[i]; }
  typename container_type::const_reference operator[](std::size_t i) const { return m_container[i]; }

  typename container_type::reference       at(std::size_t i)       { return m_container.at(i); }
  typename container_type::const_reference at(std::size_t i) const { return m_container.at(i); }
};

} // namespace analysis

#endif
