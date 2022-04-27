#pragma once
#include "matrix.hpp"
#include <vector>
#include <cstddef>

void print_path(const std::vector<size_t>& path);
template<typename T>
T get_path_length(const std::vector<size_t> & path, const Matrix<T>& mat) {
	T path_length{};
	for (size_t i = 0; i < path.size() - 1; i++) {
		const size_t& cur = path[i];
		const size_t& next = path[i + 1];
		path_length += mat.at(cur, next);
	}
	return path_length;
}

void print_accuracy_matrix(
	const std::vector<size_t>& actual_path,
	const Matrix<double>& predicted_freq
);

void print_probability_matrix(
	const Matrix<double>& predicted_freq
);
