#pragma once
#include <cstddef>
#include <vector>

template<typename T>
struct TSP_Linked_List_Node {
	T next, prev;
};

// Need to make sure no cycles
template<typename T>
class TSP_Linked_List {
public:
	TSP_Linked_List(size_t num_elements);
	std::vector<size_t> get_path(size_t start);
	size_t get_end_node(size_t start);
	void disconnect_next(size_t node);
	void disconnect_prev(size_t node);
	bool connect(size_t start, size_t end);
private:
	std::vector<TSP_Linked_List_Node<int>> list;
};

template<typename T>
TSP_Linked_List<T>::TSP_Linked_List(size_t num_elements) : list(num_elements, {-1, -1}) {}

template<typename T>
std::vector<size_t> TSP_Linked_List<T>::get_path(size_t start) {
	int cur_value = list[start].next;
	std::vector<size_t> out;
	out.reserve(list.size());
	out.push_back(start);
	while (cur_value != -1) {
		out.push_back(cur_value);
		cur_value = list[cur_value].next;
	}
	return out;
}

template<typename T>
size_t TSP_Linked_List<T>::get_end_node(size_t start) {
	int prev_value = start;
	int cur_value = start;
	while (cur_value != -1) {
		prev_value = cur_value;
		cur_value = list[cur_value].next;
	}
	return prev_value;
}

template<typename T>
void TSP_Linked_List<T>::disconnect_next(size_t node) {
	if (list[node].next == -1) {
		return;
	}
	list[list[node].next].prev = -1;
	list[node].next = -1;
}

template<typename T>
void TSP_Linked_List<T>::disconnect_prev(size_t node) {
	if (list[node].prev == -1) {
		return;
	}
	list[list[node].prev].next = -1;
	list[node].prev = -1;
}

template<typename T>
bool TSP_Linked_List<T>::connect(size_t start, size_t end) {
	if (
		list[start].next != -1 ||
		list[end].prev != -1 ||
		get_end_node(end) == start
	) {
		return false;
	}
	list[start].next = end;
	list[end].prev = start;
	return true;
}
