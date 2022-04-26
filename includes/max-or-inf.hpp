#pragma once
#include <limits>

template<typename T>
constexpr T get_max_val_or_inf() {
	if (std::numeric_limits<T>::has_infinity) {
		return std::numeric_limits<T>::infinity();
	} else {
		return std::numeric_limits<T>::max();
	}
}
