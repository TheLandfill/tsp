#include "edge.hpp"

bool Edge::operator==(const Edge& e) const noexcept {
	return start == e.start && end == e.end;
}

// Szudzik's function
std::size_t std::hash<Edge>::operator()(const Edge& e) const noexcept {
	const size_t& a = e.start;
	const size_t& b = e.end;
	return a >= b ? a * a + a + b : a + b * b;
}


