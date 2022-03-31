#pragma once
#include <vector>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>

template<typename T>
class Matrix {
public:
	Matrix(const std::filesystem::path& path);

	template<typename Func>
	Matrix(size_t num_rows, size_t num_cols, Func rng);

	void write_to_file(const std::filesystem::path& path) const;

	T at(size_t row, size_t col) const;
	T& at(size_t row, size_t col);

	size_t get_num_rows() const;
	size_t get_num_cols() const;
private:
	size_t num_rows, num_cols;
	std::vector<T> data;
};

template<typename T>
Matrix<T>::Matrix(const std::filesystem::path& path) {
	std::ifstream reader{path};
	reader >> num_rows >> num_cols;
	data.resize(num_rows * num_cols);
	for (size_t row = 0; row < num_rows; row++) {
		for (size_t col = 0; col < num_cols; col++) {
			if(!(reader >> at(row, col))) {
				std::cerr << "Could not read element at (" << row << ", " << col << ")!\n";
				throw std::runtime_error("Error Reading Element!");
			}
		}
	}
}

template<typename T>
template<typename Func>
Matrix<T>::Matrix(size_t nr, size_t nc, Func rng) :
	num_rows(nr),
	num_cols(nc)
{
	data.reserve(nr * nc);
	for (size_t row = 0; row < num_rows; row++) {
		for (size_t col = 0; col < num_cols; col++) {
			data.emplace_back(rng());
		}
	}
}

template<typename T>
void Matrix<T>::write_to_file(const std::filesystem::path& path) const {
	std::ofstream writer{path};
	writer << num_rows << " " << num_cols << "\n";
	for (size_t row = 0; row < num_rows; row++) {
		for (size_t col = 0; col < num_cols - 1; col++) {
			writer << at(row, col) << " ";
		}
		writer << at(row, num_cols - 1) << "\n";
	}
}

template<typename T>
T Matrix<T>::at(size_t row, size_t col) const {
	return data.at(row * num_cols + col);
}

template<typename T>
T& Matrix<T>::at(size_t row, size_t col) {
	return data.at(row * num_cols + col);
}

template<typename T>
size_t Matrix<T>::get_num_rows() const {
	return num_rows;
}

template<typename T>
size_t Matrix<T>::get_num_cols() const {
	return num_cols;
}
