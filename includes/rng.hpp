#pragma once
#include <cstdint>
#include <random>

class RNG {
public:
	RNG(uint32_t seed = 1, uint32_t min = 1, uint32_t max = 100);
	uint32_t operator()();
private:
	std::mt19937 rng;
	std::uniform_int_distribution<std::mt19937::result_type> dist;
};
