#ifndef _GMD_GRAPH_PATH_H_
#define _GMD_GRAPH_PATH_H_

template <bool Weighted, typename Allocator> struct graph_path_base;

template <typename Allocator>
struct graph_path_base<false, Allocator>
{
	template <bool, bool, typename, typename> friend struct graph;
	template <typename, typename, typename> friend struct graph_default;
	template <bool, typename, typename, typename> friend struct graph_weighted;
	template <bool, typename, typename, typename> friend struct graph_directed;
	template <bool, typename, typename, typename, int> friend struct graph_path_traversor;

	/* === Variables === */
	protected:
	size_t _size;
	/* === Variables === */


	/* === Constructor / Destructor === */
	public:
	graph_path_base (size_t size = 0) : _size(size) {}
	/* === Constructor / Destructor === */


	/* === Path === */
	public:
	double length () const { return _size == 0 ? 0 : _size - 1; }
	/* === Path === */
};

template <typename Allocator>
struct graph_path_base<true, Allocator>
{
	template <bool, bool, typename, typename> friend struct graph;
	template <typename, typename, typename> friend struct graph_default;
	template <bool, typename, typename, typename> friend struct graph_weighted;
	template <bool, typename, typename, typename> friend struct graph_directed;
	template <bool, typename, typename, typename, int> friend struct graph_path_traversor;

	private:
	typedef typename ::std::allocator_traits<Allocator>::template rebind_alloc<double> _Allocator;
	typedef typename ::std::allocator_traits<_Allocator> _ATraits;

	/* === Variables === */
	protected:
	size_t _size;

	private:
	_Allocator _allocator;
	double *_length;
	/* === Variables === */


	/* === Constructor / Destructor === */
	public:
	graph_path_base (size_t size = 0) : _size(size), _length(_ATraits::allocate(_allocator, _size)) {}

	~graph_path_base () { _ATraits::deallocate(_allocator, _length, _size); }
	/* === Constructor / Destructor === */


	/* === Path === */
	public:
	double length () const { return _length[_size == 0 ? 0 : _size - 1]; }
	/* === Path === */
};

template <typename Node, typename Info, typename Allocator>
struct graph_path
: public graph_path_base<Node::_Weighted, Allocator>
{
	template <bool, bool, typename, typename> friend struct graph;
	template <typename, typename, typename> friend struct graph_default;
	template <bool, typename, typename, typename> friend struct graph_weighted;
	template <bool, typename, typename, typename> friend struct graph_directed;
	template <bool, typename, typename, typename, int> friend struct graph_path_traversor;

	private:
	typedef graph_path_base<Node::_Weighted, Allocator> _Base;

	private:
	typedef graph_path_traversor<false, Node, graph_path<Node, Info, Allocator>, Info,  1>   _PTraversor;
	typedef graph_path_traversor<true , Node, graph_path<Node, Info, Allocator>, Info,  1>  _CPTraversor;
	typedef graph_path_traversor<false, Node, graph_path<Node, Info, Allocator>, Info, -1>  _RPTraversor;
	typedef graph_path_traversor<true , Node, graph_path<Node, Info, Allocator>, Info, -1> _CRPTraversor;

	public:
	typedef   _PTraversor               traversor,               iterator;
	typedef  _CPTraversor         const_traversor,         const_iterator;
	typedef  _RPTraversor       reverse_traversor,       reverse_iterator;
	typedef _CRPTraversor const_reverse_traversor, const_reverse_iterator;

	private:
	typedef typename ::std::allocator_traits<Allocator>::template rebind_alloc<Node *> _Allocator;
	typedef typename ::std::allocator_traits<_Allocator> _ATraits;


	/* === Variables === */
	private:
	_Allocator _allocator;
	Node **_path;
	/* === Variables === */


	/* === Constructor / Destructor === */
	public:
	graph_path (size_t size = 0) : _Base(size), _path(_ATraits::allocate(_allocator, size+1)) {}

	~graph_path () { _ATraits::deallocate(_allocator, _path, _Base::_size+1); }
	/* === Constructor / Destructor === */


	/* #################################################################### */
	/* ###################### Traversors / Iterators ###################### */
	public:
	inline   _PTraversor   begin ()       { return   _PTraversor(this, 0); }
	inline  _CPTraversor   begin () const { return  _CPTraversor(this, 0); }
	inline  _CPTraversor  cbegin () const { return  _CPTraversor(this, 0); }
	inline  _RPTraversor  rbegin ()       { return  _RPTraversor(this, _Base::_size == 0 ? 0 : _Base::_size - 1); }
	inline _CRPTraversor  rbegin () const { return _CRPTraversor(this, _Base::_size == 0 ? 0 : _Base::_size - 1); }
	inline _CRPTraversor crbegin () const { return _CRPTraversor(this, _Base::_size == 0 ? 0 : _Base::_size - 1); }

	inline   _PTraversor   end ()       { return   _PTraversor(this, _Base::_size); }
	inline  _CPTraversor   end () const { return  _CPTraversor(this, _Base::_size); }
	inline  _CPTraversor  cend () const { return  _CPTraversor(this, _Base::_size); }
	inline  _RPTraversor  rend ()       { return  _RPTraversor(this, _Base::_size); }
	inline _CRPTraversor  rend () const { return _CRPTraversor(this, _Base::_size); }
	inline _CRPTraversor crend () const { return _CRPTraversor(this, _Base::_size); }
	/* ###################### Traversors / Iterators ###################### */
	/* #################################################################### */


	/* #################################################################### */
	/* ############################# Capacity ############################# */
	public:
	bool  empty () const { return _Base::_size == 0; }
	size_t size () const { return _Base::_size;      }
	/* ############################# Capacity ############################# */
	/* #################################################################### */
};

#endif
