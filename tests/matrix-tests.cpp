#include "matrix.hpp"
#include <random>

class NRNG {
public:
	uint32_t operator()() {
		seed += 1;
		return seed;
	}
private:
	uint32_t seed = 0;
};

class NRNGf {
public:
	float operator()() {
		seed += M_PI;
		if (seed > 25.0f) {
			seed -= 25.0f;
		}
		return seed;
	}
private:
	float seed = 0.0f;
};

int main() {
	NRNG nrng;
	Matrix<uint32_t> gen_mat{20, 20, nrng};
	gen_mat.write_to_file("mat.dat");
	for (size_t i = 0; i < gen_mat.get_num_rows(); i++) {
		for (size_t j = 0; j < gen_mat.get_num_cols() - 1; j++) {
			std::cout << gen_mat.at(i, j) << " ";
		}
		std::cout << gen_mat.at(i, gen_mat.get_num_cols() - 1) << "\n";
	}
	Matrix<uint32_t> read_mat{"mat.dat"};
	for (size_t i = 0; i < read_mat.get_num_rows(); i++) {
		for (size_t j = 0; j < read_mat.get_num_cols() - 1; j++) {
			std::cout << read_mat.at(i, j) << " ";
		}
		std::cout << read_mat.at(i, read_mat.get_num_cols() - 1) << "\n";
	}
	read_mat.write_to_file("mat2.dat");
	NRNGf nrngf;
	Matrix<float> float_mat{20, 20, nrngf};
	for (size_t i = 0; i < float_mat.get_num_rows(); i++) {
		for (size_t j = 0; j < float_mat.get_num_cols() - 1; j++) {
			std::cout << float_mat.at(i, j) << " ";
		}
		std::cout << float_mat.at(i, float_mat.get_num_cols() - 1) << "\n";
	}
	float_mat.write_to_file("float_mat.dat");
	return 0;
}
