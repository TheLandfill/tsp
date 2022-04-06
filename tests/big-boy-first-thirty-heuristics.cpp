#include "greedy.hpp"
#include "backtracking.hpp"
#include "dijkstra.hpp"
#include <random>
#include <chrono>

void print_path(const std::vector<size_t>& path) {
	std::cout << "[ ";
	for (size_t i = 0; i < path.size() - 1; i++) {
		std::cout << path[i] << ", ";
	}
	std::cout << path.back() << " ]\n";
}

uint64_t run_masked(const Matrix<uint64_t>& masked_greedy_matrix, uint64_t max_length) {
	Backtracker<uint64_t> greed_bt{max_length};
	auto start = std::chrono::high_resolution_clock::now();
	greed_bt.run(masked_greedy_matrix, 0);
	auto duration = std::chrono::high_resolution_clock::now() - start;
	std::cout << "Duration: " << (double)std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << " ms\n";
	std::cout << "Path Length: " << greed_bt.get_shortest_path_length() << "\n";
	std::vector<size_t> shortest_path{greed_bt.get_shortest_path()};
	// print_path(shortest_path);
	return greed_bt.get_shortest_path_length();
}

class RNG {
public:
	RNG(uint32_t seed = 1, uint32_t min = 1, uint32_t max = 100) :
		rng(seed),
		dist{min, max}
	{}
	uint32_t operator()() {
		return dist(rng);
	}
private:
	std::mt19937 rng;
	std::uniform_int_distribution<std::mt19937::result_type> dist;
};

void run_func(const size_t num_nodes) {
	RNG rng{427389};
	Matrix<uint64_t> mat{num_nodes, num_nodes, rng};
	Greedy<uint64_t> greed{};
	Matrix<uint64_t> edge_freq_mat_greedy{num_nodes, num_nodes, [](){ return 0; }};
	uint64_t min_length_so_far = get_max_val_or_inf<uint64_t>() / num_nodes / 2;
	for (size_t i = 0; i < num_nodes; i++) {
		greed.run(mat, i);
		std::vector<size_t> path{greed.get_path()};
		if (i == 0) {
			min_length_so_far = greed.get_path_length();
		}
		// print_path(path);
		for (size_t j = 1; j < path.size(); j++) {
			size_t cur = path[j];
			size_t prev = path[j - 1];
			edge_freq_mat_greedy.at(prev, cur) += 1;
		}
	}
	Matrix<uint64_t> masked_greedy_matrix{mat};
	for (size_t i = 0; i < num_nodes; i++) {
		for (size_t j = 0; j < num_nodes; j++) {
			if (edge_freq_mat_greedy.at(i, j) == 0) {
				masked_greedy_matrix.at(i, j) = get_max_val_or_inf<uint64_t>() / num_nodes / 2;
			}
		}
	}
	// masked_greedy_matrix.write_to_file("masked_greedy_matrix.dat");
	std::cout << "Shortest Length from Greedy: " << min_length_so_far << "\n";
	run_masked(masked_greedy_matrix, min_length_so_far);
}

int main() {
	for (size_t i = 20; i < 40; i++) {
		std::cout << "Num Nodes: " << i << "\n";
		run_func(i);
		std::cout << "________________________________________________________________________________\n";
	}
	return 0;
}
