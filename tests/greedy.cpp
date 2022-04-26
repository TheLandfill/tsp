#include "greedy.hpp"
#include "rng.hpp"

int main() {
	RNG rng{138};
	Matrix<uint32_t> mat{22, 22, rng};
	mat.write_to_file("mat.dat");
	Greedy<uint32_t> greed{};
	greed.run(mat, 0);
	std::cout << "Path Length: " << greed.get_path_length() << "\nPath: [ ";
	std::vector<size_t> path{greed.get_path()};
	for (size_t i = 0; i < path.size() - 1; i++) {
		std::cout << path[i] << ", ";
	}
	std::cout << path.back() << " ]\n";
	uint32_t length = 0;
	std::cout << "Lengths: [ ";
	for (size_t i = 0; i < path.size() - 2; i++) {
		const auto& cur = path.at(i);
		const auto& next = path.at(i + 1);
		std::cout << mat.at(cur, next) << ", ";
		length += mat.at(cur, next);
	}
	const auto& cur = path[path.size() - 2];
	const auto& next = path[path.size() - 1];
	uint32_t temp_length = mat.at(cur, next);
	std::cout << temp_length << " ]\n";
	length += temp_length;
	std::cout << "Total Length: " << length << "\n";
}
