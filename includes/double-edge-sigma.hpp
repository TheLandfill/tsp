#pragma once
#include "matrix.hpp"
#include "max-or-inf.hpp"
#include "double-edge-nearest-neighbor.hpp"
#include "edge.hpp"
#include "print-info.hpp"
#include "k-opt.hpp"
#include <unordered_set>
#include <random>
#include <iostream>
#include <cmath>

// As much as I love the name Double_Edge_Sigma, this is literally copied and
// pasted with some minor tweaks. It would be better to implement a Sigma
// algorithm that takes in a function that generates a path from some inputs.
// I'm keeping it for now because I want to test the ideas first.
template<typename T>
class Double_Edge_Sigma {
public:
	Double_Edge_Sigma(uint32_t seed, double recency = 1.0 / 1024.0);
	void grind(const Matrix<T>& mat, size_t start, size_t num_iterations, bool run_kopt = true);
	const Matrix<double>& get_freq_mat();
	T get_min_path_length() const;
	const std::vector<size_t>& get_shortest_path_found();
private:
	Matrix<double> frequency;
	std::vector<size_t> shortest_path_found;
	T min_path_length;
	std::mt19937 rng;
	double recency;
};

template<typename T>
Double_Edge_Sigma<T>::Double_Edge_Sigma(uint32_t s, double r) :
	min_path_length{get_max_val_or_inf<T>()},
	rng{s},
	recency(r)
{}

template<typename T>
void Double_Edge_Sigma<T>::grind(const Matrix<T>& mat, size_t start, size_t num_iterations, bool run_kopt) {
	frequency = Matrix<double>(mat.get_num_rows(), mat.get_num_cols(), [](){ return 0; });
	std::uniform_int_distribution<size_t> city_selector_dist{0, mat.get_num_cols() - 1};
	Double_Edge_Nearest_Neighbor<T> denn{mat, start};
	Matrix<T> cur_mat = mat;
	K_Opt<T> kopt(137, 5);
	double cur_path_length = 1.0;
	double thing_to_add = 0.0;
	for (size_t i = 0; i < num_iterations; i++) {
		denn.run(city_selector_dist(rng));
		std::vector<size_t> path = denn.get_path();
		if (run_kopt) {
			//print_path(path);
			//std::cout << "Path Length: " << get_path_length(path, mat) << "\n";
			kopt.run(mat, start, path);
			//print_path(path);
			//std::cout << "Path Length: " << get_path_length(path, mat) << "\n";
			//std::cout << "--------------------------------------------------------------------------------\n";
		}
		cur_path_length = get_path_length(path, mat);
		if (cur_path_length < min_path_length) {
			min_path_length = get_path_length(path, mat);
			shortest_path_found = path;
		}
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
			frequency.at(cur, next) *= (1.0 - recency);
			frequency.at(cur, next) += thing_to_add * recency;
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
const Matrix<double>& Double_Edge_Sigma<T>::get_freq_mat() {
	return frequency;
}

template<typename T>
T Double_Edge_Sigma<T>::get_min_path_length() const {
	return min_path_length;
}

