#pragma once
#include "matrix.hpp"
#include "max-or-inf.hpp"
#include "edge.hpp"
#include <queue>
#include <unordered_set>
#include <string>
#include <algorithm>

template<typename T>
struct Dijkstra_Node {
	Dijkstra_Node(T cur_dist = get_max_val_or_inf<T>(), size_t node_id = (size_t)-1);
	bool operator<(const Dijkstra_Node<T>& other);
	T cur_dist;
	size_t node_id;
};

template<typename T>
Dijkstra_Node<T>::Dijkstra_Node(T d, size_t p) : cur_dist(d), node_id(p) {}

template<typename T>
bool Dijkstra_Node<T>::operator<(const Dijkstra_Node<T>& other) {
	return cur_dist < other.cur_dist;
}

template<typename T>
bool operator<(const Dijkstra_Node<T> a, const Dijkstra_Node<T> b) {
	return a.cur_dist < b.cur_dist;
}

template<typename T>
class Dijkstra {
public:
	void run(const Matrix<T>& mat, size_t start);
	std::vector<size_t> get_shortest_path_to(size_t end);
	std::vector<Edge> get_edges();
private:
	std::vector<Dijkstra_Node<T>> node_paths;
};

template<typename T>
void Dijkstra<T>::run(const Matrix<T>& mat, size_t start) {
	if (start > mat.get_num_cols()) {
		std::string error_message;
		error_message.reserve(1024);
		error_message += "Start node `";
		error_message += std::to_string(start);
		error_message += "` is out of range `";
		error_message += std::to_string(mat.get_num_cols());
		error_message += "`.";
		throw std::out_of_range(error_message);
	}
	std::unordered_set<size_t> visited;
	visited.reserve(mat.get_num_cols());
	node_paths = std::vector<Dijkstra_Node<T>>(mat.get_num_cols(), Dijkstra_Node<T>{});
	node_paths[start].cur_dist = 0;
	visited.insert(start);
	size_t cur_node = start;
	while (visited.size() < mat.get_num_cols()) {
		T cur_dist = get_max_val_or_inf<T>();
		size_t next_node = 0;
		for (size_t i = 0; i < mat.get_num_cols(); i++) {
			if (visited.count(i)) {
				continue;
			}
			T new_dist = node_paths[cur_node].cur_dist + mat.at(cur_node, i);
			if (new_dist < node_paths[i].cur_dist) {
				node_paths[i] = Dijkstra_Node<T>{new_dist, cur_node};
			}
			if (node_paths[i].cur_dist < cur_dist) {
				cur_dist = node_paths[i].cur_dist;
				next_node = i;
			}
		}
		visited.insert(cur_node);
		cur_node = next_node;
	}
}

template<typename T>
std::vector<size_t> Dijkstra<T>::get_shortest_path_to(size_t end) {
	std::vector<size_t> path;
	path.reserve(node_paths.size());
	size_t cur_node = end;
	while (cur_node != (size_t)-1) {
		path.push_back(cur_node);
		cur_node = node_paths[cur_node].node_id;
	}
	std::reverse(path.begin(), path.end());
	return path;
}

template<typename T>
std::vector<Edge> Dijkstra<T>::get_edges() {
	std::vector<Edge> edges;
	edges.reserve(node_paths.size());
	for (size_t i = 0; i < node_paths.size(); i++) {
		if (node_paths[i].node_id == (size_t)-1) {
			continue;
		}
		edges.push_back({i, node_paths[i].node_id});
	}
	return edges;
}
