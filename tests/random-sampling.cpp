#include "random-sampling.hpp"
#include "rng.hpp"

int main() {
	RNG rng{138};
	Matrix<uint32_t> gen_mat{22, 22, rng};
	gen_mat.write_to_file("mat.dat");
	Random_Sampler<uint32_t> sampler;
	sampler.sample_paths(gen_mat, 1'000);
	sampler.write_path_lengths_to_file("path_lengths.dat");
}
