#pragma once
#include "matrix.hpp"

void print_accuracy_matrix(
	const std::vector<size_t>& actual_path,
	const Matrix<double>& predicted_freq
);
