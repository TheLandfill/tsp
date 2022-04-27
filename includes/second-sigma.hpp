#pragma once
#include "matrix.hpp"
#include "max-or-inf.hpp"
#include "global-greedy.hpp"
#include "edge.hpp"
#include "k-opt.hpp"

template<typename T>
class Second_Sigma {
public:
	Second_Sigma(uint32_t seed, T min_path_length = get_max_val_or_inf<T>(), double recency = 1.0 / 1024.0);
	void grind(
		const Matrix<T>& actual_mat,
		Matrix<double>& edge_weights,
		size_t start,
		size_t num_iterations,
		bool run_kopt = true
	);
	const std::vector<size_t>& get_shortest_path_found() const;
	T get_min_path_length() const;
	size_t get_iter() const;
private:
	void fill_matrix(const Matrix<T>& mat, const Matrix<double>& edge_weights);
private:
	Matrix<double> decision_mat;
	std::vector<size_t> shortest_path_found;
	T min_path_length;
	std::mt19937 rng;
	double recency;
	size_t iteration_min_path_length_found = -1;
};

template<typename T>
Second_Sigma<T>::Second_Sigma(uint32_t s, T mpl, double r) :
	min_path_length{mpl},
	rng{s},
	recency{r}
{}

template<typename T>
void Second_Sigma<T>::grind(
	const Matrix<T>& mat,
	Matrix<double>& freq_mat,
	size_t start,
	size_t num_iterations,
	bool run_kopt
) {
	Global_Greedy<double> gg;
	decision_mat = Matrix<double>{mat.get_num_rows(), mat.get_num_cols(), [](){ return 0.0; }};
	fill_matrix(mat, freq_mat);
	K_Opt<T> kopt(137, 5);
	double thing_to_add = 0.0;
	double cur_path_length = 1.0;
	for (size_t iter = 0; iter < num_iterations; iter++) {
		gg.run(decision_mat, start);
		std::vector<size_t> path = gg.get_path();
		if (run_kopt) {
			kopt.run(mat, start, path);
		}
		cur_path_length = get_path_length(path, mat);
		if (cur_path_length < min_path_length) {
			min_path_length = get_path_length(path, mat);
			shortest_path_found = path;
			iteration_min_path_length_found = iter;
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
			freq_mat.at(cur, next) *= (1.0 - recency);
			freq_mat.at(cur, next) += (thing_to_add * recency);
			edges.push_back({cur, next});
			path_length += mat.at(cur, next);
		}
		// std::cout << "Path Length: " << path_length << "\n";
		fill_matrix(mat, freq_mat);
		std::vector<double> edge_weights;
		edge_weights.reserve(edges.size());
		for (const Edge& edge : edges) {
			T cur_edge_length = mat.at(edge.start, edge.end);
			edge_weights.push_back(mat.at(edge.start, edge.end));
		}
		std::discrete_distribution<size_t> dist{edge_weights.begin(), edge_weights.end()};
		for (size_t j = 0; j < 2 * sqrt(edges.size()); j++) {
			const Edge& cur_edge = edges[dist(rng)];
			decision_mat.at(cur_edge.start, cur_edge.end) = get_max_val_or_inf<T>() / mat.get_num_cols();
		}
	}
}

template<typename T>
void Second_Sigma<T>::fill_matrix(const Matrix<T>& mat, const Matrix<double>& freq_mat) {
	for (size_t row = 0; row < mat.get_num_rows(); row++) {
		for (size_t col = 0; col < mat.get_num_cols(); col++) {
			double cur_freq = freq_mat.at(row, col);
			if (cur_freq <= 0.00001) {
				decision_mat.at(row, col) = get_max_val_or_inf<double>() / mat.get_num_cols();
			} else {
				decision_mat.at(row, col) = mat.at(row, col) / cur_freq;
			}
		}
	}
}

template<typename T>
const std::vector<size_t>& Second_Sigma<T>::get_shortest_path_found() const {
	return shortest_path_found;
}

template<typename T>
T Second_Sigma<T>::get_min_path_length() const {
	return min_path_length;
}

template<typename T>
size_t Second_Sigma<T>::get_iter() const {
	return iteration_min_path_length_found;
}
