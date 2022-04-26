#include "dijkstra.hpp"
#include "greedy.hpp"
#include "global-greedy.hpp"
#include "backtracking.hpp"
#include "rng.hpp"
#include "print_path.hpp"
#include "print-accuracy-mat.hpp"
#include "sigma-male-algorithm.hpp"
// Actually make a timer class
#include <chrono>

uint32_t run_masked(const Matrix<uint32_t>& masked_greedy_matrix) {
	Backtracker<uint32_t> greed_bt{3000};
	auto start = std::chrono::high_resolution_clock::now();
	greed_bt.run(masked_greedy_matrix, 0);
	auto duration = std::chrono::high_resolution_clock::now() - start;
	std::cout << "Duration: " << (double)std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << " ms\n";
	std::cout << "Path Length: " << greed_bt.get_shortest_path_length() << "\n";
	std::vector<size_t> shortest_path{greed_bt.get_shortest_path()};
	print_path(shortest_path);
	return greed_bt.get_shortest_path_length();
}

int main() {
	const size_t num_nodes = 22;
	RNG rng{4312789};
	Matrix<uint32_t> mat{num_nodes, num_nodes, rng};
	mat.write_to_file("mat.dat");
	Greedy<uint32_t> greed{};
	Matrix<double> edge_freq_mat_greedy{num_nodes, num_nodes, [](){ return 0; }};
	for (size_t i = 0; i < num_nodes; i++) {
		greed.run(mat, i);
		std::vector<size_t> path{greed.get_path()};
		double path_length = get_path_length(path, mat);
		// print_path(path);
		for (size_t j = 1; j < path.size(); j++) {
			size_t cur = path[j];
			size_t prev = path[j - 1];
			edge_freq_mat_greedy.at(prev, cur) += 1.0 / path_length;
		}
	}
	Global_Greedy<uint32_t> ggreed{};
	ggreed.run(mat, 0);
	std::vector<size_t> gg_path{ggreed.get_path()};
	std::cout << "Global Greedy Path: ";
	print_path(gg_path);
	double path_length = get_path_length(gg_path, mat);
	for (size_t i = 1; i < gg_path.size(); i++) {
		size_t cur = gg_path[i];
		size_t prev = gg_path[i - 1];
		edge_freq_mat_greedy.at(prev, cur) += 1.0 / path_length;
	}
	edge_freq_mat_greedy.write_to_file("edge_freq_mat_greedy.dat");
	Matrix<uint32_t> masked_greedy_matrix{mat};
	for (size_t i = 0; i < num_nodes; i++) {
		for (size_t j = 0; j < num_nodes; j++) {
			if (edge_freq_mat_greedy.at(i, j) == 0) {
				masked_greedy_matrix.at(i, j) = get_max_val_or_inf<uint32_t>() / num_nodes / 2;
			}
		}
	}
	masked_greedy_matrix.write_to_file("masked_greedy_matrix.dat");
	uint32_t potential_shortest_length = run_masked(masked_greedy_matrix);
	Matrix<double> edge_freq_mat_dijkstra{num_nodes, num_nodes, [](){ return 0; }};
	Dijkstra<uint32_t> dijkstra;
	for (size_t start = 0; start < num_nodes; start++) {
		dijkstra.run(mat, start);
		auto edges = dijkstra.get_edges();
		for (const auto& edge : edges) {
			edge_freq_mat_dijkstra.at(edge.start, edge.end) += 1;
		}
	}
	Matrix<double> edge_freq_mat_random{num_nodes, num_nodes, [](){ return 0; }};
	std::vector<size_t> nodes_for_random;
	nodes_for_random.reserve(num_nodes);
	for (size_t i = 0; i < num_nodes; i++) {
		nodes_for_random.push_back(i);
	}
	std::default_random_engine engine;
	uint32_t seed = time(NULL);
	engine.seed(seed);
	for (size_t i = 0; i < num_nodes; i++) {
		std::shuffle(nodes_for_random.begin(), nodes_for_random.end(), engine);
		double path_length = get_path_length(nodes_for_random, mat);
		for (size_t j = 1; j < nodes_for_random.size(); j++) {
			size_t cur = nodes_for_random[j];
			size_t prev = nodes_for_random[j - 1];
			edge_freq_mat_random.at(prev, cur) += 1.0 / path_length;
		}
	}
	// Found using backtracking
	// std::vector<size_t> shortest_path{0, 10, 18, 19, 17, 20, 5, 13, 11, 6, 4, 16, 7, 3, 14, 8, 1, 12, 9, 21, 15, 2};
	Backtracker<uint32_t> bt{potential_shortest_length};
	bt.run(mat, 0);
	std::vector<size_t> shortest_path{bt.get_shortest_path()};
	size_t length_of_shortest_path = bt.get_shortest_path_length();
	std::cout << "Length of Shortest Path: " << length_of_shortest_path << "\n";
	print_path(shortest_path);
	std::cout << "Accuracy Matrix of Greedy\n";
	print_accuracy_matrix(shortest_path, edge_freq_mat_greedy);
	std::cout << "Accuracy Matrix of Dijkstra\n";
	print_accuracy_matrix(shortest_path, edge_freq_mat_dijkstra);
	std::cout << "Accuracy Matrix of Random\n";
	print_accuracy_matrix(shortest_path, edge_freq_mat_random);
	uint32_t greedy_false_negatives = 0;
	uint32_t greedy_true_positives = 0;
	uint32_t dijkstra_false_negatives = 0;
	uint32_t dijkstra_true_positives = 0;
	uint32_t random_false_negatives = 0;
	uint32_t random_true_positives = 0;
	for (size_t i = 0; i < shortest_path.size() - 1; i++) {
		const size_t& start = shortest_path[i];
		const size_t& end = shortest_path[i + 1];
		std::cout << "(" << start << ", " << end << "): " << edge_freq_mat_greedy.at(start, end) << "\t" << edge_freq_mat_dijkstra.at(start, end) << "\n";
		greedy_false_negatives += (edge_freq_mat_greedy.at(start, end) == 0);
		greedy_true_positives += (edge_freq_mat_greedy.at(start, end) != 0);
		dijkstra_false_negatives += (edge_freq_mat_dijkstra.at(start, end) == 0);
		dijkstra_true_positives += (edge_freq_mat_dijkstra.at(start, end) != 0);
		random_false_negatives += (edge_freq_mat_random.at(start, end) == 0);
		random_true_positives += (edge_freq_mat_random.at(start, end) != 0);
	}
	uint32_t greedy_false_positives = 0;
	uint32_t greedy_true_negatives = 0;
	uint32_t dijkstra_false_positives = 0;
	uint32_t dijkstra_true_negatives = 0;
	uint32_t random_false_positives = 0;
	uint32_t random_true_negatives = 0;
	size_t next_element = (size_t)-1;
	for (size_t i = 0; i < shortest_path.size() - 1; i++) {
		size_t cur_element = shortest_path[i];
		next_element = shortest_path[i + 1];
		for (size_t j = 0; j < mat.get_num_cols(); j++) {
			if (j == next_element) {
				continue;
			}
			greedy_false_positives += (edge_freq_mat_greedy.at(cur_element, j) != 0);
			greedy_true_negatives += (edge_freq_mat_greedy.at(cur_element, j) == 0);
			dijkstra_false_positives += edge_freq_mat_dijkstra.at(cur_element, j) != 0;
			dijkstra_true_negatives += (edge_freq_mat_dijkstra.at(cur_element, j) == 0);
			random_false_positives += (edge_freq_mat_random.at(cur_element, j) != 0);
			random_true_negatives += (edge_freq_mat_random.at(cur_element, j) == 0);
		}
	}
	std::cout << "Greedy True Positives: "    << (double)greedy_true_positives   /(double)(num_nodes) << "\n";
	std::cout << "Greedy False Positives: "   << (double)greedy_false_positives  /(double)(num_nodes) << "\n";
	std::cout << "Greedy True Negatives: "    << (double)greedy_true_negatives   /(double)(num_nodes) << "\n";
	std::cout << "Greedy False Negatives: "   << (double)greedy_false_negatives  /(double)(num_nodes) << "\n";
	std::cout << "Dijkstra True Positives: "  << (double)dijkstra_true_positives /(double)num_nodes << "\n";
	std::cout << "Dijkstra False Positives: " << (double)dijkstra_false_positives/(double)num_nodes << "\n";
	std::cout << "Dijkstra True Negatives: "  << (double)dijkstra_true_negatives /(double)num_nodes << "\n";
	std::cout << "Dijkstra False Negatives: " << (double)dijkstra_false_negatives/(double)num_nodes << "\n";
	std::cout << "Random True Positives: "    << (double)random_true_positives   /(double)num_nodes << "\n";
	std::cout << "Random False Positives: "   << (double)random_false_positives  /(double)num_nodes << "\n";
	std::cout << "Random True Negatives: "    << (double)random_true_negatives   /(double)num_nodes << "\n";
	std::cout << "Random False Negatives: "   << (double)random_false_negatives  /(double)num_nodes << "\n";
}
