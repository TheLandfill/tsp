#include "rng.hpp"

RNG::RNG(uint32_t seed, uint32_t min, uint32_t max) :
	rng(seed),
	dist{min, max}
{}

uint32_t RNG::operator()() {
	return dist(rng);
}
