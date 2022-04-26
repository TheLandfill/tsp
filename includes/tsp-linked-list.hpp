#pragma once
#include <cstddef>
#include <vector>

template<typename T>
struct TSP_Linked_List_Node {
	T next, prev;
};

// Need to make sure no cycles
class TSP_Linked_List {
public:
	TSP_Linked_List(size_t num_elements);
	std::vector<size_t> get_path(size_t start);
	size_t get_end_node(size_t start);
	size_t get_prev(size_t cur);
	size_t get_next(size_t cur);
	bool disconnect_next(size_t node);
	bool disconnect_prev(size_t node);
	bool connect(size_t start, size_t end);
private:
	std::vector<TSP_Linked_List_Node<int>> list;
};
