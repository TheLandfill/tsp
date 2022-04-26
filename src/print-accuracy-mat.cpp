#include "print-accuracy-mat.hpp"
#include <algorithm>

void print_accuracy_matrix(
	const std::vector<size_t>& actual_path,
	const Matrix<uint32_t>& predicted_freq,
	double scale
) {
	Matrix<uint32_t> true_freq{predicted_freq.get_num_rows(), predicted_freq.get_num_cols(), [](){ return 0; }};
	for (size_t i = 0; i < actual_path.size() - 1; i++) {
		const size_t& cur = actual_path[i];
		const size_t& next = actual_path[i + 1];
		true_freq.at(cur, next) = 1;
	}
	for (size_t row = 0; row < predicted_freq.get_num_rows(); row++) {
		for (size_t col = 0; col < predicted_freq.get_num_cols(); col++) {
			uint8_t match_type = 0;
			match_type |= (true_freq.at(row, col) > 0);
			match_type |= (predicted_freq.at(row, col) > 0) << 1;
			uint32_t val = std::clamp<uint32_t>(predicted_freq.at(row, col) * scale, 16, 255);
			switch (match_type) {
			case 0:
				std::cout << "\x1b[48;2;0;0;0m ";
				break;
			case 1:
				std::cout << "\x1b[48;2;255;0;0m ";
				break;
			case 2:
				std::cout << "\x1b[48;2;0;0;" << val << "m ";
				break;
			case 3:
				std::cout << "\x1b[48;2;0;" << val << ";0m ";
				break;
			}
		}
		std::cout << "\x1b[m\n";
	}
}
