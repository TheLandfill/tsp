#pragma once
#include <cstddef>
#include <functional>

struct Edge {
	size_t start, end;
	bool operator==(const Edge& e) const noexcept;
};

template<>
struct std::hash<Edge>
{
	std::size_t operator()(const Edge& e) const noexcept;
};
