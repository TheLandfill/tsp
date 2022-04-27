#pragma once
#include "matrix.hpp"
#include "max-or-inf.hpp"
#include "tsp-linked-list.hpp"
#include <vector>
#include <unordered_set>

// Nearest-Neighbor Algorithm, but starting from the middle of the path
template<typename T>
class Double_Edge_Nearest_Neighbor {
public:
	// start is given in the constructor so that DENN can set the edges leading
	// into the start node to be too high to be chosen.
	Double_Edge_Nearest_Neighbor(const Matrix<T>& mat, size_t start);
	void run(size_t initial_node);
	const std::vector<size_t>& get_path() const;
private:
	Matrix<T> mat;
	std::unordered_set<size_t> visited;
	std::vector<size_t> path;
	size_t start;
};

template<typename T>
Double_Edge_Nearest_Neighbor<T>::Double_Edge_Nearest_Neighbor(
	const Matrix<T>& in_mat,
	size_t s
) : mat(in_mat), start(s) {
	for (size_t row = 0; row < mat.get_num_cols(); row++) {
		mat.at(row, start) = get_max_val_or_inf<T>() / mat.get_num_cols();
	}
	visited.reserve(mat.get_num_cols());
	path.reserve(mat.get_num_cols());
}

template<typename T>
void Double_Edge_Nearest_Neighbor<T>::run(size_t initial_node) {
	visited.clear();
	path.clear();
	visited.insert(initial_node);
	size_t cur_beg = initial_node;
	size_t cur_end = initial_node;
	TSP_Linked_List tsp_ll{mat.get_num_cols()};
	while (visited.size() < mat.get_num_cols()) {
		T cur_length = get_max_val_or_inf<T>();
		size_t next_beg = start;
		size_t next_end = (size_t)-1;
		for (size_t i = 0; i < mat.get_num_cols(); i++) {
			if (visited.count(i)) {
				continue;
			}
			T temp_length = mat.at(cur_end, i);
			if (temp_length < cur_length) {
				cur_length = temp_length;
				next_end = i;
			}
			temp_length = mat.at(i, cur_beg);
			if (temp_length < cur_length) {
				cur_length = temp_length;
				next_beg = i;
			}
		}
		if (mat.at(next_beg, cur_beg) < mat.at(cur_end, next_end)) {
			tsp_ll.connect(next_beg, cur_beg);
			cur_beg = next_beg;
			visited.insert(cur_beg);
		} else {
			tsp_ll.connect(cur_end, next_end);
			cur_end = next_end;
			visited.insert(cur_end);
		}
	}
	path = tsp_ll.get_path(start);
}

template<typename T>
const std::vector<size_t>& Double_Edge_Nearest_Neighbor<T>::get_path() const {
	return path;
}
