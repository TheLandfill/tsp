#include "print-info.hpp"
#include <algorithm>

void print_probability_matrix(
	const Matrix<double>& predicted_freq
) {
	double total_hhi = 0.0;
	for (size_t row = 0; row < predicted_freq.get_num_rows(); row++) {
		double total = 0.0;
		double max_value = 0.0;
		for (size_t col = 0; col < predicted_freq.get_num_cols(); col++) {
			total += predicted_freq.at(row, col);
		}
		double hhi = 0.0;
		for (size_t col = 0; col < predicted_freq.get_num_cols(); col++) {
			double value = predicted_freq.at(row, col) / total;
			hhi += value * value;
			if (value > max_value) {
				max_value = value;
			}
			uint32_t val = 0;
			if (value > 0.0001) {
				val = std::clamp<uint32_t>(value * 256.0, 16, 255);
			}
			std::cout << "\x1b[48;2;0;" << val << ";" << val << "m ";
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
			<< max_value
			<< "\n";
		total_hhi += hhi;
	}
	std::cout << "Total hhi: " << std::setprecision(4) << total_hhi << "\n";
}
