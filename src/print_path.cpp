#include "print_path.hpp"
#include <iostream>

void print_path(const std::vector<size_t>& path) {
	std::cout << "[ ";
	for (size_t i = 0; i < path.size() - 1; i++) {
		std::cout << path[i] << ", ";
	}
	std::cout << path.back() << " ]\n";
}
