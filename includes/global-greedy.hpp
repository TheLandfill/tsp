#pragma once
#include "matrix.hpp"
#include "tsp-linked-list.hpp"
#include <vector>
#include <algorithm>
#include <queue>

template<typename T>
class Global_Greedy {
public:
	void run(const Matrix<T>& mat, size_t start);
	const std::vector<size_t>& get_path() const;
private:
	std::vector<uint8_t> num_degree;
	std::vector<size_t> path;
	T path_length;
};

template<typename T>
struct Global_Greedy_Node {
	Global_Greedy_Node(T weight, size_t start, size_t end);
	T weight;
	size_t start, end;
};

template<typename T>
Global_Greedy_Node<T>::Global_Greedy_Node(T w, size_t s, size_t e) : weight(w), start(s), end(e) {}

template<typename T>
void Global_Greedy<T>::run(const Matrix<T>& mat, size_t start) {
	num_degree.resize(mat.get_num_cols());
	std::fill(num_degree.begin(), num_degree.end(), 0);
	num_degree[start] = 1;
	size_t num_added_edges = 0;
	std::vector<Global_Greedy_Node<T>> global_greedy_nodes;
	// Technically could get away with (num_cols - 1) * num_rows, but I'm
	// paranoid.
	global_greedy_nodes.reserve(mat.get_num_cols() * mat.get_num_rows());
	for (size_t row = 0; row < mat.get_num_cols(); row++) {
		for (size_t col = 0; col < mat.get_num_cols(); col++) {
			if (row == col || col == start) {
				continue;
			}
			global_greedy_nodes.emplace_back(mat.at(row, col), row, col);
		}
	}
	std::sort(
		global_greedy_nodes.begin(),
		global_greedy_nodes.end(),
		[](const Global_Greedy_Node<T>& a, const Global_Greedy_Node<T>& b){ return a.weight < b.weight; }
	);
	TSP_Linked_List<T> tsp_ll{mat.get_num_cols()};
	for (const auto& node : global_greedy_nodes) {
		if (num_degree[node.start] == 2 || num_degree[node.end] == 2) {
			continue;
		}
		bool added_edge = tsp_ll.connect(node.start, node.end);
		num_added_edges += added_edge;
		num_degree[node.start] += added_edge;
		num_degree[node.end] += added_edge;
		if (num_added_edges == (mat.get_num_cols() - 1)) {
			break;
		}
	}
	path = tsp_ll.get_path(start);
}

template<typename T>
const std::vector<size_t>& Global_Greedy<T>::get_path() const {
	return path;
}
