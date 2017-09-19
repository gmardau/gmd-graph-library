#ifndef _GMD_GRAPH_NODE_H_
#define _GMD_GRAPH_NODE_H_

template <bool Weighted, bool Directed, typename Info, typename Allocator> struct graph_node;

template <typename Info, typename Allocator>
struct graph_node<false, false, Info, Allocator>
{
	template <bool, bool, typename, typename> friend struct graph;
	template <typename, typename, typename> friend struct graph_default;
	template <bool, typename, typename, typename> friend struct graph_weighted;
	template <bool, typename, typename, typename> friend struct graph_directed;
	template <bool, typename, typename, bool> friend struct graph_traversor;
	template <bool, typename, typename> friend struct graph_adjacency_traversor;
	template <bool, typename, typename, typename, int> friend struct graph_path_traversor;

	private:
	typedef graph_node<false, false, Info, Allocator> _Node;
	typedef ::std::unordered_set<_Node *, ::std::hash<_Node *>, ::std::equal_to<_Node *>, Allocator> _Adjacency;


	/* === Variables === */
	public:
	constexpr static bool _Weighted = false, _Directed = false;
	private:
	::__gnu_cxx::__aligned_membuf<Info> _info;
	_Node *_thread[2];
	_Adjacency _adjacency_list;
	/* === Variables === */


	/* === Constructor / Destructor === */
	private:
	graph_node (_Node *pred, _Node *succ) :_thread{pred, succ} {}
	/* === Constructor / Destructor === */


	/* === Reset === */
	private:
	inline void reset () { _adjacency_list.clear(); }
	/* === Reset === */


	/* === Info === */
	private:
	inline       Info &operator* ()       { return *_info._M_ptr(); }
	inline const Info &operator* () const { return *_info._M_ptr(); }
	inline       Info *info      ()       { return  _info._M_ptr(); }
	inline const Info *info      () const { return  _info._M_ptr(); }
	/* === Info === */


	/* === Adjacency === */
	private:
	inline       _Adjacency &in  ()       { return _adjacency_list; }
	inline const _Adjacency &in  () const { return _adjacency_list; }
	inline       _Adjacency &out ()       { return _adjacency_list; }
	inline const _Adjacency &out () const { return _adjacency_list; }

	inline static       _Node *node (const typename _Adjacency::      iterator &tr) { return *tr; }
	inline static const _Node *node (const typename _Adjacency::const_iterator &tr) { return *tr; }

	constexpr static double weight (const typename _Adjacency::      iterator &tr) { return 0; }
	constexpr static double weight (const typename _Adjacency::const_iterator &tr) { return 0; }
	/* === Adjacency === */
};

template <typename Info, typename Allocator>
struct graph_node<false, true, Info, Allocator>
{
	template <bool, bool, typename, typename> friend struct graph;
	template <typename, typename, typename> friend struct graph_default;
	template <bool, typename, typename, typename> friend struct graph_weighted;
	template <bool, typename, typename, typename> friend struct graph_directed;
	template <bool, typename, typename, bool> friend struct graph_traversor;
	template <bool, typename, typename> friend struct graph_adjacency_traversor;
	template <bool, typename, typename, typename, int> friend struct graph_path_traversor;

	private:
	typedef graph_node<false, true, Info, Allocator> _Node;
	typedef ::std::unordered_set<_Node *, ::std::hash<_Node *>, ::std::equal_to<_Node *>, Allocator> _Adjacency;


	/* === Variables === */
	public:
	constexpr static bool _Weighted = false, _Directed = true;
	private:
	::__gnu_cxx::__aligned_membuf<Info> _info;
	_Node *_thread[2];
	_Adjacency _adjacency_list_in, _adjacency_list_out;
	/* === Variables === */


	/* === Constructor / Destructor === */
	private:
	graph_node (_Node *pred, _Node *succ) :_thread{pred, succ} {}
	/* === Constructor / Destructor === */


	/* === Reset === */
	private:
	inline void
	reset ()
	{ _adjacency_list_in.clear(); _adjacency_list_out.clear(); }
	/* === Reset === */


	/* === Info === */
	private:
	inline       Info &operator* ()       { return *_info._M_ptr(); }
	inline const Info &operator* () const { return *_info._M_ptr(); }
	inline       Info *info      ()       { return  _info._M_ptr(); }
	inline const Info *info      () const { return  _info._M_ptr(); }
	/* === Info === */


	/* === Adjacency === */
	private:
	inline       _Adjacency &in  ()       { return _adjacency_list_in;  }
	inline const _Adjacency &in  () const { return _adjacency_list_in;  }
	inline       _Adjacency &out ()       { return _adjacency_list_out; }
	inline const _Adjacency &out () const { return _adjacency_list_out; }

	inline static       _Node *node (const typename _Adjacency::      iterator &tr) { return *tr; }
	inline static const _Node *node (const typename _Adjacency::const_iterator &tr) { return *tr; }

	constexpr static double weight (const typename _Adjacency::      iterator &tr) { return 0; }
	constexpr static double weight (const typename _Adjacency::const_iterator &tr) { return 0; }
	/* === Adjacency === */
};

template <typename Info, typename Allocator>
struct graph_node<true, false, Info, Allocator>
{
	template <bool, bool, typename, typename> friend struct graph;
	template <typename, typename, typename> friend struct graph_default;
	template <bool, typename, typename, typename> friend struct graph_weighted;
	template <bool, typename, typename, typename> friend struct graph_directed;
	template <bool, typename, typename, bool> friend struct graph_traversor;
	template <bool, typename, typename> friend struct graph_adjacency_traversor;
	template <bool, typename, typename, typename, int> friend struct graph_path_traversor;

	private:
	typedef graph_node<true, false, Info, Allocator> _Node;
	typedef ::std::unordered_map<_Node *, double, ::std::hash<_Node *>, ::std::equal_to<_Node *>, Allocator> _Adjacency;


	/* === Variables === */
	public:
	constexpr static bool _Weighted = true, _Directed = false;
	private:
	::__gnu_cxx::__aligned_membuf<Info> _info;
	_Node *_thread[2];
	_Adjacency _adjacency_list;
	/* === Variables === */


	/* === Constructor / Destructor === */
	private:
	graph_node (_Node *pred, _Node *succ) :_thread{pred, succ} {}
	/* === Constructor / Destructor === */


	/* === Reset === */
	private:
	inline void
	reset ()
	{ _adjacency_list.clear(); }
	/* === Reset === */


	/* === Info === */
	private:
	inline       Info &operator* ()       { return *_info._M_ptr(); }
	inline const Info &operator* () const { return *_info._M_ptr(); }
	inline       Info *info      ()       { return  _info._M_ptr(); }
	inline const Info *info      () const { return  _info._M_ptr(); }
	/* === Info === */


	/* === Adjacency === */
	private:
	inline       _Adjacency &in  ()       { return _adjacency_list; }
	inline const _Adjacency &in  () const { return _adjacency_list; }
	inline       _Adjacency &out ()       { return _adjacency_list; }
	inline const _Adjacency &out () const { return _adjacency_list; }

	inline static       _Node *node (const typename _Adjacency::      iterator &tr) { return tr->first; }
	inline static const _Node *node (const typename _Adjacency::const_iterator &tr) { return tr->first; }

	inline static double weight (const typename _Adjacency::      iterator &tr) { return tr->second; }
	inline static double weight (const typename _Adjacency::const_iterator &tr) { return tr->second; }
	/* === Adjacency === */
};

template <typename Info, typename Allocator>
struct graph_node<true, true, Info, Allocator>
{
	template <bool, bool, typename, typename> friend struct graph;
	template <typename, typename, typename> friend struct graph_default;
	template <bool, typename, typename, typename> friend struct graph_weighted;
	template <bool, typename, typename, typename> friend struct graph_directed;
	template <bool, typename, typename, bool> friend struct graph_traversor;
	template <bool, typename, typename> friend struct graph_adjacency_traversor;
	template <bool, typename, typename, typename, int> friend struct graph_path_traversor;

	private:
	typedef graph_node<true, true, Info, Allocator> _Node;
	typedef ::std::unordered_map<_Node *, double, ::std::hash<_Node *>, ::std::equal_to<_Node *>, Allocator> _Adjacency;


	/* === Variables === */
	public:
	constexpr static bool _Weighted = true, _Directed = true;
	private:
	::__gnu_cxx::__aligned_membuf<Info> _info;
	_Node *_thread[2];
	_Adjacency _adjacency_list_in, _adjacency_list_out;
	/* === Variables === */


	/* === Constructor / Destructor === */
	private:
	graph_node (_Node *pred, _Node *succ) :_thread{pred, succ} {}
	/* === Constructor / Destructor === */


	/* === Reset === */
	private:
	inline void
	reset ()
	{ _adjacency_list_in.clear(); _adjacency_list_out.clear(); }
	/* === Reset === */


	/* === Info === */
	private:
	inline       Info &operator* ()       { return *_info._M_ptr(); }
	inline const Info &operator* () const { return *_info._M_ptr(); }
	inline       Info *info      ()       { return  _info._M_ptr(); }
	inline const Info *info      () const { return  _info._M_ptr(); }
	/* === Info === */


	/* === Adjacency === */
	private:
	inline       _Adjacency &in  ()       { return _adjacency_list_in;  }
	inline const _Adjacency &in  () const { return _adjacency_list_in;  }
	inline       _Adjacency &out ()       { return _adjacency_list_out; }
	inline const _Adjacency &out () const { return _adjacency_list_out; }

	inline static       _Node *node (const typename _Adjacency::      iterator &tr) { return tr->first; }
	inline static const _Node *node (const typename _Adjacency::const_iterator &tr) { return tr->first; }

	inline static double weight (const typename _Adjacency::      iterator &tr) { return tr->second; }
	inline static double weight (const typename _Adjacency::const_iterator &tr) { return tr->second; }
	/* === Adjacency === */
};

#endif
