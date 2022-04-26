#pragma once
#include "matrix.hpp"
#include "max-or-inf.hpp"
#include "global-greedy.hpp"
#include "edge.hpp"
#include "print_path.hpp"
#include "k-opt.hpp"
#include <unordered_set>
#include <random>
#include <iostream>
#include <cmath>

template<typename T>
class Sigma_Male {
public:
	Sigma_Male(uint32_t seed);
	void run(const Matrix<T>& mat, size_t start, size_t num_iterations, bool run_kopt = true);
	const Matrix<double>& get_freq_mat();
	T get_min_path_length() const;
private:
	Matrix<double> frequency;
	T min_path_length;
	std::mt19937 rng;
};

template<typename T>
Sigma_Male<T>::Sigma_Male(uint32_t s) : min_path_length{get_max_val_or_inf<T>()}, rng{s} {}

template<typename T>
void Sigma_Male<T>::run(const Matrix<T>& mat, size_t start, size_t num_iterations, bool run_kopt) {
	frequency = Matrix<double>(mat.get_num_rows(), mat.get_num_cols(), [](){ return 0; });
	Global_Greedy<T> gg;
	Matrix<T> cur_mat = mat;
	K_Opt<T> kopt(137, 5);
	double cur_path_length = 1.0;
	double thing_to_add = 0.0;
	for (size_t i = 0; i < num_iterations; i++) {
		gg.run(cur_mat, start);
		std::vector<size_t> path = gg.get_path();
		if (run_kopt) {
			//print_path(path);
			//std::cout << "Path Length: " << get_path_length(path, mat) << "\n";
			kopt.run(mat, start, path);
			//print_path(path);
			//std::cout << "Path Length: " << get_path_length(path, mat) << "\n";
			//std::cout << "--------------------------------------------------------------------------------\n";
		}
		cur_path_length = get_path_length(path, mat);
		min_path_length = std::min(min_path_length, get_path_length(path, mat));
		std::vector<Edge> edges;
		edges.reserve(path.size());
		T path_length{};
		for (size_t j = 0; j < path.size() - 1; j++) {
			const size_t& cur = path[j];
			const size_t& next = path[j + 1];
			if (cur_path_length <= min_path_length) {
				thing_to_add = 1.0;
			} else {
				thing_to_add = 1.0 / (1.0 + cur_path_length - min_path_length);
				thing_to_add *= thing_to_add;
			}
			frequency.at(cur, next) += thing_to_add;
			edges.push_back({cur, next});
			path_length += mat.at(cur, next);
		}
		// std::cout << "Path Length: " << path_length << "\n";
		cur_mat = mat;
		std::vector<double> edge_weights;
		edge_weights.reserve(edges.size());
		for (const Edge& edge : edges) {
			T cur_edge_length = mat.at(edge.start, edge.end);
			edge_weights.push_back(mat.at(edge.start, edge.end));
		}
		std::discrete_distribution<size_t> dist{edge_weights.begin(), edge_weights.end()};
		for (size_t j = 0; j < 2 * sqrt(edges.size()); j++) {
			const Edge& cur_edge = edges[dist(rng)];
			cur_mat.at(cur_edge.start, cur_edge.end) = get_max_val_or_inf<T>() / mat.get_num_cols();
		}
	}
}

template<typename T>
const Matrix<double>& Sigma_Male<T>::get_freq_mat() {
	return frequency;
}

template<typename T>
T Sigma_Male<T>::get_min_path_length() const {
	return min_path_length;
}
