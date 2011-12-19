#ifndef SYMBOL_HPP_
#define SYMBOL_HPP_

#include "analysis/expr.hpp"
#include "analysis/basic.hpp"
#include "analysis/number.hpp"

namespace analysis {

class symbol;

//! \brief Anonymous symbol class
class symbol_
: public basic {

  REGISTER_CLASS( symbol_, basic )

  friend class symbol;

protected:
  //! \brief Default constructor
  symbol_();
  //! \brief Destructor
  ~symbol_();

protected:
  symbol_* clone() const;

protected:
  void print(std::basic_ostream<char> &) const;
  bool has(const symbol&) const;

  expr subs(const exmap &) const;

private:
  expr differentiate(const symbol&) const;
  std::size_t hash() const;
  util::cmp_t compare_same_type(const basic&) const;
};

//! \brief Named symbol class
class ident
: public symbol_ {

  REGISTER_CLASS( ident, symbol_ )

  friend class symbol;

protected:
  //! \brief Constructor from string
  ident(const std::string &);
  //! \brief Destructor
  ~ident();

protected:
  void print(std::basic_ostream<char> &) const;

private:
  std::string m_name;
};

//! \brief Constant symbol class
class constant
: public ident {

  REGISTER_CLASS( constant, ident )

  friend class symbol;

protected:
  //! \brief Constructor from string
  constant(const std::string &, const number &);
  //! \brief Destructor
  ~constant();

private:
  number m_value;
};

//! \brief Symbol handler
struct symbol
: public expr {

  friend class symbol_;

  symbol()
  : expr( new symbol_ ) {}

  symbol(const std::string &s)
  : expr( new ident(s) ) {}

  void swap(symbol &o)
  { expr::swap( o ); }

  const symbol_* get() const
  { return static_cast<const symbol_*>( expr::get() ); }

  std::size_t hash() const
  { return get()->symbol_::hash(); }

  static util::cmp_t compare(const symbol &a, const symbol &b)
  {
    // equivalent to the hash comparison in this case
    return a.get()->symbol_::compare_same_type( *b.get() );
  }
};

}

#endif /* SYMBOL_HPP_ */
