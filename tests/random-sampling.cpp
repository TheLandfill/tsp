#include "random-sampling.hpp"
#include <random>

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

int main() {
	RNG rng{138};
	Matrix<uint32_t> gen_mat{22, 22, rng};
	gen_mat.write_to_file("mat.dat");
	Random_Sampler<uint32_t> sampler;
	sampler.sample_paths(gen_mat, 1'000);
	sampler.write_path_lengths_to_file("path_lengths.dat");
}
