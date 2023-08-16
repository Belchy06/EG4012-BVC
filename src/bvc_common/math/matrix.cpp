#include <cassert>
#include <cmath>
#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <random>

#include "bvc_common/math/matrix.h"

using namespace bvc_vector;

//=============================================================================
// matrix c-tors and d-tor
//=============================================================================

template <typename T>
matrix<T>::matrix(size_t in_num_rows, size_t in_num_cols, T in_value)
{
	num_rows = in_num_rows;
	num_cols = in_num_cols;
	data.resize(num_rows * num_cols, in_value);
}

template <typename T>
matrix<T>::matrix(size_t in_num_rows, size_t in_num_cols, const std::vector<T>& in_data)
{
	if (in_num_rows * in_num_cols != in_data.size())
	{
		throw std::invalid_argument("matrix::matrix");
	}
	num_rows = in_num_rows;
	num_cols = in_num_cols;
	data = in_data;
}

template <typename T>
matrix<T>::matrix() {}

template <typename T>
matrix<T>::~matrix() {}

//=============================================================================
// Basic accessors
//=============================================================================

template <typename T>
size_t matrix<T>::get_num_rows() const
{
	return num_rows;
}

template <typename T>
size_t matrix<T>::get_num_columns() const
{
	return num_cols;
}

template <typename T>
matrix_size matrix<T>::size() const
{
	if (num_rows * num_cols != data.size())
	{
		throw std::logic_error("matrix::size");
	}
	// clang++ suggests braces around initialization of subobject
	return matrix_size(num_rows, num_cols);
}

template <typename T>
size_t matrix<T>::num_elements() const
{
	if (num_rows * num_cols != data.size())
	{
		throw std::logic_error("matrix::num_elements");
	}
	return data.size();
}

//=============================================================================
// Indexing
//=============================================================================

template <typename T>
std::array<size_t, 2> matrix<T>::subscripts_from_index(size_t in_idx) const
{
	if (in_idx >= data.size())
	{
		throw std::out_of_range("matrix::subscripts_from_index");
	}
	return { { in_idx % num_rows, in_idx / num_rows } };
}

template <typename T>
size_t matrix<T>::index_from_subscripts(size_t in_i, size_t in_j) const
{
	if (in_i >= num_rows || in_j >= num_cols)
	{
		throw std::out_of_range("matrix::index_from_subscripts");
	}
	return in_j * num_rows + in_i;
}

template <typename T>
T& matrix<T>::operator()(size_t in_i, size_t in_j)
{
	const size_t idx = index_from_subscripts(in_i, in_j);
	return operator()(idx);
}

template <typename T>
const T& matrix<T>::operator()(size_t in_i, size_t in_j) const
{
	const size_t idx = index_from_subscripts(in_i, in_j);
	return operator()(idx);
}

template <typename T>
T& matrix<T>::operator()(size_t in_idx)
{
	if (in_idx >= num_elements())
	{
		throw std::out_of_range("matrix::operator()");
	}
	return data[in_idx];
}

template <typename T>
const T& matrix<T>::operator()(size_t in_idx) const
{
	if (in_idx >= num_elements())
	{
		throw std::out_of_range("matrix::operator()");
	}
	return data.at(in_idx);
}

//=============================================================================
// Accessors
//=============================================================================

template <typename T>
void matrix<T>::set_row(const std::vector<T>& in_row, size_t in_i)
{
	if (in_i >= num_rows || num_cols != in_row.size())
	{
		throw std::invalid_argument("matrix::set_row");
	}
	for (size_t j = 0; j < num_cols; j++)
	{
		operator()(in_i, j) = in_row.at(j);
	}
}

template <typename T>
void matrix<T>::set_column(const std::vector<T>& in_column, size_t in_j)
{
	if (in_j >= num_cols || num_rows != in_column.size())
	{
		throw std::invalid_argument("matrix::set_column");
	}
	const ptrdiff_t idx = static_cast<ptrdiff_t>(index_from_subscripts(0, in_j));
	std::copy(in_column.begin(), in_column.end(), data.begin() + idx);
}

template <typename T>
void matrix<T>::set_data(const std::vector<T>& in_data)
{
	if (num_elements() != in_data.size())
	{
		throw std::invalid_argument("matrix::set_data");
	}
	data = in_data;
}

template <typename T>
std::vector<T> matrix<T>::get_row(size_t in_i) const
{
	std::vector<T> row;
	row.reserve(num_cols);
	for (size_t j = 0; j < num_cols; j++)
	{
		const T& value = operator()(in_i, j);
		row.push_back(value);
	}
	return row;
}

template <typename T>
std::vector<T> matrix<T>::get_column(size_t in_j) const
{
	const ptrdiff_t idx_start = static_cast<ptrdiff_t>(index_from_subscripts(0, in_j));
	const ptrdiff_t idx_end = idx_start + static_cast<ptrdiff_t>(num_rows);
	return std::vector<T>(data.begin() + idx_start, data.begin() + idx_end);
}

template <typename T>
std::vector<T>& matrix<T>::get_data()
{
	return data;
}

template <typename T>
const std::vector<T>& matrix<T>::get_data() const
{
	return data;
}

template <typename T>
void matrix<T>::delete_row(size_t in_i)
{
	for (size_t j = 0; j < num_cols; j++)
	{
		const ptrdiff_t idx = static_cast<ptrdiff_t>(index_from_subscripts(in_i, num_cols - j - 1));
		data.erase(data.begin() + idx);
	}
	num_rows--;
	assert(num_elements() == data.size());
}

template <typename T>
void matrix<T>::delete_column(size_t in_j)
{
	const ptrdiff_t idx_start = static_cast<ptrdiff_t>(index_from_subscripts(0, in_j));
	const ptrdiff_t idx_end = static_cast<ptrdiff_t>(index_from_subscripts(0, in_j + 1));
	data.erase(data.begin() + idx_start, data.begin() + idx_end);
	num_cols--;
	assert(num_elements() == data.size());
}

//=============================================================================
// Utils
//=============================================================================

template <typename T>
std::vector<T> matrix<T>::diag() const
{
	std::vector<T> d;
	const size_t   diag_length = std::min(num_rows, num_cols);
	d.reserve(diag_length);
	for (size_t k = 0; k < diag_length; k++)
	{
		const T& value = operator()(k, k);
		d.push_back(value);
	}
	return d;
}

template <typename T>
void matrix<T>::fill(T in_value)
{
	std::fill(data.begin(), data.end(), in_value);
}

template <typename T>
void matrix<T>::rand(unsigned int in_seed)
{
	std::mt19937 gen;
	gen.seed(in_seed);
	std::uniform_real_distribution<T> dis(0, 1);
	std::generate(data.begin(), data.end(), [&dis, &gen]() {
		return dis(gen);
	});
}

template <typename T>
void matrix<T>::rand()
{
	this->rand(std::random_device()());
}

template <typename T>
void matrix<T>::identity()
{
	if (num_rows != num_cols)
	{
		throw std::logic_error("matrix::identity");
	}
	for (size_t j = 0; j < num_cols; j++)
	{
		for (size_t i = 0; i < num_rows; i++)
		{
			operator()(i, j) = (i == j) ? static_cast<T>(1) : static_cast<T>(0);
		}
	}
}

template <typename T>
void matrix<T>::transpose()
{
	// TODO: inplace tranposition
	std::vector<T> transposed_data;
	transposed_data.reserve(num_rows * num_cols);
	for (size_t i = 0; i < num_rows; i++)
	{
		for (size_t j = 0; j < num_cols; j++)
		{
			const T& value = operator()(i, j);
			transposed_data.push_back(value);
		}
	}
	data = transposed_data;
	std::swap(num_rows, num_cols);
	assert(num_elements() == data.size());
}

template <typename T>
void matrix<T>::reshape(size_t in_num_rows, size_t in_num_cols)
{
	if (in_num_rows * in_num_cols != num_elements())
	{
		throw std::invalid_argument("matrix::reshape");
	}
	num_rows = in_num_rows;
	num_cols = in_num_cols;
	assert(num_elements() == data.size());
}

template <typename T>
void matrix<T>::round()
{
	bvc_vector::round(data);
}

template <typename T>
void matrix<T>::floor()
{
	bvc_vector::floor(data);
}

template <typename T>
void matrix<T>::ceil()
{
	bvc_vector::ceil(data);
}

template <typename T>
void matrix<T>::clamp(T min_value, T max_value)
{
	bvc_vector::clamp(data, min_value, max_value);
}

template <typename T>
T matrix<T>::max() const
{
	return bvc_vector::max(data);
}

template <typename T>
T matrix<T>::min() const
{
	return bvc_vector::min(data);
}

//=============================================================================
// Binary matrix-matrix operations
//=============================================================================

template <typename T>
matrix<T> matrix<T>::operator*(const matrix<T>& in_rhs) const
{
	if (in_rhs.get_num_rows() != num_cols)
	{
		throw std::invalid_argument("matrix::operator*");
	}
	matrix<T> res(num_rows, in_rhs.get_num_columns(), 0.0);
	for (size_t i = 0; i < num_rows; i++)
	{
		for (size_t j = 0; j < in_rhs.get_num_columns(); j++)
		{
			T& value = res(i, j);
			for (size_t k = 0; k < num_cols; k++)
			{
				value += operator()(i, k) * in_rhs(k, j);
			}
		}
	}
	return res;
}

template <typename T>
matrix<T> matrix<T>::operator+(matrix<T> in_rhs) const
{
	if (size() != in_rhs.size())
	{
		throw std::invalid_argument("matrix::operator+");
	}
	in_rhs.get_data() += data;
	return in_rhs;
}

template <typename T>
matrix<T> matrix<T>::operator-(matrix<T> in_rhs) const
{
	if (size() != in_rhs.size())
	{
		throw std::invalid_argument("matrix::operator-");
	}
	in_rhs.get_data() -= data;
	return in_rhs;
}

template <typename T>
matrix<T> matrix<T>::element_product(matrix<T> in_rhs) const
{
	if (size() != in_rhs.size())
	{
		throw std::invalid_argument("matrix::element_product");
	}
	in_rhs.get_data() *= data;
	return in_rhs;
}

template <typename T>
matrix<T> matrix<T>::element_quotient(matrix<T> in_rhs) const
{
	if (size() != in_rhs.size())
	{
		throw std::invalid_argument("matrix::element_quotient");
	}
	in_rhs.get_data() /= data;
	return in_rhs;
}

//=============================================================================
// Binary Scalar-matrix operations
//=============================================================================

template <typename T>
matrix<T> matrix<T>::operator+(T in_rhs) const
{
	return matrix<T>(num_rows, num_cols, data + in_rhs);
}

template <typename T>
matrix<T> matrix<T>::operator-(T in_rhs) const
{
	return matrix<T>(num_rows, num_cols, data - in_rhs);
}

template <typename T>
matrix<T> matrix<T>::operator*(T in_rhs) const
{
	return matrix<T>(num_rows, num_cols, data * in_rhs);
}

template <typename T>
matrix<T> matrix<T>::operator/(T in_rhs) const
{
	return matrix<T>(num_rows, num_cols, data / in_rhs);
}

template <typename T>
matrix<T> matrix<T>::operator^(T in_rhs) const
{
	return matrix<T>(num_rows, num_cols, data ^ in_rhs);
}

//=============================================================================
// Unary operations
//=============================================================================

template <typename T>
matrix<T> matrix<T>::operator-() const
{
	return matrix<T>(num_rows, num_cols, -data);
}

// Explicit template instantiation
template class matrix<double>;
