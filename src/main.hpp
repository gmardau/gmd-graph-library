#ifndef _GMD_GRAPH_MAIN_H_
#define _GMD_GRAPH_MAIN_H_

template <bool Weighted, bool Directed, typename Info, typename Allocator = ::std::allocator<Info>>
struct graph
{
	template <bool, bool, typename, typename> friend struct graph;

	private:
	using _Node = graph_node<Weighted, Directed, Info, Allocator>;

	private:
	using   _Traversor = graph_traversor<false, _Node, Info, 1>;
	using  _CTraversor = graph_traversor<true , _Node, Info, 1>;
	using  _RTraversor = graph_traversor<false, _Node, Info, 0>;
	using _CRTraversor = graph_traversor<true , _Node, Info, 0>;
	using _Path = graph_path<_Node, Info, Allocator>;

	public:
	using               traversor =   _Traversor; using               iterator =   _Traversor;
	using         const_traversor =  _CTraversor; using         const_iterator =  _CTraversor;
	using       reverse_traversor =  _RTraversor; using       reverse_iterator =  _RTraversor;
	using const_reverse_traversor = _CRTraversor; using const_reverse_iterator = _CRTraversor;
	using path = _Path;

	private:
	using _Allocator = typename ::std::allocator_traits<Allocator>::template rebind_alloc<_Node>;
	using _ATraits   = typename ::std::allocator_traits<_Allocator>;


	/* === Variables === */
	protected:
	_Allocator _allocator;
	size_t _size = 0;
	_Node *_head;
	/* === Variables === */


	/* #################################################################### */
	/* ##################### Constructor / Destructor ##################### */
	/* === Standard === */
	public:
	graph (const Allocator &a = Allocator()) : _allocator(_Allocator(a)), _head(_new_head()) {}
	/* === Standard === */


	/* === Range === */
	public:
	template <typename T1, typename T2>
	graph (const T1 &first, const T2 &last, const Allocator &a = Allocator())
		: _allocator(_Allocator(a)), _head(_new_head()) { insert(first, last); }
	/* === Range === */


	/* === Copy === */
	public:
	graph (const graph &other) : _allocator(_ATraits::select_on_container_copy_construction(other._allocator)),
		_head(_new_head()) { _copy(other); }

	graph (const graph &other, const Allocator &a) : _allocator(_Allocator(a)),
		_head(_new_head()) { _copy(other); }
	/* === Copy === */


	/* === Move === */
	public:
	graph (graph &&other)
		: _allocator(other._allocator), _size(other._size), _head(other._head) { other._init(); }

	graph (graph &&other, const Allocator &a)
		: _allocator(_Allocator(a)),    _size(other._size), _head(other._head)
	{ if(_ATraits::is_always_equal::value || _allocator == other.allocator) other._init();
	  else { _init(); _copy(); other._renew(); } }
	/* === Move === */


	/* === Initializer List === */
	public:
	graph (const ::std::initializer_list<Info> &il, const Allocator &a = Allocator())
		: _allocator(_Allocator(a)), _head(_new_head()) { insert(il); }
	/* === Initializer List === */


	/* === Destructor === */
	public:
	~graph ()
	{
		_Node *node1 = _head->_thread[1], *node2;
		for( ; node1 != _head; node1 = node2) { node2 = node1->_thread[1]; _del_node(node1); }
		_del_head();
	}
	/* === Destructor === */
	/* ##################### Constructor / Destructor ##################### */
	/* #################################################################### */


	/* ##################################################################### */
	/* ########################## Assign operator ########################## */
	/* === Copy === */
	public:
	graph &
	operator= (const graph &other)
	{ if(this != &other) { _prepare_copy(other); _copy(other); } return *this; }

	public:
	template <bool Weighted_Other, bool Directed_Other, typename Allocator_Other>
	graph &
	operator= (const graph<Weighted_Other, Directed_Other, Info, Allocator_Other> &other)
	{ _prepare_copy(other); _copy(other); return *this; }
	/* === Copy === */

	/* === Move === */
	public:
	graph &
	operator= (graph &&other)
	{
		if(this != &other) {
			if(_prepare_move(::std::move(other))) { _move(::std::move(other)); other._init();  }
			else                                  { _copy(other);              other._renew(); } }
		return *this;
	}

	public:
	template <bool Weighted_Other, bool Directed_Other, typename Allocator_Other>
	graph &
	operator= (graph<Weighted_Other, Directed_Other, Info, Allocator_Other> &&other)
	{ _prepare_move(::std::move(other)); _copy(other); other._renew(); return *this; }
	/* === Move === */

	/* === Initializer List === */
	public:
	graph &
	operator= (const ::std::initializer_list<Info> &il)
	{ clear(); insert(il); return *this; }
	/* === Initializer List === */
	/* ########################## Assign operator ########################## */
	/* ##################################################################### */


	/* ##################################################################### */
	/* ############################# Observers ############################# */
	/* === Allocator === */
	public:
	inline Allocator get_allocator () const { return Allocator(_allocator); }
	/* === Allocator === */
	/* ############################# Observers ############################# */
	/* ##################################################################### */


	/* #################################################################### */
	/* ###################### Traversors / Iterators ###################### */
	public:
	inline   _Traversor   begin ()       { return   _Traversor(_head->_thread[1]); }
	inline  _CTraversor   begin () const { return  _CTraversor(_head->_thread[1]); }
	inline  _CTraversor  cbegin () const { return  _CTraversor(_head->_thread[1]); }
	inline  _RTraversor  rbegin ()       { return  _RTraversor(_head->_thread[0]); }
	inline _CRTraversor  rbegin () const { return _CRTraversor(_head->_thread[0]); }
	inline _CRTraversor crbegin () const { return _CRTraversor(_head->_thread[0]); }

	inline   _Traversor   end ()       { return   _Traversor(_head); }
	inline  _CTraversor   end () const { return  _CTraversor(_head); }
	inline  _CTraversor  cend () const { return  _CTraversor(_head); }
	inline  _RTraversor  rend ()       { return  _RTraversor(_head); }
	inline _CRTraversor  rend () const { return _CRTraversor(_head); }
	inline _CRTraversor crend () const { return _CRTraversor(_head); }
	/* ###################### Traversors / Iterators ###################### */
	/* #################################################################### */


	/* #################################################################### */
	/* ############################# Capacity ############################# */
	public:
	inline bool      empty () const { return _size == 0; }
	// inline size_t     size () const { return _size;      }
	inline size_t max_size () const { return _ATraits::max_size(_allocator); }
	/* ############################# Capacity ############################# */
	/* #################################################################### */


	/* ##################################################################### */
	/* ############################# Modifiers ############################# */
	/* === Clear === */
	public:
	void
	clear ()
	{
		_Node *node1 = _head->_thread[1], *node2;
		for( ; node1 != _head; node1 = node2) { node2 = node1->_thread[1]; _del_node(node1); }
		_head->_thread[0] = _head; _head->_thread[1] = _head;
	}
	/* === Clear === */

	/* === Insert === */
	public:
	template <typename T = _Traversor>
	inline T
	insert (const Info &info)
	{
		_Node *node = _new_node(_head->_thread[0], _head, info);
		_head->_thread[0]->_thread[1] = node; _head->_thread[0] = node;
		return T(node);
	}

	public:
	template <typename TOut = _Traversor, typename TIn>
	inline TOut
	insert (const TIn &tr)
	{ return insert<TOut>(*tr); }

	public:
	template <typename T1, typename T2>
	void
	insert (const T1 &first, const T2 &last)
	{
		_Node *node = _head->_thread[0];
		for(T1 tr = first; tr != last; ++tr)
			node = node->_thread[1] = _new_node(node, _head, *tr);
		_head->_thread[0] = node;
	}

	public:
	inline void
	insert (const ::std::initializer_list<Info> &il)
	{ insert(il.begin(), il.end()); }
	/* === Insert === */


	/* === Emplace === */
	public:
	template <typename T = _Traversor, typename... Args>
	T
	emplace (Args&&... info_args)
	{
		_Node *node = _new_node(_head->_thread[0], _head, ::std::forward<Args>(info_args)...);
		_head->_thread[0]->_thread[1] = node; _head->_thread[0] = node;
		return T(node);
	}
	/* === Emplace === */


	/* === Erase === */
	public:
	template <typename T = _Traversor>
	inline void
	erase (const T &tr)
	{
		for(typename _Node::_Adjacency::iterator it = tr.node()->in().begin(); it != tr.node()->in().end(); ++it)
			_Node::node(it)->out().erase(tr.node());
		tr.node()->_thread[0]->_thread[1] = tr.node()->_thread[1];
		tr.node()->_thread[1]->_thread[0] = tr.node()->_thread[0];
		_del_node(tr.node());
	}

	public:
	template <typename T1, typename T2>
	void
	erase (const T1 &first, const T2 &last)
	{
		if(first.node() == _head->_thread[1] && last.node() == _head && _size > 0) clear();
		else for(T1 tr = first; tr != last; ) erase(tr++);
	}
	/* === Erase === */


	/* === Swap === */
	public:
	void
	swap (graph &other)
	{ if(this != &other) { if(_prepare_swap(other)) _swap_move(other); else _swap_copy(other); } }

	public:
	template <bool Weighted_Other, bool Directed_Other, typename Allocator_Other>
	void
	swap (graph<Weighted_Other, Directed_Other, Info, Allocator_Other> &other)
	{ _prepare_swap(other); _swap_copy(other); }
	/* === Swap === */


	/* === Transfer === */
	public:
	template <typename TOut = _Traversor, typename TIn>
	TOut
	transfer (graph &other, const TIn &tr)
	{
		if(_ATraits::is_always_equal::value || _allocator == other._allocator)
			 return TOut(_transfer_move(other, tr.node()));
		else return TOut(_transfer_copy(other, tr.node()));
	}

	public:
	template <typename TOut = _Traversor, bool Weighted_Other, bool Directed_Other, typename Allocator_Other, typename TIn>
	inline TOut
	transfer (graph<Weighted_Other, Directed_Other, Info, Allocator_Other> &other, const TIn &tr)
	{ return TOut(_transfer_copy(other, tr.node())); }
	/* === Transfer === */


	/* === Merge === */
	public:
	void
	merge (graph &other)
	{
		if(this == &other || other._size == 0) return;
		if(_ATraits::is_always_equal::value || _allocator == other._allocator)
			 _merge_move(other);
		else _merge_copy(other);
	}

	public:
	template <bool Weighted_Other, bool Directed_Other, typename Allocator_Other>
	inline void
	merge (graph<Weighted_Other, Directed_Other, Info, Allocator_Other> &other)
	{ if(other._size != 0) _merge_copy(other); }
	/* === Merge === */
	/* ############################# Modifiers ############################# */
	/* ##################################################################### */


	/* ##################################################################### */
	/* ############################### Graph ############################### */
	public:
	size_t
	size ()
	const
	{
		return 0;
	}

	public:
	size_t
	order ()
	const
	{ return _size; }
	/* ############################### Graph ############################### */
	/* ##################################################################### */


	/* ##################################################################### */
	/* ############################# Adjacency ############################# */
	/* === Connect === */
	public:
	template <typename T1, typename T2>
	::std::enable_if_t<!Weighted && ::std::is_same_v<T1, T1>, void>
	connect (const T1 &from, const T2 &to)
	{
		from.node()->out().insert(  to.node());
		  to.node()->in ().insert(from.node());
	}

	public:
	template <typename T1, typename T2>
	::std::enable_if_t<Weighted && ::std::is_same_v<T1, T1>, bool>
	connect (const T1 &from, const T2 &to, double weight)
	{
		::std::pair<typename _Node::_Adjacency::const_iterator, bool>
		       result = from.node()->out().insert({  to.node(), weight});
		if(result.second) to.node()->in ().insert({from.node(), weight});
		return result.second;
	}

	public:
	template <typename T1, typename T2>
	::std::enable_if_t<Weighted && ::std::is_same_v<T1, T1>, void>
	connect_replace (const T1 &from, const T2 &to, double weight)
	{
		from.node()->out().insert_or_assign({  to.node(), weight});
		  to.node()->in ().insert_or_assign({from.node(), weight});
	}
	/* === Connect === */


	/* === Disconnect === */
	public:
	template <typename T1, typename T2>
	void
	disconnect (const T1 &from, const T2 &to)
	{
		from.node()->out().erase(  to.node());
		  to.node()->in ().erase(from.node());
	}
	/* === Disconnect === */


	/* === Clean === */
	public:
	void
	clean ()
	{
		for(_Node *node = _head->_thread[1]; node != _head; node = node->_thread[1]) {
			node->out().clear(); if(Directed) node->in().clear(); }
	}

	public:
	template <typename T>
	void
	clean (const T &tr)
	{
		for(typename _Node::_Adjacency::iterator it = tr.node()->in().begin(); it != tr.node()->in().end(); ++it)
			_Node::node(it)->out().erase(tr.node());
		tr.node()->in().clear();
		if(Directed) {
			for(typename _Node::_Adjacency::iterator it = tr.node()->out().begin(); it != tr.node()->out().end(); ++it)
				_Node::node(it)->in().erase(tr.node());
			tr.node()->out().clear(); }
	}
	/* === Clean === */
	/* ############################# Adjacency ############################# */
	/* ##################################################################### */


	/* #################################################################### */
	/* ############################## Lookup ############################## */


	/* ############################## Lookup ############################## */
	/* #################################################################### */


	/* ##################################################################### */
	/* ############################### Print ############################### */
	public:
	template <typename Printer>
	void
	print (Printer &printer)
	const
	{ for(_Node *node = _head->_thread[1]; node != _head; node = node->_thread[1]) _print(printer, node); }

	public:
	template <typename Printer>
	void
	print (const Printer &printer = Printer())
	const
	{ for(_Node *node = _head->_thread[1]; node != _head; node = node->_thread[1]) _print(printer, node); }

	public:
	template <typename Printer, typename T>
	inline void
	print (const T &tr, Printer &printer)
	const
	{ _print(printer, tr.node()); }

	public:
	template <typename Printer, typename T>
	inline void
	print (const T &tr, const Printer &printer = Printer())
	const
	{ _print(printer, tr.node()); }
	/* ############################### Print ############################### */
	/* ##################################################################### */


	#include "path_finding.hpp"


	/* #################################################################### */
	/* ###################### Public functions (API) ###################### */
	/* #################################################################### */
	                            /* ########### */
	/* #################################################################### */
	                            /* ########### */
	/* #################################################################### */
	/* ######################## Internal functions ######################## */
	/* #################################################################### */


	/* ##################################################################### */
	/* ########################## List Management ########################## */
	private:
	inline void  _init () { _size = 0; _head = _new_head(); }
	inline void _reset () { _size = 0;           _reset_head(); }
	inline void _renew () { clear();             _renew_head(); }
	/* ########################## List Management ########################## */
	/* ##################################################################### */


	/* ##################################################################### */
	/* ########################## _Node Management ########################## */
	/* === Sentinel === */
	private:
	_Node *
	_new_head ()
	{
		_Node *head = _ATraits::allocate(_allocator, 1);
		:: new(head) _Node(head, head);
		return head;
	}

	private:
	void
	_del_head ()
	{ _head->~_Node(); _ATraits::deallocate(_allocator, _head, 1); }

	private:
	void
	_renew_head ()
	{ _del_head(); _head = _new_head(); }

	private:
	void
	_reset_head ()
	{ _head->_thread[0] = _head; _head->_thread[1] = _head; }
	/* === Sentinel === */


	/* === _Node === */
	private:
	template <typename... Args>
	_Node *
	_new_node (_Node *pred, _Node *succ, Args&&... info_args)
	{
		++_size;
		_Node *node = _ATraits::allocate(_allocator, 1);
		:: new(node) _Node(pred, succ);
		_new_info(node, ::std::forward<Args>(info_args)...);
		return node;
	}

	private:
	void
	_del_node (_Node *node)
	{
		--_size;
		_del_info(node);
		node->~_Node();
		_ATraits::deallocate(_allocator, node, 1);
	}
	/* === _Node === */


	/* === Info === */
	private:
	template <typename... Args>
	inline void
	_new_info (_Node *node, Args&&... info_args)
	{ ::new(node->info()) Info(::std::forward<Args>(info_args)...); }

	private:
	inline void
	_del_info (_Node *node)
	{ node->info()->~Info(); }
	/* === Info === */
	/* ########################## _Node Management ########################## */
	/* ##################################################################### */


	/* #################################################################### */
	/* ##################### External List Management ##################### */
	/* === Preprocessing === */
	private:
	template <bool Weighted_Other, bool Directed_Other, typename Allocator_Other>
	void
	_prepare_copy (const graph<Weighted_Other, Directed_Other, Info, Allocator_Other> &other)
	{
		clear();
		_del_head();
		_copy_allocator(other);
		_head = _new_head();
	}

	private:
	template <bool Weighted_Other, bool Directed_Other, typename Allocator_Other>
	bool
	_prepare_move (graph<Weighted_Other, Directed_Other, Info, Allocator_Other> &&other)
	{
		clear();
		_del_head();
		if(_move_allocator(::std::move(other))) return true;
		_head = _new_head();
		return false;
	}

	private:
	template <bool Weighted_Other, bool Directed_Other, typename Allocator_Other>
	inline bool
	_prepare_swap (graph<Weighted_Other, Directed_Other, Info, Allocator_Other> &other)
	{ return _swap_allocator(other); }
	/* === Preprocessing === */


	/* === Allocator === */
	private:
	inline void
	_copy_allocator (const graph &other)
	{ if(_ATraits::propagate_on_container_copy_assignment) _allocator = other._allocator; }

	private:
	template <bool Weighted_Other, bool Directed_Other, typename Allocator_Other>
	constexpr void
	_copy_allocator (const graph<Weighted_Other, Directed_Other, Info, Allocator_Other> &other) {}

	private:
	inline bool
	_move_allocator (graph &&other)
	{
		if(_ATraits::propagate_on_container_move_assignment::value) {
			_allocator = ::std::move(other._allocator); return true; }
		else return _ATraits::is_always_equal::value || _allocator == other._allocator;
	}

	private:
	template <bool Weighted_Other, bool Directed_Other, typename Allocator_Other>
	constexpr bool
	_move_allocator (const graph<Weighted_Other, Directed_Other, Info, Allocator_Other> &other)
	{ return false; }

	private:
	inline bool
	_swap_allocator (graph &other)
	{
		if(_ATraits::propagate_on_container_swap) { ::std::swap(_allocator, other._allocator); return true; }
		else return _ATraits::is_always_equal::value || _allocator == other._allocator;
	}

	private:
	template <bool Weighted_Other, bool Directed_Other, typename Allocator_Other>
	constexpr bool
	_swap_allocator (graph<Weighted_Other, Directed_Other, Info, Allocator_Other> &other)
	{ return false; }
	/* === Allocator === */


	/* === Copy === */
	private:
	template <bool Weighted_Other, bool Directed_Other, typename Allocator_Other>
	void
	_copy (const graph<Weighted_Other, Directed_Other, Info, Allocator_Other> &other)
	{
		if(other._size == 0) return;
		_copy_list(_head, other._head->_thread[1], other._head);
		_copy_adjacencies(_head->_thread[1], _head, other._head->_thread[1]);
	}

	private:
	template <typename Node_Other>
	void
	_copy_list (_Node *end, Node_Other *begin_other, Node_Other *end_other)
	{
		_Node *node = end->_thread[0];
		for( ; begin_other != end_other; begin_other = begin_other->_thread[1])
			node = node->_thread[1] = _new_node(node, end, **begin_other);
		end->_thread[0] = node;
	}

	private:
	template <typename Node_Other>
	void
	_copy_adjacencies (_Node *begin, _Node *end, Node_Other *begin_other)
	{
		_Node *node1 = begin, *node2;
		Node_Other *node_other1 = begin_other, *node_other2;
		for( ; node1 != end; node1 = node1->_thread[1], node_other1 = node_other1->_thread[1]) {
			_copy_self_adjacency(node1, node_other1);
			node2 = begin->_thread[1]; node_other2 = begin_other->_thread[1];
			for( ; node2 != end; node2 = node2->_thread[1], node_other2 = node_other2->_thread[1])
				_copy_adjacency(node1, node2, node_other1, node_other2); }
	}

	private:
	template <typename Node_Other, typename = ::std::enable_if_t<!Weighted || !Node_Other::_Weighted>>
	void
	_copy_self_adjacency (_Node *node, Node_Other *node_other)
	{ if(node_other->out().count(node_other)) node->out().insert(node); if(Directed) node->in().insert(node); }

	private:
	template <typename Node_Other, typename = ::std::enable_if_t<Weighted && Node_Other::_Weighted>, typename = void>
	void
	_copy_self_adjacency (_Node *node, Node_Other *node_other)
	{
		typename Node_Other::_Adjacency::const_iterator it = node_other->out().find(node_other);
		if(it != node_other->out().cend()) {
			node->out().insert({node, it->second}); if(Directed) node->in().insert({node, it->second}); }
	}

	private:
	template <typename Node_Other, typename = ::std::enable_if_t<!Weighted || !Node_Other::_Weighted>>
	void
	_copy_adjacency (_Node *node1, _Node *node2, Node_Other *node_other1, Node_Other *node_other2)
	{
		if(!Node_Other::_Directed) {
			if(node_other1->out().count(node_other2)) { _connect(node1, node2);
				if(Directed) _connect(node2, node1); } }
		else if(Directed) { if(node_other1->out().count(node_other2)) _connect(node1, node2);
		                    if(node_other2->out().count(node_other1)) _connect(node2, node1); }
		else if(node_other1->out().count(node_other2) || node_other2->out().count(node_other1)) _connect(node1, node2);
	}

	private:
	template <typename Node_Other, typename = ::std::enable_if_t<Weighted && Node_Other::_Weighted>, typename = void>
	void
	_copy_adjacency (_Node *node1, _Node *node2, Node_Other *node_other1, Node_Other *node_other2)
	{
		typename Node_Other::_Adjacency::const_iterator it = node_other1->out().find(node_other2);
		if(!Node_Other::_Directed) {
			if(it != node_other1->out().cend()) { _connect(node1, node2, it->second);
				if(Directed) _connect(node2, node1, it->second); } }
		else if(Directed) {
			if(it != node_other1->out().cend()) _connect(node1, node2, it->second);
			it = node_other2->out().find(node_other1);
			if(it != node_other2->out().cend()) _connect(node2, node1, it->second); }
		else {
			typename Node_Other::_Adjacency::const_iterator it2 = node_other2->out().find(node_other1);
			if(it != node_other1->out().cend()) {
				if(it2 != node_other2->out().cend())  _connect(node1, node2, (it->second + it2->second) / 2);
				else                                  _connect(node1, node2, it ->second); }
			else if(it2 != node_other2->out().cend()) _connect(node1, node2, it2->second); }
	}
	/* === Copy === */


	/* === Move === */
	private:
	void
	_move (graph &&other)
	{ _size = other._size; _head = other._head; }
	/* === Move === */


	/* === Swap === */
	private:
	void
	_swap_move (graph &other)
	{ ::std::swap(_size, other._size); ::std::swap(_head, other._head); }

	private:
	template <bool Weighted_Other, bool Directed_Other, typename Allocator_Other>
	void
	_swap_copy (graph<Weighted_Other, Directed_Other, Info, Allocator_Other> &other)
	{
		_Node *new_head = _new_head();
		typename graph<Weighted_Other, Directed_Other, Info, Allocator_Other>
		::_Node *new_head_other = other._new_head();
		if(other._size != 0) {
			_copy_list(new_head, other._head->_thread[1], other._head);
			_copy_adjacencies(new_head->_thread[1], new_head, other._head->_thread[1]); }
		if(_size != 0) {
			other._copy_list(new_head_other, _head->_thread[1], _head);
			other._copy_adjacencies(new_head_other->_thread[1], new_head_other, _head->_thread[1]); }
		      clear();       _head = new_head;
		other.clear(); other._head = new_head_other;
	}
	/* === Swap === */


	/* === Transfer === */
	private:
	_Node *
	_transfer_move (graph &other, _Node *node_other)
	{ other._erase<false>(node_other); node_other.reset(); _emplace(node_other); return node_other; }

	private:
	template <bool Weighted_Other, bool Directed_Other, typename Allocator_Other, typename Node_Other>
	_Node *
	_transfer_copy (graph<Weighted_Other, Directed_Other, Info, Allocator_Other> &other, Node_Other *node_other)
	{ _Node *node = _insert(**node_other); other._erase(node_other); return node; }
	/* === Transfer === */


	/* === Merge === */
	private:
	void
	_merge_move (graph &other)
	{
		_head->_thread[0]->_thread[1] = other._head->_thread[1];
		other._head->_thread[1]->_thread[0] = _head->_thread[0];
		other._head->_thread[0]->_thread[1] = _head;
		_head->_thread[0] = other._head->_thread[0];
		_size += other._size; other._reset();
	}

	private:
	template <bool Weighted_Other, bool Directed_Other, typename Allocator_Other>
	void
	_merge_copy (graph<Weighted_Other, Directed_Other, Info, Allocator_Other> &other)
	{
		_Node *last = _head->_thread[0];
		_copy_list(_head, other._head->_thread[1], other._head);
		_copy_adjacencies(last->_thread[1], _head, other._head->_thread[1]);
		other.clear();
	}
	/* === Merge === */
	/* ##################### External List Management ##################### */
	/* #################################################################### */


	/* ##################################################################### */
	/* ############################# Modifiers ############################# */
	/* === Insert === */
	private:
	_Node *
	_insert (const Info &info)
	{
		_Node *node = _new_node(_head->_thread[0], _head, info);
		_head->_thread[0]->_thread[1] = node; _head->_thread[0] = node;
		return node;
	}
	/* === Insert === */


	/* === Emplace === */
	private:
	void
	_emplace (_Node *node)
	{
		node->_thread[0] = _head->_thread[0]; node->_thread[1] = _head;
		_head->_thread[0]->_thread[1] = node; _head->_thread[0] = node;
	}
	/* === Emplace === */


	/* === Erase === */
	private:
	template <bool Delete = true>
	void
	_erase (_Node *node)
	{
		for(typename _Node::_Adjacency::iterator it = node->in().begin(); it != node->in().end(); ++it)
			_Node::node(it)->out().erase(node);
		node->_thread[0]->_thread[1] = node->_thread[1];
		node->_thread[1]->_thread[0] = node->_thread[0];
		if(Delete) _del_node(node);
	}
	/* === Erase === */
	/* ############################# Modifiers ############################# */
	/* ##################################################################### */


	/* ##################################################################### */
	/* ############################# Adjacency ############################# */
	/* === Connect === */
	private:
	template <typename Dummy = void>
	::std::enable_if_t<!Weighted, Dummy>
	_connect (_Node *from, _Node *to)
	{ from->out().insert(to); to->in().insert(from); }

	private:
	template <typename Dummy = void>
	::std::enable_if_t<Weighted, Dummy>
	_connect (_Node *from, _Node *to, double weight = 1)
	{ from->out().insert({to, weight}); to->in().insert({from, weight}); }
	/* === Connect === */
	/* ############################# Adjacency ############################# */
	/* ##################################################################### */


	/* ##################################################################### */
	/* ############################### Print ############################### */
	private:
	template <typename Printer>
	void
	_print (Printer &printer, const _Node *node)
	const
	{
		printf("\u2022 "); printer(**node); if(!Directed) printf(" \u2194"); else printf(" \u2192");
		for(typename _Node::_Adjacency::const_iterator it = node->out().cbegin(); it != node->out().cend(); ++it) {
			printf(" %c", _Node::node(it) == node ? '[' : '(');
			// if(_Node::node(it) == node) printf("\u21ba ");
			printer(**_Node::node(it)); if(Weighted) printf(": %g", _Node::weight(it));
			printf("%c", _Node::node(it) == node ? ']' : ')'); }
		printf("\n");
	}

	private:
	template <typename Printer>
	void
	_print (const Printer &printer, const _Node *node)
	const
	{
		printf("\u2022 "); printer(**node); if(!Directed) printf(" \u2194"); else printf(" \u2192");
		for(typename _Node::_Adjacency::const_iterator it = node->out().cbegin(); it != node->out().cend(); ++it) {
			printf(" %c", _Node::node(it) == node ? '[' : '(');
			// if(_Node::node(it) == node) printf("\u21ba ");
			printer(**_Node::node(it)); if(Weighted) printf(": %g", _Node::weight(it));
			printf("%c", _Node::node(it) == node ? ']' : ')'); }
		printf("\n");
	}
	/* ############################### Print ############################### */
	/* ##################################################################### */
};

#endif
