#pragma once
#include "matrix.hpp"
#include <unordered_set>
#include <vector>
#include <cstddef>

template<typename T>
class Backtracker {
public:
	Backtracker(T initial_path_length);
	void run(const Matrix<T>& mat, size_t start);
	const std::vector<size_t>& get_shortest_path() const;
	T get_shortest_path_length();
private:
	void run_iter(const Matrix<T>& mat);
private:
	std::unordered_set<size_t> visited;
	std::vector<size_t> cur_path;
	std::vector<size_t> shortest_path;
	std::vector<T> cur_path_lengths;
	T shortest_path_length;
};

template<typename T>
Backtracker<T>::Backtracker(T initial_path_length) : shortest_path_length(initial_path_length) {}

template<typename T>
void Backtracker<T>::run(const Matrix<T>& mat, size_t start) {
	visited.insert(start);
	cur_path.push_back(start);
	cur_path_lengths.push_back(0);
	run_iter(mat);
	// std::cout << "\r\x1b[K";
}

template<typename T>
void Backtracker<T>::run_iter(const Matrix<T>& mat) {
	// If we've visited all the cities without going over the current shortest
	// path length, we've found our new shortest path.
	if (visited.size() >= mat.get_num_cols()) {
		shortest_path = cur_path;
		shortest_path_length = cur_path_lengths.back();
		// std::cout << "\r\x1b[K" << shortest_path_length << std::flush;
	}
	for (size_t i = 0; i < mat.get_num_cols(); i++) {
		if (visited.count(i) > 0) {
			continue;
		}
		T dist_to_next = mat.at(cur_path.back(), i) + cur_path_lengths.back();
		if (dist_to_next > shortest_path_length) {
			continue;
		}
		cur_path_lengths.push_back(dist_to_next);
		visited.insert(i);
		cur_path.push_back(i);
		run_iter(mat);
		cur_path.pop_back();
		visited.erase(i);
		cur_path_lengths.pop_back();
	}
}

template<typename T>
const std::vector<size_t>& Backtracker<T>::get_shortest_path() const {
	return shortest_path;
}

template<typename T>
T Backtracker<T>::get_shortest_path_length() {
	return shortest_path_length;
}
