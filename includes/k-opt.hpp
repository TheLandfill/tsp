#pragma once
#include "matrix.hpp"
#include "tsp-linked-list.hpp"
#include "edge.hpp"
#include "backtracking.hpp"
#include <unordered_set>
#include <vector>
#include <random>

template<typename T>
class K_Opt {
public:
	K_Opt(uint32_t seed, uint32_t k = 3);
	void run(const Matrix<T>& mat, size_t start_node, std::vector<size_t>& path);
private:
	void fill_k_opt_matrix(const Matrix<T>& mat, const std::vector<Edge>& parts);
private:
	uint32_t k;
	Matrix<T> k_opt_mat;
	std::mt19937 rng;
	std::vector<Edge> parts;
	std::vector<size_t> starts_of_sub_paths;
};

template<typename T>
K_Opt<T>::K_Opt(uint32_t seed, uint32_t k_param) :
	k(k_param),
	k_opt_mat{k + 1, k + 1, [](){ return T{}; }},
	rng{seed},
	parts{k + 1, {0, 0}}
{
	starts_of_sub_paths.reserve(k + 1);
}

template<typename T>
void K_Opt<T>::run(
	const Matrix<T>& mat,
	size_t start_node,
	std::vector<size_t>& path
) {
	TSP_Linked_List tsp_ll(path.size());
	T path_length{};
	std::vector<double> edge_weights(path.size(), 0.0);
	for (size_t i = 1; i < path.size(); i++) {
		const size_t& prev = path.at(i - 1);
		const size_t& cur = path.at(i);
		tsp_ll.connect(prev, cur);
		edge_weights.at(cur) = mat.at(prev, cur);
		path_length += mat.at(prev, cur);
	}
	Backtracker bt{path_length};
	if ((int64_t)k > (int64_t)(path.size() - 5)) {
		bt.run(mat, path_length);
		path = bt.get_shortest_path();
		return;
	}
	size_t num_rejections = 0;
	const size_t max_num_rejections = path.size() * 4;
	std::discrete_distribution<size_t> dist{edge_weights.begin(), edge_weights.end()};
	std::unordered_set<Edge> removed_edges{};
	while (num_rejections < max_num_rejections) {
		//std::cout << "\x1b[033K\r" << num_rejections;
		removed_edges.clear();
		bt.reset(path_length);
		starts_of_sub_paths.clear();
		starts_of_sub_paths.push_back(start_node);
		bool improvement_made = false;
		for (size_t i = 0; i < k;) {
			size_t edge_to_remove = dist(rng);
			size_t prev_edge = tsp_ll.get_prev(edge_to_remove);
			bool edge_was_removed = tsp_ll.disconnect_prev(edge_to_remove);
			i += edge_was_removed;
			if (edge_was_removed) {
				starts_of_sub_paths.push_back(edge_to_remove);
				removed_edges.insert({prev_edge, edge_to_remove});
			}
		}
		for (size_t i = 0; i < k + 1; i++) {
			const size_t& start = starts_of_sub_paths.at(i);
			parts.at(i) = { start, tsp_ll.get_end_node(start) };
		}
		fill_k_opt_matrix(mat, parts);
		bt.run(k_opt_mat, 0);
		const std::vector<size_t>& k_opt_path = bt.get_shortest_path();
		for (size_t i = 1; i < k_opt_path.size(); i++) {
			const size_t& start = parts.at(k_opt_path.at(i - 1)).end;
			const size_t& end = parts.at(k_opt_path.at(i)).start;
			tsp_ll.connect(start, end);
		}
		for (const Edge& e : removed_edges) {
			if (tsp_ll.get_next(e.start) != e.end) {
				improvement_made = true;
				break;
			}
		}
		if (improvement_made) {
			size_t cur_node = start_node;
			size_t next_node = cur_node;
			while (cur_node != (size_t)-1) {
				next_node = tsp_ll.get_next(cur_node);
				edge_weights.at(cur_node) = mat.at(cur_node, next_node);
				cur_node = next_node;
			}
			dist = std::discrete_distribution<size_t>{edge_weights.begin(), edge_weights.end()};
			num_rejections = 0;
		} else {
			num_rejections += 1;
		}
	}
	std::cout << "\x1b[K\r";
	path = tsp_ll.get_path(start_node);
}

template<typename T>
void K_Opt<T>::fill_k_opt_matrix(const Matrix<T>& mat, const std::vector<Edge>& parts) {
	for (size_t i = 0; i < k + 1; i++) {
		for (size_t j = 1; j < k + 1; j++) {
			if (i == j) {
				continue;
			}
			k_opt_mat.at(i, j) = mat.at(parts.at(i).end, parts.at(j).start);
		}
	}
}
