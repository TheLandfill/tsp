#include "tsp-linked-list.hpp"

TSP_Linked_List::TSP_Linked_List(size_t num_elements) : list(num_elements, {-1, -1}) {}

std::vector<size_t> TSP_Linked_List::get_path(size_t start) {
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

size_t TSP_Linked_List::get_end_node(size_t start) {
	int prev_value = start;
	int cur_value = start;
	while (cur_value != -1) {
		prev_value = cur_value;
		cur_value = list[cur_value].next;
	}
	return prev_value;
}

size_t TSP_Linked_List::get_prev(size_t cur) {
	return list[cur].prev;
}

size_t TSP_Linked_List::get_next(size_t cur) {
	return list[cur].next;
}

bool TSP_Linked_List::disconnect_next(size_t node) {
	if (list[node].next == -1) {
		return false;
	}
	list[list[node].next].prev = -1;
	list[node].next = -1;
	return true;
}

bool TSP_Linked_List::disconnect_prev(size_t node) {
	if (list[node].prev == -1) {
		return false;
	}
	list[list[node].prev].next = -1;
	list[node].prev = -1;
	return true;
}

bool TSP_Linked_List::connect(size_t start, size_t end) {
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
