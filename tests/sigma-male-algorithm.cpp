#include "sigma-male-algorithm.hpp"
#include "double-edge-sigma.hpp"
#include "backtracking.hpp"
#include "print-info.hpp"
#include "rng.hpp"
#include "max-or-inf.hpp"
#include "second-sigma.hpp"

int main() {
	std::cout << "\x1b[m";
	RNG rng{42378};
	size_t problem_size = 40;
	size_t start = 0;
	Matrix<uint32_t> mat{problem_size, problem_size, rng};
	mat.write_to_file("mat.dat");

	Global_Greedy<uint32_t> gg;
	gg.run(mat, start);
	std::cout << "Multifragment Length: " << get_path_length(gg.get_path(), mat) << "\n";

	size_t num_iterations = problem_size * 20;

	Sigma_Male<uint32_t> sm{rng()};
	sm.grind(mat, start, num_iterations, false);
	std::cout << "Ran " << num_iterations << " iterations of Multifragment Sigma without k-opt.\n";
	std::cout << "Found path of length " << sm.get_min_path_length() << ".\n";
	sm.get_freq_mat().write_to_file("sigma-male-mat.dat");
	const Matrix<double>& sm_freq_mat = sm.get_freq_mat();
	print_probability_matrix(sm_freq_mat);
	std::cout << "--------------------------------------------------------------------------------\n";

	Sigma_Male<uint32_t> sm_kopt{rng()};
	sm_kopt.grind(mat, start, num_iterations);
	std::cout << "Ran " << num_iterations << " iterations of Multifragment Sigma with k-opt.\n";
	std::cout << "Found path of length " << sm_kopt.get_min_path_length() << ".\n";
	Matrix<double> sm_kopt_freq_mat = sm_kopt.get_freq_mat();
	print_probability_matrix(sm_kopt_freq_mat);
	std::cout << "--------------------------------------------------------------------------------\n";

	Double_Edge_Sigma<uint32_t> de{rng()};
	de.grind(mat, start, num_iterations, false);
	std::cout << "Ran " << num_iterations << " iterations of DENN Sigma without k-opt.\n";
	std::cout << "Found path of length " << de.get_min_path_length() << ".\n";
	Matrix<double> de_freq_mat = de.get_freq_mat();
	print_probability_matrix(de_freq_mat);
	std::cout << "--------------------------------------------------------------------------------\n";

	Double_Edge_Sigma<uint32_t> de_kopt{rng()};
	de_kopt.grind(mat, start, num_iterations);
	std::cout << "Ran " << num_iterations << " iterations of DENN Sigma with k-opt.\n";
	std::cout << "Found path of length " << de_kopt.get_min_path_length() << ".\n";
	Matrix<double> de_kopt_freq_mat = de_kopt.get_freq_mat();
	print_probability_matrix(de_kopt_freq_mat);
	std::cout << "--------------------------------------------------------------------------------\n";

	Matrix<double> mixed_freq_mat_multi = de_kopt_freq_mat;
	for (size_t row = 0; row < mixed_freq_mat_multi.get_num_rows(); row++) {
		for (size_t col = 0; col < mixed_freq_mat_multi.get_num_cols(); col++) {
			mixed_freq_mat_multi.at(row, col) *= 1.0 / 3.0;
			mixed_freq_mat_multi.at(row, col) += sm_kopt_freq_mat.at(row, col) / 3.0;
			mixed_freq_mat_multi.at(row, col) += sm_freq_mat.at(row, col) / 3.0;
		}
	}
	std::cout << "Mixed Probability Matrix\n";
	print_probability_matrix(mixed_freq_mat_multi);
	std::cout << "--------------------------------------------------------------------------------\n";

	Second_Sigma sec_sig_denn{52, de_kopt.get_min_path_length()};
	sec_sig_denn.grind(mat, de_kopt_freq_mat, start, num_iterations * 5);
	std::cout << "Ran " << num_iterations * 5 << " iterations of second sigma on DENN.\n";
	std::cout << "Found path of length " << sec_sig_denn.get_min_path_length() << " on iteration " << sec_sig_denn.get_iter() << ".\n";
	print_probability_matrix(de_kopt_freq_mat);
	std::cout << "--------------------------------------------------------------------------------\n";

	Second_Sigma sec_sig_multi{52, sm_kopt.get_min_path_length()};
	sec_sig_multi.grind(mat, sm_kopt_freq_mat, start, num_iterations * 5);
	std::cout << "Ran " << num_iterations * 5 << " iterations of second sigma on Multifragment.\n";
	std::cout << "Found path of length " << sec_sig_multi.get_min_path_length() << " on iteration " << sec_sig_multi.get_iter() << ".\n";
	print_probability_matrix(sm_kopt_freq_mat);
	std::cout << "--------------------------------------------------------------------------------\n";

	Second_Sigma sec_sig_mixed{52, std::min(sm_kopt.get_min_path_length(), de_kopt.get_min_path_length())};
	sec_sig_mixed.grind(mat, mixed_freq_mat_multi, start, num_iterations * 5);
	std::cout << "Ran " << num_iterations * 5 << " iterations of second sigma on mixture.\n";
	std::cout << "Found path of length " << sec_sig_mixed.get_min_path_length() << " on iteration " << sec_sig_mixed.get_iter() << ".\n";
	print_probability_matrix(mixed_freq_mat_multi);
	std::cout << "--------------------------------------------------------------------------------\n";

	Matrix<double> garbage_mat{mat.get_num_rows(), mat.get_num_cols(), [](){ return 0.1; }};
	Second_Sigma garbage{52, get_max_val_or_inf<uint32_t>()};
	garbage.grind(mat, garbage_mat, start, num_iterations * 10);
	std::cout << "Ran " << num_iterations * 10 << " iterations of second sigma on mixture.\n";
	std::cout << "Found path of length " << garbage.get_min_path_length() << " on iteration " << garbage.get_iter() << ".\n";
	print_probability_matrix(garbage_mat);
	std::cout << "--------------------------------------------------------------------------------\n";
	return 0;
}
