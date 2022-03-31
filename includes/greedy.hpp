#pragma once
#include "matrix.hpp"
#include <vector>
#include <unordered_set>

template<typename T>
class Greedy {
public:
	void run(const Matrix<T>& mat, size_t start);
	const std::vector<size_t>& get_path() const;
	T get_path_length();
private:
	std::unordered_set<size_t> visited;
	std::vector<size_t> path;
	T path_length;
};

template<typename T>
void Greedy<T>::run(const Matrix<T>& mat, size_t start) {
	visited.insert(start);
	path.emplace_back(start);
	size_t cur = start;
	while (visited.size() < mat.get_num_cols()) {
		T cur_length = 0;
		bool city_found = false;
		size_t next = (size_t)-1;
		for (size_t i = 0; i < mat.get_num_cols(); i++) {
			if (visited.count(i)) {
				continue;
			}
			T temp_length = mat.at(cur, i);
			if ((!city_found) || temp_length < cur_length) {
				cur_length = temp_length;
				next = i;
				city_found = true;
			}
		}
		path_length += cur_length;
		path.emplace_back(next);
		visited.insert(next);
		cur = next;
	}
}

template<typename T>
const std::vector<size_t>& Greedy<T>::get_path() const {
	return path;
}

template<typename T>
T Greedy<T>::get_path_length() {
	return path_length;
}

