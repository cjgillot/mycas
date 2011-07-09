/*
 * heap.hxx
 *
 *  Created on: 21 mai 2011
 *      Author: k1000
 */

#ifndef HEAP_HXX_
#define HEAP_HXX_

#include "stdlib.hxx"
#include "operators.hxx"

namespace imperative {
namespace heap {

/*!
 * \class heap
 * \brief standard _max_-heap class
 *     implemented on top of std::*_heap
 *
 * O must be less_than_comparable,
 * assignable and copyable
 *
 * all references returned by find_max* functions
 * are invalidated by insert and detete_max* calls
 */
template<class O>
class heap {
  BOOST_STATIC_ASSERT(!boost::is_pointer<O>::value);

protected:
  //! \brief Implementation container type
  typedef std::vector<O> vect_t;

  //! \brief Implementation container
  vect_t data;

public:
  /*!
   * \brief Default constructor
   *
   * Creates an empty heap
   */
  inline
  heap() {}

public:
  /*!
   * \brief Copy constructor
   */
  inline
  heap(const heap &o)
  : data(o.data) {}
  /*!
   * \brief Assignment operator
   */
  inline heap &
  operator=(const heap &o) {
    data = o.data;
    return *this;
  }
  /*!
   * \brief Non-throwing swap
   */
  inline void
  swap(heap &o) {
    std::swap(data, o.data);
  }

  /*!
   * \brief Sized constructor
   *
   * Construct empty heap and reserves size
   *
   * \param n : the reserved size
   */
  explicit inline
  heap(size_t n)
  : data()
  { data.reserve(n); }

  /*!
   * \brief Destructor
   */
  inline
  ~heap() {}

public:
  /*!
   * \brief Emptiness test
   *
   * constant time.
   */
  inline bool
  empty() const {
    return data.empty();
  }

  /*!
   * \brief Maximum element access
   *
   * constant time.
   *
   * \return Reference to the maximum element
   */
  inline O &
  find_max() {
    assert(! empty());
    return data.front();
  }
  /*!
   * \brief Maximum element access, const version
   *
   * constant time.
   *
   * \return Reference to the maximum element
   */
  inline const O &
  find_max() const {
    assert(! empty());
    return data.front();
  }

public:
  /*!
   *  \brief Insertion
   *
   *  \param x : the element to be inserted
   *
   *  Inserts a new element into the heap.
   *  logarithmic time.
   */
  inline void
  insert(const O &x) {
    data.push_back(x);
    boost::push_heap(data);
  }

  /*!
   * \brief Maximum deletion
   *
   * Deletes the maximum element from the heap.
   * logarithmic time.
   */
  inline void
  delete_max() {
    assert(data.size() > 0);

    boost::pop_heap(data);
    data.pop_back();
  }
};

namespace detail {

/*!
 * \class list
 *
 * \brief list pointer proxy for chain class
 *
 * The struct behaves like a raw pointer :
 * there is no allocation/deallocation caring.
 * Have been added :
 * - one forwarding contructor
 * - the front() element comparison function
 */
template<class O>
class list
: operators::ordered<list<O> > {

public:
  //! \brief Pointed list type
  typedef std_ext::slist<O> impl_t;

private:
  impl_t* ptr;

public:
  /*!
   * \brief Default construction
   */
  inline
  list()
  : ptr(0) {}

public:
  /*!
   * \brief Copy constructor
   */
  inline
  list(const list &o)
  : ptr(o.ptr) {}
  /*!
   * \brief Assignment operator
   */
  inline list &
  operator=(const list &o) {
    ptr=o.ptr;
    return *this;
  }
  /*!
   * \brief Non-throwing swap
   */
  inline void
  swap(list &o) {
    std::swap(ptr, o.ptr);
  }

public:
  /*!
   * \brief Forwarded constructor
   *
   * \param n : list size
   * \param x : list element
   *
   * This call is directly passed to
   * the corresponding list constructor.
   */
  inline
  list(std::size_t n, const O &x)
  : ptr(new impl_t(n,x)) {}

public:
  /*!
   * \brief Destructor
   */
  inline
  ~list() {}

  /*!
   * \brief Pointer deallocation
   */
  inline void
  destroy()
  { delete ptr; }

public:
  /*!
   * \brief List dereference operator
   */
  inline impl_t &
  operator*() const
  { return *ptr; }

  /*!
   * \brief List member dereference operator
   */
  inline impl_t*
  operator->() const
  { return ptr; }

public:
  /*!
   * \brief Comparison function
   *
   * Only compares the lists' front() elements.
   *
   * \param a,b : list objects
   * \return : comparison integer
   *    between a.front() and b.front()
   */
  static inline int
  compare(const list &a, const list &b) {
    return algebra::compare(a->front(), b->front());
  }

  /*!
   * \brief Element equality function
   *
   * \param a : a list
   * \param x : a value
   *
   * \return : true if a.front() == x
   */
  friend inline bool
  operator==(const list &a, const O &x) {
    return a->front() == x;
  }
};

} // namespace detail

/*!
 * \class chain
 *
 * \brief chained heap class :
 *     elements comparing equal are packed in lists
 *
 * features :
 * -> constant time    find_maxs()
 * -> logarithmic time delete_maxs()
 *
 * drawbacks :
 * -> linear time      insert()
 *    (heap<O>::insert() is logarithmic time)
 *
 * memory management :
 * -> all sub-lists are new-allocated,
 *      no null pointer nor empty list
 * -> allocation in insert()
 * -> deallocation in delete_*() and dtor
 */
template<class O>
class chain
: protected heap<detail::list<O> > {

private:
  typedef detail::list<O> le;
  typedef heap<le> impl;

  // the underlying heap contains
  //  k [list<O>] objects
  // for a total of n [O] objects

public:
  /*!
   * \brief List type returned by find_maxs()
   */
  typedef typename le::impl_t list_t;

public:
  /*!
   * \brief Default constructor
   *
   * Creates an empty chained heap
   */
  inline
  chain()
  : impl() {}

public:
  /*!
   * \brief Copy constructor
   */
  inline
  chain(const chain &o)
  : impl(o) {}
  /*!
   * \brief Assignment operator
   */
  inline chain &
  operator=(const chain &o) {
    impl::operator=(o);
    return *this;
  }
  /*!
   * \brief Non-throwing swap
   */
  inline void
  swap(chain &o)
  { impl::swap(o); }

  /*!
   * \brief Sized constructor
   *
   * Construct empty heap and reserves size
   *
   * \param n : the reserved size
   */
  explicit inline
  chain(size_t n)
  : impl(n) {}

  /*!
   * \brief Destructor
   *
   * Properly deletes all the allocated lists.
   */
  inline
  ~chain() {
    boost::for_each(impl::data
    , std::mem_fun_ref(&le::destroy)
    );
    impl::data.clear();
  }

public:
  /*!
   * \brief Emptiness test
   *
   * constant time.
   */
  using impl::empty;

  /*!
   * \brief Maximum element access
   *
   * constant time.
   *
   * \return Reference to the maximum element
   */
  inline O &
  find_max()
  { return impl::find_max()->front(); }
  /*!
   * \brief Maximum element access, const version
   *
   * constant time.
   *
   * \return Reference to the maximum element
   */
  inline const O &
  find_max() const
  { return impl::find_max()->front(); }

  /*!
   * \brief Maximum elements list access
   *
   * constant time.
   *
   * \return Reference to the maximum elements list
   */
  inline list_t &
  find_maxs()
  { return *impl::find_max(); }
  /*!
   * \brief Maximum elements list access, const version
   *
   * constant time.
   *
   * \return Reference to the maximum elements list
   */
  inline const list_t &
  find_maxs() const
  { return *impl::find_max(); }

public:
  /*!
   * \brief Insertion
   *
   * \param x : the element to be inserted
   *
   * If there already exists a list
   * whose elements are equivalent to x,
   * x is added at the back of this list.
   * linear time.
   */
  inline void
  insert(const O &x) {
    // seek for existing list
    typedef typename impl::vect_t::iterator iter;
    iter it = std::find(impl::data.begin(), impl::data.end(), x);
    if(it != impl::data.end()) {
      (*it)->push_front(x);
      return;
    }

    // no existing list -> insert new le(x)
    impl::insert(le(1, x));
  }

  /*!
   * \brief Maximum deletion
   *
   * Deletes the maximum element from the heap.
   * worst case logarithmic time.
   */
  inline void
  delete_max() {
    assert(! empty());

    le lmax = impl::find_max();

    assert(!lmax->empty());

    lmax->pop_front();

    if(lmax->empty()) {
      lmax.destroy();
      impl::delete_max();
    }
  }

  /*!
   * \brief Maximum list deletion
   *
   * Deletes the maximum list from the heap.
   * logarithmic time.
   */
  inline void
  delete_maxs() {
    assert(! empty());
    impl::find_max().destroy();
    impl::delete_max();
  }
};


}} // namespace imperative::heap

#endif /* HEAP_HXX_ */
