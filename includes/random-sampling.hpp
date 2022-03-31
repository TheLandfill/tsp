#pragma once
#include "matrix.hpp"
#include <unordered_set>
#include <vector>
#include <fstream>
#include <algorithm>
#include <random>

template<typename T>
class Random_Sampler {
public:
	void sample_paths(const Matrix<T>& mat, size_t num_paths_to_sample);
	void write_path_lengths_to_file(const std::filesystem::path& p);
private:
	void sample_path(const Matrix<T>& mat);
private:
	std::default_random_engine engine;
	std::vector<T> path_lengths;
};

template<typename T>
void Random_Sampler<T>::sample_paths(const Matrix<T>& mat, size_t num_paths_to_sample) {
	uint32_t seed = time(NULL);
	engine.seed(seed);
	path_lengths.reserve(num_paths_to_sample);
	for (size_t i = 0; i < num_paths_to_sample; i++) {
		sample_path(mat);
	}
}

template<typename T>
void Random_Sampler<T>::write_path_lengths_to_file(const std::filesystem::path& path) {
	std::ofstream writer{path};
	for (const auto& cur : path_lengths) {
		writer << cur << "\n";
	}
}

template<typename T>
void Random_Sampler<T>::sample_path(const Matrix<T>& mat) {
	std::vector<size_t> nodes;
	nodes.reserve(mat.get_num_rows());
	for (size_t i = 0; i < mat.get_num_rows(); i++) {
		nodes.emplace_back(i);
	}
	std::shuffle(nodes.begin(), nodes.end(), engine);
	T cur_path_length{};
	for (size_t index = 0; index < nodes.size(); index++) {
		size_t cur = nodes[index];
		size_t next = nodes[(index + nodes.size() - 1) % nodes.size()];
		cur_path_length += mat.at(cur, next);
	}
	path_lengths.emplace_back(cur_path_length);
}
