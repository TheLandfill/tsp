#include "sigma-male-algorithm.hpp"
#include "backtracking.hpp"
#include "print-accuracy-mat.hpp"
#include "print_path.hpp"
#include "rng.hpp"

int main() {
	RNG rng{138};
	size_t problem_size = 30;
	Matrix<uint32_t> mat{problem_size, problem_size, rng};
	mat.write_to_file("mat.dat");
	Sigma_Male<uint32_t> sm{rng()};
	size_t num_iterations = 256;
	sm.run(mat, 0, num_iterations, false);
	std::cout << "Ran " << num_iterations << " iterations of sigma male algorithm.\n";
	Sigma_Male<uint32_t> sm_kopt{rng()};
	sm_kopt.run(mat, 0, num_iterations);
	std::cout << "Ran " << num_iterations << " iterations of sigma male algorithm.\n";
	std::cout << "Found path of length " << sm_kopt.get_min_path_length() << ".\n";
	Backtracker<uint32_t> bt{sm_kopt.get_min_path_length()};
	bt.run(mat, 0);
	sm.get_freq_mat().write_to_file("sigma-male-mat.dat");
	const auto& sm_freq_mat = sm.get_freq_mat();
	const auto& sm_kopt_freq_mat = sm_kopt.get_freq_mat();
	const auto& actual_path = bt.get_shortest_path();
	print_path(actual_path);
	print_accuracy_matrix(actual_path, sm_freq_mat, 256.0 / static_cast<double>(num_iterations));
	std::cout << "\n--------------------------------------------------------------------------------\n";
	print_accuracy_matrix(actual_path, sm_kopt_freq_mat, 256.0 / static_cast<double>(num_iterations));
	return 0;
}
