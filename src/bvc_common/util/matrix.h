#pragma once

#include <cmath>
#include <array>
#include <vector>
#include <iostream>
#include <iomanip>

#include "vector.h"

//=============================================================================
// matrix_size
//=============================================================================

struct matrix_size : public std::array<size_t, 2>
{
	matrix_size() {}
	matrix_size(size_t in_num_rows, size_t in_num_cols)
	{
		this->operator[](0) = in_num_rows;
		this->operator[](1) = in_num_cols;
	}
	size_t get_num_rows() const { return this->at(0); }
	size_t get_num_columns() const { return this->at(1); }
};

//=============================================================================
// matrix
// Simple data type that allows to represent a 2D matrix in column-major
// order. This class also offers several operators to manipulate the data.
//=============================================================================

template <typename T>
class matrix
{

public:
	//===========================================================================
	// matrix c-tors and d-tors
	//===========================================================================

	matrix(size_t in_num_rows, size_t in_num_cols, T in_value = static_cast<T>(0));
	matrix(size_t in_num_rows, size_t in_num_cols, const std::vector<T>& in_data);
	matrix();
	~matrix();

	//===========================================================================
	// Basic accessors
	//===========================================================================

	size_t		get_num_rows() const;
	size_t		get_num_columns() const;
	matrix_size size() const;
	size_t		num_elements() const;

	//===========================================================================
	// Indexing
	//===========================================================================

	std::array<size_t, 2> subscripts_from_index(size_t in_idx) const;
	size_t				  index_from_subscripts(size_t in_i, size_t in_j) const;

	T&		 operator()(size_t in_i, size_t in_j);
	const T& operator()(size_t in_i, size_t in_j) const;
	T&		 operator()(size_t in_idx);
	const T& operator()(size_t in_idx) const;

	//===========================================================================
	// Accessors
	//===========================================================================

	void set_row(const std::vector<T>& in_row, size_t in_i);
	void set_column(const std::vector<T>& in_column, size_t in_j);
	void set_data(const std::vector<T>& in_data);

	std::vector<T>		  get_row(size_t in_i) const;
	std::vector<T>		  get_column(size_t in_j) const;
	std::vector<T>&		  get_data();
	const std::vector<T>& get_data() const;

	void delete_row(size_t in_i);
	void delete_column(size_t in_j);

	//===========================================================================
	// Utils
	//===========================================================================

	std::vector<T> diag() const;

	void fill(T value);
	void rand(unsigned int in_seed);
	void rand();
	void identity();

	void transpose();
	void reshape(size_t in_num_rows, size_t in_num_cols);

	void round();
	void floor();
	void ceil();
	void clamp(T in_min_value, T in_max_value);

	T max() const;
	T min() const;

	//===========================================================================
	// Binary matrix-matrix operations
	//===========================================================================

	matrix<T> operator*(const matrix<T>& in_rhs) const;
	matrix<T> operator+(matrix<T> in_rhs) const;
	matrix<T> operator-(matrix<T> in_rhs) const;

	matrix<T> element_product(matrix<T> in_rhs) const;
	matrix<T> element_quotient(matrix<T> in_rhs) const;

	//===========================================================================
	// Binary Scalar-matrix operations
	//===========================================================================

	matrix<T> operator+(T in_rhs) const;
	matrix<T> operator-(T in_rhs) const;
	matrix<T> operator*(T in_rhs) const;
	matrix<T> operator/(T in_rhs) const;
	matrix<T> operator^(T in_rhs) const;

	//===========================================================================
	// Unary operations
	//===========================================================================

	matrix<T> operator-() const;

private:
	//===========================================================================
	// Member variables
	//===========================================================================

	size_t		   num_rows;
	size_t		   num_cols;
	std::vector<T> data;

}; // class matrix<T>

//=============================================================================
// Overloading operator<< for ostream operations
//=============================================================================

// Overloaded operator<< for matrix<T>
template <typename T>
inline std::ostream& operator<<(std::ostream& out, const matrix<T>& matrix)
{
	for (size_t i = 0, endI = matrix.get_num_rows(); i < endI; i++)
	{
		out << matrix.get_row(i) << std::endl;
	}
	return out;
}
