#include "backtracking.hpp"
#include "random-sampling.hpp"
#include "rng.hpp"
#include <random>
// Actually make a timer class
#include <chrono>

int main() {
	RNG rng{138};
	Matrix<uint32_t> mat{22, 22, rng};
	mat.write_to_file("mat.dat");
	Random_Sampler<uint32_t> sampler;
	sampler.sample_paths(mat, 1'000);
	sampler.write_path_lengths_to_file("path_lengths.dat");
	Backtracker<uint32_t> bt{180};
	auto start = std::chrono::high_resolution_clock::now();
	bt.run(mat, 0);
	auto duration = std::chrono::high_resolution_clock::now() - start;
	std::cout << "Duration: " << (double)std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << " ms\n";
	std::cout << "Path Length: " << bt.get_shortest_path_length() << "\nPath: [ ";
	std::vector<size_t> shortest_path{bt.get_shortest_path()};
	for (size_t i = 0; i < shortest_path.size() - 1; i++) {
		std::cout << shortest_path[i] << ", ";
	}
	std::cout << shortest_path.back() << " ]\n";
	uint32_t length = 0;
	std::cout << "Lengths: [ ";
	for (size_t i = 0; i < shortest_path.size() - 2; i++) {
		const auto& cur = shortest_path.at(i);
		const auto& next = shortest_path.at(i + 1);
		std::cout << mat.at(cur, next) << ", ";
		length += mat.at(cur, next);
	}
	const auto& cur = shortest_path[shortest_path.size() - 2];
	const auto& next = shortest_path[shortest_path.size() - 1];
	uint32_t temp_length = mat.at(cur, next);
	std::cout << temp_length << " ]\n";
	length += temp_length;
	std::cout << "Total Length: " << length << "\n";
}
