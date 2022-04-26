#include "dijkstra.hpp"
#include "rng.hpp"
#include <random>

void dijkstra_runner(const Matrix<uint32_t>& mat);

int main() {
	RNG rng{138};
	Matrix<uint32_t> mat{22, 22, rng};
	mat.write_to_file("mat.dat");
	dijkstra_runner(mat);
	Matrix<uint32_t> mat2{"known_mat.csv"};
	dijkstra_runner(mat2);
}

void dijkstra_runner(const Matrix<uint32_t>& mat) {
	Dijkstra<uint32_t> dijkstra;
	size_t start = 0;
	dijkstra.run(mat, start);
	auto edges = dijkstra.get_edges();
	for (const auto& edge : edges) {
		std::cout << edge.start << " " << edge.end << "\n";
	}
	for (size_t i = 0; i < mat.get_num_cols(); i++) {
		uint32_t length = 0;
		std::vector<size_t> path{dijkstra.get_shortest_path_to(i)};
		std::cout << "Path from " << start << " to " << i << ".\n\t[ ";
		size_t prev_node = (size_t)-1;
		for (const auto& cur_node : path) {
			std::cout << cur_node << ", ";
			if (prev_node == (size_t)-1) {
				prev_node = cur_node;
				continue;
			}
			length += mat.at(prev_node, cur_node);
			prev_node = cur_node;
		}
		std::cout << (char)8 << (char)8 << " ]\n\tTotal Length: " << length << "\n";
	}
}
