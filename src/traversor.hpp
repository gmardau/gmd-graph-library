#ifndef _GMD_GRAPH_TRAVERSOR_H_
#define _GMD_GRAPH_TRAVERSOR_H_

template <bool Const, typename Node, typename Info> struct graph_adjacency_traversor;
template <bool Const, typename Node, typename Path, typename Info, int Direction> struct graph_path_traversor;

template <bool Const, typename Node, typename Info, bool Direction>
struct graph_traversor
{
	template <bool, typename, typename, bool> friend struct graph_traversor;
	template <bool, bool, typename, typename> friend struct graph;
	template <typename, typename, typename> friend struct graph_default;
	template <bool, typename, typename, typename> friend struct graph_weighted;
	template <bool, typename, typename, typename> friend struct graph_directed;
	template <bool, typename, typename> friend struct graph_adjacency_traversor;
	template <bool, typename, typename, typename, int> friend struct graph_path_traversor;

	private:
	typedef _select_t<Const, const Node, Node> _Node;
	typedef _select_t<Const, const Info, Info> _Info;
	typedef _select_t<Const, typename Node::_Adjacency::const_iterator, typename Node::_Adjacency::iterator> _Handle;

	private:
	typedef graph_traversor<Const, Node, Info, Direction> _Traversor;
	typedef graph_traversor<Const, Node, Info, Direction^1> _RTraversor;
	typedef graph_adjacency_traversor<false, Node, Info>  _ATraversor;
	typedef graph_adjacency_traversor<true , Node, Info> _ACTraversor;

	public:
	typedef  _ATraversor       traversor,       iterator;
	typedef _ACTraversor const_traversor, const_iterator;


	/* === Variables === */
	private:
	_Node *_node;
	inline _Node *node () const { return _node; }
	/* === Variables === */


	/* === Constructor / Destructor === */
	public:
	graph_traversor (_Node *node = nullptr) : _node(node) {}

	template <bool Direction_Other>
	graph_traversor (const graph_traversor<false, Node, Info, Direction_Other> &other) : _node(other._node) {}

	graph_traversor (const graph_adjacency_traversor<Const, Node, Info> &other) : _node(other.node()) {}

	template <typename Path_Other, int Direction_Other>
	graph_traversor (const graph_path_traversor<false, Node, Path_Other, Info, Direction_Other> &other)
		: _node(other.node()) {}

	template <typename Path_Other, int Direction_Other, typename = std::enable_if_t<Const, Path_Other>>
	graph_traversor (const graph_path_traversor<true, Node, Path_Other, Info, Direction_Other> &other)
		: _node(other.node()) {}
	/* === Constructor / Destructor === */


	/* === Reverse === */
	public:
	inline _RTraversor reverse () { return _RTraversor(_node); }
	/* === Reverse === */


	/* === Info === */
	public:
	inline _Info info        () const { return **_node; }
	inline _Info &operator*  () const { return **_node; }
	inline _Info *operator-> () const { return _node->info(); }

	inline bool operator() () const { return _node != nullptr; }
	template <typename T> inline bool same(const T &tr) const { return (*this)() && tr() && node() == tr.node(); }

	template <typename T> inline bool operator== (const T &tr) const { return _node == tr._node; }
	template <typename T> inline bool operator!= (const T &tr) const { return _node != tr._node; }
	/* === Info === */


	/* === Traversal === */
	public:
	inline _Traversor &operator-- ()    {                         _node = _node->_thread[Direction^1]; return *this; }
	inline _Traversor  operator-- (int) { _Traversor tmp = *this; _node = _node->_thread[Direction^1]; return tmp;   }
	inline _Traversor &operator++ ()    {                         _node = _node->_thread[Direction];   return *this; }
	inline _Traversor  operator++ (int) { _Traversor tmp = *this; _node = _node->_thread[Direction];   return tmp;   }

	inline _Traversor &go_prev () { _node = _node->_thread[Direction^1]; return *this; }
	inline _Traversor &go_next () { _node = _node->_thread[Direction];   return *this; }

	inline _Traversor prev () const { return _Traversor(_node->_thread[Direction^1]); }
	inline _Traversor next () const { return _Traversor(_node->_thread[Direction]);   }

	public:
	template <typename Dummy = void>
	inline ::std::enable_if_t<Node::_Weighted && ::std::is_same_v<Dummy, Dummy>, _Traversor &>
	go_shortest ()
	{
		::std::pair<_Node *, double> in = _shortest_pair<0>(), out = _shortest_pair<1>();
		_node = in.second < out.second ? in.first : out.first; return *this;
	}

	template <typename Dummy = void>
	inline ::std::enable_if_t<Node::_Weighted && ::std::is_same_v<Dummy, Dummy>, _Traversor &>
	go_shortest_in ()
	{ _node = _shortest<0>(); return *this; }

	template <typename Dummy = void>
	inline ::std::enable_if_t<Node::_Weighted && ::std::is_same_v<Dummy, Dummy>, _Traversor &>
	go_shortest_out ()
	{ _node = _shortest<1>(); return *this; }

	template <typename Dummy = void>
	inline ::std::enable_if_t<Node::_Weighted && ::std::is_same_v<Dummy, Dummy>, _Traversor &>
	go_longest ()
	{
		::std::pair<_Node *, double> in = _longest_pair<0>(), out = _longest_pair<1>();
		_node = in.second > out.second ? in.first : out.first; return *this;
	}

	template <typename Dummy = void>
	inline ::std::enable_if_t<Node::_Weighted && ::std::is_same_v<Dummy, Dummy>, _Traversor &>
	go_longest_in ()
	{ _node = _longest<0>(); return *this; }

	template <typename Dummy = void>
	inline ::std::enable_if_t<Node::_Weighted && ::std::is_same_v<Dummy, Dummy>, _Traversor &>
	go_longest_out ()
	{ _node = _longest<1>(); return *this; }

	template <typename Dummy = void>
	inline ::std::enable_if_t<Node::_Weighted && ::std::is_same_v<Dummy, Dummy>, _Traversor &>
	go_closest ()
	{
		::std::pair<_Node *, double> in = _closest_pair<0>(), out = _closest_pair<1>();
		_node = ::std::abs(in.second) < ::std::abs(out.second) ? in.first : out.first; return *this;
	}

	template <typename Dummy = void>
	inline ::std::enable_if_t<Node::_Weighted && ::std::is_same_v<Dummy, Dummy>, _Traversor &>
	go_closest_in ()
	{ _node = _closest<0>(); return *this; }

	template <typename Dummy = void>
	inline ::std::enable_if_t<Node::_Weighted && ::std::is_same_v<Dummy, Dummy>, _Traversor &>
	go_closest_out ()
	{ _node = _closest<1>(); return *this; }


	template <typename Dummy = void>
	inline ::std::enable_if_t<Node::_Weighted && ::std::is_same_v<Dummy, Dummy>, _Traversor>
	shortest () const
	{
		::std::pair<_Node *, double> in = _shortest_pair<0>(), out = _shortest_pair<1>();
		return _Traversor(in.second < out.second ? in.first : out.first);
	}

	template <typename Dummy = void>
	inline ::std::enable_if_t<Node::_Weighted && ::std::is_same_v<Dummy, Dummy>, _Traversor>
	shortest_in () const
	{ return _Traversor(_shortest<0>()); }

	template <typename Dummy = void>
	inline ::std::enable_if_t<Node::_Weighted && ::std::is_same_v<Dummy, Dummy>, _Traversor>
	shortest_out () const
	{ return _Traversor(_shortest<1>()); }

	template <typename Dummy = void>
	inline ::std::enable_if_t<Node::_Weighted && ::std::is_same_v<Dummy, Dummy>, _Traversor>
	longest () const
	{
		::std::pair<_Node *, double> in = _longest_pair<0>(), out = _longest_pair<1>();
		return _Traversor(in.second > out.second ? in.first : out.first);
	}

	template <typename Dummy = void>
	inline ::std::enable_if_t<Node::_Weighted && ::std::is_same_v<Dummy, Dummy>, _Traversor>
	longest_in () const
	{ return _Traversor(_longest<0>()); }

	template <typename Dummy = void>
	inline ::std::enable_if_t<Node::_Weighted && ::std::is_same_v<Dummy, Dummy>, _Traversor>
	longest_out () const
	{ return _Traversor(_longest<1>()); }

	template <typename Dummy = void>
	inline ::std::enable_if_t<Node::_Weighted && ::std::is_same_v<Dummy, Dummy>, _Traversor>
	closest () const
	{
		::std::pair<_Node *, double> in = _closest_pair<0>(), out = _closest_pair<1>();
		return _Traversor(::std::abs(in.second) < ::std::abs(out.second) ? in.first : out.first);
	}

	template <typename Dummy = void>
	inline ::std::enable_if_t<Node::_Weighted && ::std::is_same_v<Dummy, Dummy>, _Traversor>
	closest_in () const
	{ return _Traversor(_closest<0>()); }

	template <typename Dummy = void>
	inline ::std::enable_if_t<Node::_Weighted && ::std::is_same_v<Dummy, Dummy>, _Traversor>
	closest_out () const
	{ return _Traversor(_closest<1>()); }


	template <typename Dummy = void>
	inline ::std::enable_if_t<Node::_Weighted && ::std::is_same_v<Dummy, Dummy>, ::std::pair<_Traversor, double>>
	shortest_pair () const
	{
		::std::pair<_Node *, double> in = _shortest_pair<0>(), out = _shortest_pair<1>();
		return in.second < out.second ? in : out;
	}

	template <typename Dummy = void>
	inline ::std::enable_if_t<Node::_Weighted && ::std::is_same_v<Dummy, Dummy>, ::std::pair<_Traversor, double>>
	shortest_in_pair () const
	{ return _shortest_pair<0>(); }

	template <typename Dummy = void>
	inline ::std::enable_if_t<Node::_Weighted && ::std::is_same_v<Dummy, Dummy>, ::std::pair<_Traversor, double>>
	shortest_out_pair () const
	{ return _shortest_pair<1>(); }

	template <typename Dummy = void>
	inline ::std::enable_if_t<Node::_Weighted && ::std::is_same_v<Dummy, Dummy>, ::std::pair<_Traversor, double>>
	longest_pair () const
	{
		::std::pair<_Node *, double> in = _longest_pair<0>(), out = _longest_pair<1>();
		return in.second > out.second ? in : out;
	}

	template <typename Dummy = void>
	inline ::std::enable_if_t<Node::_Weighted && ::std::is_same_v<Dummy, Dummy>, ::std::pair<_Traversor, double>>
	longest_in_pair () const
	{ return _longest_pair<0>(); }

	template <typename Dummy = void>
	inline ::std::enable_if_t<Node::_Weighted && ::std::is_same_v<Dummy, Dummy>, ::std::pair<_Traversor, double>>
	longest_out_pair () const
	{ return _longest_pair<1>(); }

	template <typename Dummy = void>
	inline ::std::enable_if_t<Node::_Weighted && ::std::is_same_v<Dummy, Dummy>, ::std::pair<_Traversor, double>>
	closest_pair () const
	{
		::std::pair<_Node *, double> in = _closest_pair<0>(), out = _closest_pair<1>();
		return ::std::abs(in.second) < ::std::abs(out.second) ? in : out;
	}

	template <typename Dummy = void>
	inline ::std::enable_if_t<Node::_Weighted && ::std::is_same_v<Dummy, Dummy>, ::std::pair<_Traversor, double>>
	closest_in_pair () const
	{ return _closest_pair<0>(); }

	template <typename Dummy = void>
	inline ::std::enable_if_t<Node::_Weighted && ::std::is_same_v<Dummy, Dummy>, ::std::pair<_Traversor, double>>
	closest_out_pair () const
	{ return _closest_pair<1>(); }

	private:
	template <bool In_Out>
	inline ::std::enable_if_t<Node::_Weighted && In_Out == In_Out, _Node *>
	_shortest ()
	const
	{
		_Node *node = nullptr; double weight = ::std::numeric_limits<double>::max();
		for(_Handle it =  (In_Out ? _node->out() : _node->in()).begin();
		            it != (In_Out ? _node->out() : _node->in()).end  (); ++it)
			if(it->second < weight) { weight = it->second; node = it->first; }
		return node;
	}

	private:
	template <bool In_Out>
	inline ::std::enable_if_t<Node::_Weighted && In_Out == In_Out, ::std::pair<_Node *, double>>
	_shortest_pair ()
	const
	{
		_Node *node = nullptr; double weight = ::std::numeric_limits<double>::max();
		for(_Handle it =  (In_Out ? _node->out() : _node->in()).begin();
		            it != (In_Out ? _node->out() : _node->in()).end  (); ++it)
			if(it->second < weight) { weight = it->second; node = it->first; }
		return {node, weight};
	}

	private:
	template <bool In_Out>
	inline ::std::enable_if_t<Node::_Weighted && In_Out == In_Out, _Node *>
	_longest ()
	const
	{
		_Node *node = nullptr; double weight = ::std::numeric_limits<double>::lowest();
		for(_Handle it =  (In_Out ? _node->out() : _node->in()).begin();
		            it != (In_Out ? _node->out() : _node->in()).end  (); ++it)
			if(it->second > weight) { weight = it->second; node = it->first; }
		return node;
	}

	private:
	template <bool In_Out>
	inline ::std::enable_if_t<Node::_Weighted && In_Out == In_Out, ::std::pair<_Node *, double>>
	_longest_pair ()
	const
	{
		_Node *node = nullptr; double weight = ::std::numeric_limits<double>::lowest();
		for(_Handle it =  (In_Out ? _node->out() : _node->in()).begin();
		            it != (In_Out ? _node->out() : _node->in()).end  (); ++it)
			if(it->second > weight) { weight = it->second; node = it->first; }
		return {node, weight};
	}

	private:
	template <bool In_Out>
	inline ::std::enable_if_t<Node::_Weighted && In_Out == In_Out, _Node *>
	_closest ()
	const
	{
		_Node *node = nullptr; double weight = ::std::numeric_limits<double>::max();
		for(_Handle it =  (In_Out ? _node->out() : _node->in()).begin();
		            it != (In_Out ? _node->out() : _node->in()).end  (); ++it)
			if(::std::abs(it->second) < ::std::abs(weight)) { weight = it->second; node = it->first; }
		return node;
	}

	private:
	template <bool In_Out>
	inline ::std::enable_if_t<Node::_Weighted && In_Out == In_Out, ::std::pair<_Node *, double>>
	_closest_pair ()
	const
	{
		_Node *node = nullptr; double weight = ::std::numeric_limits<double>::max();
		for(_Handle it =  (In_Out ? _node->out() : _node->in()).begin();
		            it != (In_Out ? _node->out() : _node->in()).end  (); ++it)
			if(::std::abs(it->second) < ::std::abs(weight)) { weight = it->second; node = it->first; }
		return {node, weight};
	}
	/* === Traversal === */


	/* === Adjacency === */
	public:
	inline size_t size     () const { return _node->out().size(); }
	inline size_t size_in  () const { return _node->in ().size(); }
	inline size_t size_out () const { return _node->out().size(); }

	template <typename Dummy = void> inline ::std::enable_if_t<!Const && ::std::is_same_v<Dummy, Dummy>, _ATraversor>
	                       begin     ()       { return  _ATraversor(_node->out(). begin()); }
	inline   _ACTraversor  begin     () const { return _ACTraversor(_node->out().cbegin()); }
	inline   _ACTraversor cbegin     () const { return _ACTraversor(_node->out().cbegin()); }

	template <typename Dummy = void> inline ::std::enable_if_t<!Const && ::std::is_same_v<Dummy, Dummy>, _ATraversor>
	                       begin_in  ()       { return  _ATraversor(_node->in (). begin()); }
	inline   _ACTraversor  begin_in  () const { return _ACTraversor(_node->in ().cbegin()); }
	inline   _ACTraversor cbegin_in  () const { return _ACTraversor(_node->in ().cbegin()); }

	template <typename Dummy = void> inline ::std::enable_if_t<!Const && ::std::is_same_v<Dummy, Dummy>, _ATraversor>
	                       begin_out ()       { return  _ATraversor(_node->out(). begin()); }
	inline   _ACTraversor  begin_out () const { return _ACTraversor(_node->out().cbegin()); }
	inline   _ACTraversor cbegin_out () const { return _ACTraversor(_node->out().cbegin()); }

	template <typename Dummy = void> inline ::std::enable_if_t<!Const && ::std::is_same_v<Dummy, Dummy>, _ATraversor>
	                       end     ()       { return  _ATraversor(_node->out(). end()); }
	inline   _ACTraversor  end     () const { return _ACTraversor(_node->out().cend()); }
	inline   _ACTraversor cend     () const { return _ACTraversor(_node->out().cend()); }

	template <typename Dummy = void> inline ::std::enable_if_t<!Const && ::std::is_same_v<Dummy, Dummy>, _ATraversor>
	                       end_in  ()       { return  _ATraversor(_node->in (). end()); }
	inline   _ACTraversor  end_in  () const { return _ACTraversor(_node->in ().cend()); }
	inline   _ACTraversor cend_in  () const { return _ACTraversor(_node->in ().cend()); }

	template <typename Dummy = void> inline ::std::enable_if_t<!Const && ::std::is_same_v<Dummy, Dummy>, _ATraversor>
	                       end_out ()       { return  _ATraversor(_node->out(). end()); }
	inline   _ACTraversor  end_out () const { return _ACTraversor(_node->out().cend()); }
	inline   _ACTraversor cend_out () const { return _ACTraversor(_node->out().cend()); }
	/* === Adjacency === */
};

template <bool Const, typename Node, typename Info>
struct graph_adjacency_traversor
{
	template <bool, typename, typename> friend struct graph_adjacency_traversor;
	template <bool, bool, typename, typename> friend struct graph;
	template <typename, typename, typename> friend struct graph_default;
	template <bool, typename, typename, typename> friend struct graph_weighted;
	template <bool, typename, typename, typename> friend struct graph_directed;
	template <bool, typename, typename, bool> friend struct graph_traversor;
	template <bool, typename, typename, typename, int> friend struct graph_path_traversor;

	private:
	typedef _select_t<Const, const Node, Node> _Node;
	typedef _select_t<Const, const Info, Info> _Info;
	typedef _select_t<Const, typename Node::_Adjacency::const_iterator, typename Node::_Adjacency::iterator> _Handle;
	typedef _select_t<Const, const double, double> _Weight;

	private:
	typedef graph_adjacency_traversor<false, Node, Info>  _ATraversor;
	typedef graph_adjacency_traversor<true , Node, Info> _ACTraversor;


	/* === Variables === */
	private:
	_Handle _handle;
	inline _Node *node () const { return Node::node(_handle); }
	/* === Variables === */


	/* === Constructor / Destructor === */
	public:
	graph_adjacency_traversor (_Handle &&handle = _Handle()) : _handle(handle) {}

	graph_adjacency_traversor (const graph_adjacency_traversor<false, Node, Info> &other) : _handle(other._handle) {}
	/* === Constructor / Destructor === */


	/* === Info === */
	public:
	inline _Info info        () const { return **node(); }
	inline _Info &operator*  () const { return **node(); }
	inline _Info *operator-> () const { return node()->info(); }

	inline bool operator() () const { return _handle._M_cur != nullptr; }
	template <typename T> inline bool same(const T &tr) const { return (*this)() && tr() && node() == tr.node(); }

	template <typename T> inline bool operator== (const T &tr) const { return _handle == tr._handle; }
	template <typename T> inline bool operator!= (const T &tr) const { return _handle != tr._handle; }

	template <typename Dummy = void>
	inline ::std::enable_if_t<Node::_Weighted && ::std::is_same_v<Dummy, Dummy>, _Weight &>
	weight ()
	const
	{ return _handle->second; }

	template <typename Dummy = void>
	inline ::std::enable_if_t<Node::_Weighted && ::std::is_same_v<Dummy, Dummy>, ::std::pair<_Info &, _Weight &>>
	pair ()
	const
	{ return {**_handle->first, _handle->second}; }
	/* === Info === */


	/* === Traversal === */
	public:
	inline _ATraversor &operator-- ()    {                          --_handle; return *this; }
	inline _ATraversor  operator-- (int) { _ATraversor tmp = *this; --_handle; return tmp;   }
	inline _ATraversor &operator++ ()    {                          ++_handle; return *this; }
	inline _ATraversor  operator++ (int) { _ATraversor tmp = *this; ++_handle; return tmp;   }

	inline _ATraversor &go_prev () { --_handle; return *this; }
	inline _ATraversor &go_next () { ++_handle; return *this; }

	inline _ATraversor prev () const { return _ATraversor(::std::prev(_handle)); }
	inline _ATraversor next () const { return _ATraversor(::std::next(_handle)); }
	/* === Traversal === */
};

template <bool Const, typename Node, typename Path, typename Info, int Direction>
struct graph_path_traversor
{
	#define _thread(D) D ? (_index == _path->_size ? 0 : _index + 1) : (_index == 0 ? _path->_size : _index - 1)

	template <bool, typename, typename, typename, int> friend struct graph_path_traversor;
	template <bool, bool, typename, typename> friend struct graph;
	template <typename, typename, typename> friend struct graph_default;
	template <bool, typename, typename, typename> friend struct graph_weighted;
	template <bool, typename, typename, typename> friend struct graph_directed;
	template <bool, typename, typename, bool> friend struct graph_traversor;
	template <bool, typename, typename> friend struct graph_adjacency_traversor;

	private:
	typedef _select_t<Const, const Path, Path> _Path;
	typedef _select_t<Const, const Node, Node> _Node;
	typedef _select_t<Const, const Info, Info> _Info;

	private:
	typedef graph_path_traversor<Const, Node, Path, Info, Direction> _PTraversor;
	typedef graph_path_traversor<Const, Node, Path, Info, Direction*-1> _RPTraversor;


	/* === Variables === */
	private:
	_Path *_path;
	size_t _index;
	inline _Node *node () const { return _path->_path[_index]; }
	/* === Variables === */


	/* === Constructor / Destructor === */
	public:
	graph_path_traversor (_Path *path = nullptr, size_t index = 0) : _path(path), _index(index) {}

	template <int Direction_Other>
	graph_path_traversor (const graph_path_traversor<false, Node, Path, Info, Direction_Other> &other)
		: _path(other._path), _index(other._index) {}
	/* === Constructor / Destructor === */


	/* === Reverse === */
	public:
	inline _RPTraversor reverse () { return _RPTraversor(_path, _index); }
	/* === Reverse === */


	/* === Info === */
	public:
	inline _Info info        () const { return **_path->_path[_index]; }
	inline _Info &operator*  () const { return **_path->_path[_index]; }
	inline _Info *operator-> () const { return _path->_path[_index]->info(); }

	inline bool operator() () const { return _path != nullptr; }
	template <typename T> inline bool same(const T &tr) const { return (*this)() && tr() && node() == tr.node(); }

	template <typename T> inline bool operator== (const T &tr) const { return _path == tr._path && _index == tr._index; }
	template <typename T> inline bool operator!= (const T &tr) const { return _path != tr._path || _index != tr._index; }

	auto length () const { return length_total(); }

	template <typename Dummy = void>
	inline ::std::enable_if_t<!Node::_Weighted && ::std::is_same_v<Dummy, Dummy>, size_t>
	length_last ()
	const
	{ return _index == _path->_size || _index == 0 ? 0 : 1; }

	template <typename Dummy = void>
	inline ::std::enable_if_t<Node::_Weighted && ::std::is_same_v<Dummy, Dummy>, double>
	length_last ()
	const
	{ return _index == _path->_size || _index == 0 ? 0 : _path->_length[_index] - _path->_length[_index-1]; }

	template <typename Dummy = void>
	inline ::std::enable_if_t<!Node::_Weighted && ::std::is_same_v<Dummy, Dummy>, size_t>
	length_total ()
	const
	{ return _index == _path->_size ? 0 : _index; }

	template <typename Dummy = void>
	inline ::std::enable_if_t<Node::_Weighted && ::std::is_same_v<Dummy, Dummy>, double>
	length_total ()
	const
	{ return _index == _path->_size ? 0 : _path->_length[_index]; }
	/* === Info === */


	/* === Traversal === */
	public:
	inline _PTraversor &operator-- ()    {                          _index = _thread(Direction^1); return *this; }
	inline _PTraversor  operator-- (int) { _PTraversor tmp = *this; _index = _thread(Direction^1); return tmp;   }
	inline _PTraversor &operator++ ()    {                          _index = _thread(Direction);   return *this; }
	inline _PTraversor  operator++ (int) { _PTraversor tmp = *this; _index = _thread(Direction);   return tmp;   }

	inline _PTraversor &go_prev () { _index = _thread(Direction^1); return *this; }
	inline _PTraversor &go_next () { _index = _thread(Direction);   return *this; }

	inline _PTraversor prev () const { return _PTraversor(_path, _thread(Direction^1)); }
	inline _PTraversor next () const { return _PTraversor(_path, _thread(Direction));   }
	/* === Traversal === */

	#undef _thread
};

#endif
