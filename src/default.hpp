#ifndef _GMD_GRAPH_DEFAULT_H_
#define _GMD_GRAPH_DEFAULT_H_

template <typename Node, typename Info, typename Allocator>
struct graph_default
{
	private:
	typedef graph_weighted<Node::_Weighted, Node, Info, Allocator> _GWeighted;
	typedef graph_directed<Node::_Directed, Node, Info, Allocator> _GDirected;
	typedef graph_path<Node, Info, Allocator> _Path;
	typedef ::std::unordered_map<Node *, Node *, ::std::hash<Node *>, ::std::equal_to<Node *>, Allocator> _Map;
	typedef ::std::deque<Node *, Allocator> _Deque;
	typedef typename Node::_Adjacency::iterator _It;


	/* ##################################################################### */
	/* ############################ Pathfinding ############################ */
	/* === Breadth-First Search === */
	public:
	template <bool Bidirectional = true, typename T1, typename T2>
	static inline _Path
	bfs (const T1 &start, const T2 &end)
	{ return !Bidirectional ? _bdfs1<0>(start.node(), end.node()) : _bdfs2<0>(start.node(), end.node()); }

	public:
	template <bool Bidirectional = true, typename T1, typename T2>
	static inline _Path
	breadth_first_search (const T1 &start, const T2 &end)
	{ return !Bidirectional ? _bdfs1<0>(start.node(), end.node()) : _bdfs2<0>(start.node(), end.node()); }
	/* === Breadth-First Search === */


	/* === Depth-First Search === */
	public:
	template <bool Bidirectional = true, typename T1, typename T2>
	static inline _Path
	dfs (const T1 &start, const T2 &end)
	{ return !Bidirectional ? _bdfs1<1>(start.node(), end.node()) : _bdfs2<1>(start.node(), end.node()); }

	public:
	template <bool Bidirectional = true, typename T1, typename T2>
	static inline _Path
	depth_first_search (const T1 &start, const T2 &end)
	{ return !Bidirectional ? _bdfs1<1>(start.node(), end.node()) : _bdfs2<1>(start.node(), end.node()); }
	/* === Depth-First Search === */


	/* === Breadth/Depth -First Search === */
	private:
	template <bool BFS_DFS>
	static _Path
	_bdfs1 (Node *start, Node *end)
	{
		if(start == end) { _Path path(1); path._path[0] = start; return path; }

		_Map parent; parent.emplace(start, nullptr);
		_Deque deque; deque.push_back(start);

		Node *node;
		while(deque.size() > 0) {
			if(!BFS_DFS) { node = deque.front(); deque.pop_front(); }
			else         { node = deque.back (); deque.pop_back (); }
			for(_It it = node->out().begin(); it != node->out().end(); ++it) {
				if(Node::node(it) == end) { parent.emplace(end, node); return _GWeighted::_fill_path(end, parent); }
				if(parent.emplace(Node::node(it), node).second) deque.push_back(Node::node(it)); } }

		return _Path();
	}

	private:
	template <bool BFS_DFS>
	static _Path
	_bdfs2 (Node *start, Node *end)
	{
		if(start == end) { _Path path(1); path._path[0] = start; return path; }

		_Map parent1, parent2; parent1.emplace(start, nullptr); parent2.emplace(end, nullptr);
		_Deque deque1, deque2; deque1.push_back(start);         deque2.push_back(end);

		Node *node;
		while(true) {
			if(!BFS_DFS) { node = deque1.front(); deque1.pop_front(); }
			else         { node = deque1.back (); deque1.pop_back (); }
			for(_It it = node->out().begin(); it != node->out().end(); ++it) {
				if(parent2.count(Node::node(it))) return _GWeighted::_fill_path(node, Node::node(it), parent1, parent2);
				if(parent1.emplace(Node::node(it), node).second) deque1.push_back(Node::node(it)); }
			if(deque1.size() == 0) break;

			if(!BFS_DFS) { node = deque2.front(); deque2.pop_front(); }
			else         { node = deque2.back (); deque2.pop_back (); }
			for(_It it = node->in().begin(); it != node->in().end(); ++it) {
				if(parent1.count(Node::node(it))) return _GWeighted::_fill_path(Node::node(it), node, parent1, parent2);
				if(parent2.emplace(Node::node(it), node).second) deque2.push_back(Node::node(it)); }
			if(deque2.size() == 0) break; }

		return _Path();
	}
	/* === Breadth/Depth -First Search === */
	/* ############################ Pathfinding ############################ */
	/* ##################################################################### */
};

#endif
