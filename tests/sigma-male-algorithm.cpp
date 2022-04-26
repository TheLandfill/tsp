#include "sigma-male-algorithm.hpp"
#include "backtracking.hpp"
#include "print-accuracy-mat.hpp"
#include "print_path.hpp"
#include "rng.hpp"
#include "max-or-inf.hpp"

int main() {
	RNG rng{138};
	size_t problem_size = 30;
	size_t start = 0;
	Matrix<uint32_t> mat{problem_size, problem_size, rng};
	mat.write_to_file("mat.dat");
	Global_Greedy<uint32_t> gg;
	gg.run(mat, start);
	std::cout << "Multifragment Length: " << get_path_length(gg.get_path(), mat) << "\n";
	Sigma_Male<uint32_t> sm{rng()};
	size_t num_iterations = problem_size * problem_size * 2;
	sm.run(mat, start, num_iterations, false);
	std::cout << "Ran " << num_iterations << " iterations of sigma male algorithm.\n";
	Sigma_Male<uint32_t> sm_kopt{rng()};
	sm_kopt.run(mat, start, num_iterations);
	std::cout << "Ran " << num_iterations << " iterations of sigma male algorithm.\n";
	std::cout << "Found path of length " << sm_kopt.get_min_path_length() << ".\n";
	sm.get_freq_mat().write_to_file("sigma-male-mat.dat");
	const Matrix<double>& sm_freq_mat = sm.get_freq_mat();
	Matrix<double> sm_kopt_freq_mat = sm_kopt.get_freq_mat();
	for (size_t row = 0; row < sm_kopt_freq_mat.get_num_rows(); row++) {
		double total = 0.0;
		for (size_t col = 0; col < sm_kopt_freq_mat.get_num_cols(); col++) {
			total += sm_kopt_freq_mat.at(row, col);
		}
		for (size_t col = 0; col < sm_kopt_freq_mat.get_num_cols(); col++) {
			sm_kopt_freq_mat.at(row, col) /= total;
		}
	}
	Backtracker<double> bt{static_cast<double>(sm_kopt.get_min_path_length())};
	Matrix<double> masked_mat{problem_size, problem_size, [](){ return get_max_val_or_inf<double>(); }};
	for (size_t row = 0; row < masked_mat.get_num_rows(); row++) {
		for (size_t col = 0; col < masked_mat.get_num_cols(); col++) {
			if (sm_kopt_freq_mat.at(row, col) >= 0.001) {
				masked_mat.at(row, col) = mat.at(row, col);
			}
		}
	}
	masked_mat.write_to_file("masked-mat.dat");
	bt.run(masked_mat, start);
	const auto& actual_path = bt.get_shortest_path();
	print_path(actual_path);
	print_accuracy_matrix(actual_path, sm_freq_mat);
	std::cout << "\n--------------------------------------------------------------------------------\n";
	print_accuracy_matrix(actual_path, sm_kopt_freq_mat);
	return 0;
}
