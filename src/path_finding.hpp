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
_bdfs1 (_Node *start, _Node *end)
{
	if(start == end) { _Path path(1); path._path[0] = start; return path; }

	_Map parent; parent.emplace(start, nullptr);
	_Deque deque; deque.push_back(start);

	_Node *node;
	while(deque.size() > 0) {
		if(!BFS_DFS) { node = deque.front(); deque.pop_front(); }
		else         { node = deque.back (); deque.pop_back (); }
		for(_It it = node->out().begin(); it != node->out().end(); ++it) {
			if(_Node::node(it) == end) { parent.emplace(end, node); return _GWeighted::_fill_path(end, parent); }
			if(parent.emplace(_Node::node(it), node).second) deque.push_back(_Node::node(it)); } }

	return _Path();
}

private:
template <bool BFS_DFS>
static _Path
_bdfs2 (_Node *start, _Node *end)
{
	if(start == end) { _Path path(1); path._path[0] = start; return path; }

	_Map parent1, parent2; parent1.emplace(start, nullptr); parent2.emplace(end, nullptr);
	_Deque deque1, deque2; deque1.push_back(start);         deque2.push_back(end);

	_Node *node;
	while(true) {
		if(!BFS_DFS) { node = deque1.front(); deque1.pop_front(); }
		else         { node = deque1.back (); deque1.pop_back (); }
		for(_It it = node->out().begin(); it != node->out().end(); ++it) {
			if(parent2.count(_Node::node(it))) return _GWeighted::_fill_path(node, _Node::node(it), parent1, parent2);
			if(parent1.emplace(_Node::node(it), node).second) deque1.push_back(_Node::node(it)); }
		if(deque1.size() == 0) break;

		if(!BFS_DFS) { node = deque2.front(); deque2.pop_front(); }
		else         { node = deque2.back (); deque2.pop_back (); }
		for(_It it = node->in().begin(); it != node->in().end(); ++it) {
			if(parent1.count(_Node::node(it))) return _GWeighted::_fill_path(_Node::node(it), node, parent1, parent2);
			if(parent2.emplace(_Node::node(it), node).second) deque2.push_back(_Node::node(it)); }
		if(deque2.size() == 0) break; }

	return _Path();
}
/* === Breadth/Depth -First Search === */

/* === Fill path === */
private:
static _Path
_fill_path (_Node *end, _Map &parent)
{
	size_t size = 1;
	for(_Node *node = parent[end]; node != nullptr; ++size, node = parent[node]) {}
	_Path path(size);
	path._path[--size] = end;
	for(_Node *node = parent[end]; node != nullptr; node = parent[node]) path._path[--size] = node;
	return path;
}

private:
static _Path
_fill_path (_Node *middle1, _Node *middle2, _Map &parent1, _Map &parent2)
{
	size_t size1 = 1, size2 = 1;
	for(_Node *node = parent1[middle1]; node != nullptr; ++size1, node = parent1[node]) {}
	for(_Node *node = parent2[middle2]; node != nullptr; ++size2, node = parent2[node]) {}
	_Path path(size1 + size2); size2 = size1 - 1;
	path._path[++size2] = middle2;
	for(_Node *node = parent2[middle2]; node != nullptr; node = parent2[node]) path._path[++size2] = node;
	path._path[--size1] = middle1;
	for(_Node *node = parent1[middle1]; node != nullptr; node = parent1[node]) path._path[--size1] = node;
	return path;
}
/* === Fill path === */
