#ifndef EXPAIRSEQ_HANDLE_HPP
#define EXPAIRSEQ_HANDLE_HPP

#include<boost/intrusive_ptr.hpp>

namespace analysis {
namespace epseq {

template<class I, class M>
struct handle {

private:
  typedef expairseq<I, M> eps_t;

public: // cdtor
  handle(const eps_t*);

  handle(const handle &);

  handle &operator=(const handle &);

  ~handle() throw();

public: // coercion
  operator expr() const;

public: // operations
  handle operator+(const handle &o) const;
  handle operator-(const handle &o) const;
  handle operator-() const;
  handle sca(const number &n) const;

public: // tests & comparison
  bool null() const;

  static util::cmp_t compare(const handle &a, const handle &b);
  static util::cmp_t deep_compare(const handle &a, const handle &b);

  std::size_t hash() const;

public: // printing
  template<class S>
  void print(S &os) const;

  const eps_t* ptr() const throw();

private: // operations implementation
  eps_t* chg_coef(const number &n) const throw();

private: // member data
  //! \invariant Non null pointer
  boost::intrusive_ptr<const eps_t> m_ptr;
};

}} // namespace analysis::epseq

#endif
