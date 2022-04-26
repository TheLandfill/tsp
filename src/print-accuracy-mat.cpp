#include "print-accuracy-mat.hpp"
#include "max-or-inf.hpp"
#include <algorithm>
#include <iomanip>

void print_accuracy_matrix(
	const std::vector<size_t>& actual_path,
	const Matrix<double>& predicted_freq
) {
	Matrix<uint32_t> true_freq{predicted_freq.get_num_rows(), predicted_freq.get_num_cols(), [](){ return 0; }};
	for (size_t i = 0; i < actual_path.size() - 1; i++) {
		const size_t& cur = actual_path[i];
		const size_t& next = actual_path[i + 1];
		true_freq.at(cur, next) = 1;
	}
	double total_hhi = 0.0;
	double estimated_number_of_correct_edges = 0.0;
	for (size_t row = 0; row < predicted_freq.get_num_rows(); row++) {
		double total = 0.0;
		double max_value = 0.0;
		for (size_t col = 0; col < predicted_freq.get_num_cols(); col++) {
			total += predicted_freq.at(row, col);
		}
		double hhi = 0.0;
		double actual_value = 0.0;
		for (size_t col = 0; col < predicted_freq.get_num_cols(); col++) {
			uint8_t match_type = 0;
			double value = predicted_freq.at(row, col) / total;
			hhi += value * value;
			match_type |= (true_freq.at(row, col) > 0);
			match_type |= (predicted_freq.at(row, col) > 0) << 1;
			if (value > max_value) {
				max_value = value;
			}
			uint32_t val = std::clamp<uint32_t>(value * 256.0, 16, 255);
			switch (match_type) {
			case 0:
				std::cout << "\x1b[48;2;0;0;0m ";
				break;
			case 1:
				std::cout << "\x1b[48;2;0;0;255m ";
				break;
			case 2:
				std::cout << "\x1b[48;2;" << val << ";0;0m ";
				break;
			case 3:
				actual_value = value;
				estimated_number_of_correct_edges += actual_value;
				std::cout << "\x1b[48;2;0;" << val << ";0m ";
				break;
			}
		}
		std::cout
			<< "\x1b[m|    "
			<< std::setprecision(2)
			<< std::setfill('0')
			<< std::fixed
			<< hhi
			<< "    "
			<< std::setprecision(2)
			<< std::setfill('0')
			<< std::fixed
			<< actual_value
			<< "    "
			<< std::setprecision(2)
			<< std::setfill('0')
			<< std::fixed
			<< max_value
			<< "\n";
		total_hhi += hhi;
	}
	std::cout << "Total hhi: " << std::setprecision(4) << total_hhi << "\n";
	std::cout << "\"Estimated\" Number of Correct Edges: " << std::setprecision(2) << estimated_number_of_correct_edges << "\n";
	// for (double i = 0.01; i < 1.0; i += 0.01) {
	// 	uint32_t val = std::clamp<uint32_t>(i * i * 256.0, 16, 255);
	// 	std::cout << "\x1b[48;2;" << val << "
	// }
}
