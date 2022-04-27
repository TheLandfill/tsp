#include "double-edge-nearest-neighbor.hpp"
#include "rng.hpp"
#include "print-info.hpp"

int main() {
	RNG rng{138};
	size_t problem_size = 22;
	Matrix<uint32_t> mat{problem_size, problem_size, rng};
	mat.write_to_file("mat.dat");
	size_t start = 0;
	Double_Edge_Nearest_Neighbor<uint32_t> denn{mat, start};
	for (size_t i = 0; i < problem_size; i++) {
		std::cout << "Starting from: " << i << "\n";
		const std::vector<size_t>& path = denn.get_path();
		denn.run(i);
		std::cout << "Path: ";
		print_path(denn.get_path());
		std::cout << "Lengths: [ ";
		uint32_t length = 0;
		for (size_t j = 0; j < path.size() - 2; j++) {
			const auto& cur = path.at(j);
			const auto& next = path.at(j + 1);
			std::cout << mat.at(cur, next) << ", ";
			length += mat.at(cur, next);
		}
		const auto& cur = path.at(path.size() - 2);
		const auto& next = path.at(path.size() - 1);
		uint32_t temp_length = mat.at(cur, next);
		std::cout << temp_length << " ]\n";
		length += temp_length;
		std::cout << "Total Length: " << length << "\n";
	}
}
