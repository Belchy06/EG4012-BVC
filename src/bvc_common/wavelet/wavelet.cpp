#include "bvc_common/math/vector.h"
#include "wavelet.h"

using namespace bvc_vector;

//=============================================================================
// bvc_wavelet_decomposition_1d
//=============================================================================

template <typename T>
bvc_wavelet_decomposition_1d<T>::bvc_wavelet_decomposition_1d(size_t in_num_levels)
{
	// Internal representation is structured as
	// {d1, d2, ... dn, an}
	this->resize(in_num_levels + 1);
}

template <typename T>
bvc_wavelet_decomposition_1d<T>::~bvc_wavelet_decomposition_1d() {}

template <typename T>
size_t bvc_wavelet_decomposition_1d<T>::num_levels() const
{
	return this->size() - 1;
}

template <typename T>
const std::vector<T>& bvc_wavelet_decomposition_1d<T>::get_app_coefficients() const
{
	return this->back();
}

template <typename T>
const std::vector<T>& bvc_wavelet_decomposition_1d<T>::get_det_coefficients(size_t in_level) const
{
	if (in_level >= num_levels())
	{
		throw std::invalid_argument("bvc_wavelet_decomposition_1d::get_det_coefficients");
	}
	return this->at(in_level);
}

template <typename T>
void bvc_wavelet_decomposition_1d<T>::set_det_coefficients(const std::vector<T>& in_d, size_t in_level)
{
	if (in_level >= num_levels())
	{
		throw std::invalid_argument("bvc_wavelet_decomposition_1d::set_det_coefficients");
	}
	this->operator[](in_level) = in_d;
}

template <typename T>
void bvc_wavelet_decomposition_1d<T>::set_app_coefficients(const std::vector<T>& in_a)
{
	this->back() = in_a;
}

//=============================================================================
// bvc_wavelet_decomposition_2d
//=============================================================================

template <typename T>
bvc_wavelet_decomposition_2d<T>::bvc_wavelet_decomposition_2d(size_t in_num_levels)
{
	// Internal representation is structured as
	// {d1, v1, h1, d2, v2, h2, ... dn, vn, hn, an}
	this->resize(3 * in_num_levels + 1);
}

template <typename T>
bvc_wavelet_decomposition_2d<T>::~bvc_wavelet_decomposition_2d() {}

template <typename T>
size_t bvc_wavelet_decomposition_2d<T>::num_levels() const
{
	return (this->size() - 1) / 3;
}

template <typename T>
void bvc_wavelet_decomposition_2d<T>::set_app_coefficients(const matrix<T>& in_a)
{
	this->back() = in_a;
}

template <typename T>
const matrix<T>& bvc_wavelet_decomposition_2d<T>::get_app_coefficients() const
{
	return this->back();
}

template <typename T>
const matrix<T>& bvc_wavelet_decomposition_2d<T>::get_det_coefficients(bvc_wavelet_subbdand in_subband, size_t in_n) const
{
	if (in_subband != BVC_WAVELET_SUBBAND_HORIZONTAL && in_subband != BVC_WAVELET_SUBBAND_VERTICAL && in_subband != BVC_WAVELET_SUBBAND_DIAGONAL)
	{
		throw std::invalid_argument("bvc_wavelet_decomposition_2d::get_det_coefficients");
	}
	if (in_n >= num_levels())
	{
		throw std::invalid_argument("bvc_wavelet_decomposition_2d::get_det_coefficients ");
	}
	// Convert subband to int for indexing
	const size_t idx = static_cast<size_t>(in_subband);
	return this->at(3 * in_n + idx);
}

template <typename T>
void bvc_wavelet_decomposition_2d<T>::set_det_coefficients(const matrix<T>& in_d, bvc_wavelet_subbdand in_subband, size_t in_n)
{
	if (in_subband != BVC_WAVELET_SUBBAND_HORIZONTAL && in_subband != BVC_WAVELET_SUBBAND_VERTICAL && in_subband != BVC_WAVELET_SUBBAND_DIAGONAL)
	{
		throw std::invalid_argument("bvc_wavelet_decomposition_2d::set_det_coefficients");
	}
	if (in_n >= num_levels())
	{
		throw std::invalid_argument("bvc_wavelet_decomposition_2d::set_det_coefficients ");
	}
	const size_t idx = static_cast<size_t>(in_subband);
	this->at(3 * in_n + idx) = in_d;
}

template <typename T>
matrix<T> bvc_wavelet_decomposition_2d<T>::get_matrix()
{
	matrix_size size = this->at(0).size();
	matrix<T>	full;
	//(size_level_0.get_num_rows() * 2, size_level_0.get_num_columns() * 2);

	for (int n = num_levels() - 1; n >= 0; n--)
	{
		matrix<T> l = this->back();
		this->pop_back();
		matrix<T> r = this->back();
		this->pop_back();

		full = matrix<T>(l.get_num_rows() * 2, l.get_num_columns() * 2);

		for (size_t row = 0; row < l.get_num_rows(); row++)
		{
			std::vector<T> fr;
			fr.insert(fr.end(), l.get_row(row).begin(), l.get_row(row).end());
			fr.insert(fr.end(), r.get_row(row).begin(), r.get_row(row).end());
			full.set_row(fr, row);
		}

		l = this->back();
		this->pop_back();
		r = this->back();
		this->pop_back();

		for (size_t row = 0; row < l.get_num_rows(); row++)
		{
			std::vector<T> fr;
			fr.insert(fr.end(), l.get_row(row).begin(), l.get_row(row).end());
			fr.insert(fr.end(), r.get_row(row).begin(), r.get_row(row).end());
			full.set_row(fr, row + l.get_num_rows());
		}

		this->push_back(full);
	}
	return this->back();
}

//=============================================================================
// wavelet c-tor and d-tor
//=============================================================================

template <typename T>
wavelet<T>::wavelet(const std::vector<T> in_lo_d, const std::vector<T> in_hi_d, const std::vector<T> in_lo_r, const std::vector<T> in_hi_r)
{
	lo_d = in_lo_d;
	hi_d = in_hi_d;
	lo_r = in_lo_r;
	hi_r = in_hi_r;
	half_length_filter = lo_d.size() / 2;
}

template <typename T>
wavelet<T>::~wavelet()
{
}

//=============================================================================
// 1D Discrete wavelet Transform (DWT)
//=============================================================================

template <typename T>
bvc_wavelet_decomposition_1d<T> wavelet<T>::decompose(const std::vector<T>& in_x, size_t in_num_levels) const
{
	if (in_x.size() < (1 << in_num_levels))
	{
		throw std::invalid_argument("wavelet::decompose");
	}
	bvc_wavelet_decomposition_1d<T> decomposition(in_num_levels);
	std::vector<T>					a = in_x;
	for (size_t k = 0; k < in_num_levels; k++)
	{
		std::vector<T> ak, dk;
		discrete_wavelet_transform(a, ak, dk);
		// Save approximation coefficients for next iteration
		a = ak;
		// Store detail coefficients of current level k to Decomposition object
		decomposition.set_det_coefficients(dk, k);
	}
	// Store final approximation coefficients
	decomposition.set_app_coefficients(a);
	return decomposition;
}

template <typename T>
std::vector<T> wavelet<T>::reconstruct(const bvc_wavelet_decomposition_1d<T>& in_decomposition, size_t in_length_rec) const
{
	return app_coefficients(in_decomposition, 0, in_length_rec);
}

template <typename T>
void wavelet<T>::discrete_wavelet_transform(const std::vector<T>& in_x, std::vector<T>& out_a, std::vector<T>& out_d) const
{
	out_a = conv_down(in_x, lo_d);
	out_d = conv_down(in_x, hi_d);
}

template <typename T>
std::vector<T> wavelet<T>::inverse_discrete_wavelet_transform(const std::vector<T>& in_a, const std::vector<T>& in_d, size_t in_l) const
{
	return up_conv(in_a, lo_r, in_l) + up_conv(in_d, hi_r, in_l);
}

//=============================================================================
// 2D Discrete wavelet Transform (DWT)
//=============================================================================

template <typename T>
bvc_wavelet_decomposition_2d<T> wavelet<T>::decompose(const matrix<T>& in_x, size_t in_num_levels) const
{
	if (in_x.get_num_rows() < (1 << in_num_levels) || in_x.get_num_columns() < (1 << in_num_levels))
	{
		throw std::invalid_argument("wavelet::decompose");
	}
	bvc_wavelet_decomposition_2d<T> decomposition(in_num_levels);
	matrix<T>						a = in_x;
	for (size_t k = 0; k < in_num_levels; k++)
	{
		matrix<T> ak, hk, vk, dk;
		discrete_wavelet_transform(a, ak, hk, vk, dk);
		// Save approximation coefficients for next iteration
		a = ak;
		// Store subbands obtained at current level k to Decomposition object
		decomposition.set_det_coefficients(dk, BVC_WAVELET_SUBBAND_DIAGONAL, k);
		decomposition.set_det_coefficients(vk, BVC_WAVELET_SUBBAND_VERTICAL, k);
		decomposition.set_det_coefficients(hk, BVC_WAVELET_SUBBAND_HORIZONTAL, k);
	}
	// Store final approximation
	decomposition.set_app_coefficients(a);
	return decomposition;
}

template <typename T>
matrix<T> wavelet<T>::reconstruct(const bvc_wavelet_decomposition_2d<T>& decomposition, const matrix_size& size_rec) const
{
	// Call app_coefficients with maximum level of reconstruction (0)
	return app_coefficients(decomposition, 0, size_rec);
}

template <typename T>
void wavelet<T>::discrete_wavelet_transform(const matrix<T>& x, matrix<T>& a, matrix<T>& h, matrix<T>& v, matrix<T>& d) const
{
	conv_down(x, lo_d, a, h);
	conv_down(x, hi_d, v, d);
}

template <typename T>
matrix<T> wavelet<T>::inverse_discrete_wavelet_transform(const matrix<T>& a, const matrix<T>& h, const matrix<T>& v,
	const matrix<T>& d, const matrix_size& s) const
{
	// Return sum of upsampled subbands
	return up_conv(a, lo_r, lo_r, s) + up_conv(h, hi_r, lo_r, s) + up_conv(v, lo_r, hi_r, s) + up_conv(d, hi_r, hi_r, s);
}

//=============================================================================
// Utils
//=============================================================================

template <typename T>
std::vector<T> wavelet<T>::lin_rec(const std::vector<T>& a, size_t num_levels) const
{
	bvc_wavelet_decomposition_1d<T> c(num_levels);
	const size_t					length = a.size();
	for (size_t k = 0; k < num_levels; k++)
	{
		// Set k-th level detail coefficients to zero
		std::vector<T> dk(length * (1 << (num_levels - k - 1)), static_cast<T>(0));
		c.set_det_coefficients(dk, k);
	}
	// Set approx coefficients equal to the given input
	c.set_app_coefficients(a);
	// Call app_coefficients with length of reconstruction equal to N*2^n
	return app_coefficients(c, 0, length * (1 << num_levels));
}

template <typename T>
matrix<T> wavelet<T>::lin_rec(const matrix<T>& a, size_t num_levels) const
{
	bvc_wavelet_decomposition_2d<T> c(num_levels);
	const size_t					num_rows = a.get_num_rows();
	const size_t					num_cols = a.get_num_columns();
	for (size_t k = 0; k < num_levels; k++)
	{
		// Set all subbands at k-th level to zero
		matrix<T> tmp(num_rows * (1 << (num_levels - k - 1)), num_cols * (1 << (num_levels - k - 1)), static_cast<T>(0));
		c.set_det_coefficients(tmp, BVC_WAVELET_SUBBAND_DIAGONAL, k);
		c.set_det_coefficients(tmp, BVC_WAVELET_SUBBAND_VERTICAL, k);
		c.set_det_coefficients(tmp, BVC_WAVELET_SUBBAND_HORIZONTAL, k);
	}
	// Set approx coefficients equal to given input
	c.set_app_coefficients(a);
	// Call app_coefficients with size_rec equal to to M*2^n-by-N*2^n
	const matrix_size size_rec(num_rows * (1 << num_levels), num_cols * (1 << num_levels));
	return app_coefficients(c, 0, size_rec);
}

//===========================================================================
// Utils for 1D DWT
//===========================================================================

template <typename T>
std::vector<T> wavelet<T>::app_coefficients(const bvc_wavelet_decomposition_1d<T>& decomposition, size_t level) const
{
	if (level >= decomposition.num_levels())
	{
		throw std::invalid_argument("wavelet::app_coefficients");
	}
	const size_t length_rec = (level == 0) ? 2 * decomposition.get_det_coefficients(0).size() : decomposition.get_det_coefficients(level - 1).size();
	return app_coefficients(decomposition, level, length_rec);
}

template <typename T>
std::vector<T> wavelet<T>::app_coefficients(const bvc_wavelet_decomposition_1d<T>& decomposition, size_t level, size_t length_rec) const
{
	std::vector<T> a = decomposition.get_app_coefficients();
	for (size_t k = decomposition.num_levels(); k != level; k--)
	{
		// Get size from next upper level, if current level is the final then
		// resort to the final lenght argument sz
		const size_t length_rec_k = k != 1 ? decomposition.get_det_coefficients(k - 2).size() : length_rec;
		// Reconstruct signal of size s from kth level approx and detail
		a = inverse_discrete_wavelet_transform(a, decomposition.get_det_coefficients(k - 1), length_rec_k);
	}
	return a;
}

template <typename T>
std::vector<T> wavelet<T>::conv_down(const std::vector<T>& x, const std::vector<T>& f) const
{
	const size_t   last = static_cast<size_t>(std::ceil(static_cast<double>(x.size()) / 2.0) * 2.0);
	std::vector<T> z = w_extend(x, half_length_filter);
	z = conv(z, f, BVC_WAVELET_CONVOLUTION_VALID);
	std::vector<T> y(last / 2);
	for (size_t k = 1; k <= last; k += 2)
	{
		// Since k is always an odd integer, k/2 === floor(k/2)
		y[k / 2] = z[k];
	}
	return y;
}

template <typename T>
std::vector<T> wavelet<T>::up_conv(const std::vector<T>& x, const std::vector<T>& f, size_t s) const
{
	const size_t   lf = f.size();
	std::vector<T> y = dyadup(x);
	y = w_extend(y, lf / 2);
	y = conv(y, f, BVC_WAVELET_CONVOLUTION_FULL);
	y.erase(y.begin(), y.begin() + lf - 1);
	y.erase(y.begin() + s, y.end());
	return y;
}

//===========================================================================
// Utils for 2D DWT
//===========================================================================

template <typename T>
matrix<T> wavelet<T>::app_coefficients(const bvc_wavelet_decomposition_2d<T>& decomposition, size_t level) const
{
	if (level >= decomposition.num_levels())
	{
		throw std::invalid_argument("wavelet::Appcoef2");
	}
	matrix_size size_rec;
	if (level == 0)
	{
		// Level zero contains the biggest size of the decomposition, so we set the
		// reconstruction size equal to twice the size at level zero.
		const matrix_size size_level_0 = decomposition.get_det_coefficients(BVC_WAVELET_SUBBAND_HORIZONTAL, 0).size();
		size_rec = matrix_size(size_level_0.get_num_rows() * 2, size_level_0.get_num_columns() * 2);
	}
	else
	{
		// If level is not zero, get size of (any) subband at the next highest
		// level. We take here the horizontal subband.
		size_rec = decomposition.get_det_coefficients(BVC_WAVELET_SUBBAND_HORIZONTAL, level - 1).size();
	}
	return app_coefficients(decomposition, level, size_rec);
}

template <typename T>
matrix<T> wavelet<T>::app_coefficients(const bvc_wavelet_decomposition_2d<T>& decomposition, size_t level,
	const matrix_size& size_rec) const
{
	matrix<T> a = decomposition.get_app_coefficients();
	for (size_t k = decomposition.num_levels(); k != level; k--)
	{
		// Select size of reconstruction from any subband at the next highest level
		// We arbitrarily use the horizontal subband. If there is no next highest
		// level, then we use the size_rec argument.
		const matrix_size s = (k != 1) ? decomposition.get_det_coefficients(BVC_WAVELET_SUBBAND_HORIZONTAL, k - 2).size() : size_rec;
		const matrix<T>&  hk = decomposition.get_det_coefficients(BVC_WAVELET_SUBBAND_HORIZONTAL, k - 1);
		const matrix<T>&  vk = decomposition.get_det_coefficients(BVC_WAVELET_SUBBAND_VERTICAL, k - 1);
		const matrix<T>&  dk = decomposition.get_det_coefficients(BVC_WAVELET_SUBBAND_DIAGONAL, k - 1);
		a = inverse_discrete_wavelet_transform(a, hk, vk, dk, s);
	}
	return a;
}

template <typename T>
void wavelet<T>::conv_down(const matrix<T>& x, const std::vector<T>& f, matrix<T>& xL, matrix<T>& xH) const
{
	// Compute size of subbands
	const size_t num_rows_subband = static_cast<size_t>(std::ceil(static_cast<double>(x.get_num_rows()) / 2.0));
	const size_t num_cols_subband = static_cast<size_t>(std::ceil(static_cast<double>(x.get_num_columns()) / 2.0));
	// Convolve and decimate each row with the chosen filter
	matrix<T> tmp(x.get_num_rows(), num_cols_subband);
	for (size_t i = 0; i < x.get_num_rows(); i++)
	{
		const std::vector<T> row = conv_down(x.get_row(i), f);
		tmp.set_row(row, i);
	}
	// Compute high-pass subband of pre-filtered tmp matrix
	xH = matrix<T>(num_rows_subband, num_cols_subband);
	for (size_t j = 0; j < tmp.get_num_columns(); j++)
	{
		const std::vector<T> column = conv_down(tmp.get_column(j), hi_d);
		xH.set_column(column, j);
	}
	// Compute low-pass subband of pre-filtered tmp matrix
	xL = matrix<T>(num_rows_subband, num_cols_subband);
	for (size_t j = 0; j < tmp.get_num_columns(); j++)
	{
		const std::vector<T> column = conv_down(tmp.get_column(j), lo_d);
		xL.set_column(column, j);
	}
}

template <typename T>
matrix<T> wavelet<T>::up_conv(const matrix<T>& x, const std::vector<T>& f1, const std::vector<T>& f2, const matrix_size& s) const
{
	// Upsample each column
	matrix<T> tmp(s[0], x.get_num_columns());
	for (size_t j = 0; j < x.get_num_columns(); j++)
	{
		tmp.set_column(up_conv(x.get_column(j), f1, s.get_num_rows()), j);
	}
	// Upsample each row of tmp matrix
	matrix<T> y(s.get_num_rows(), s.get_num_columns());
	for (size_t i = 0; i < y.get_num_rows(); i++)
	{
		y.set_row(up_conv(tmp.get_row(i), f2, s.get_num_columns()), i);
	}
	return y;
}

//=============================================================================
// Utils
//=============================================================================

template <typename T>
std::vector<T> wavelet<T>::dyadup(const std::vector<T>& x) const
{
	// Return a vector having double length
	std::vector<T> y;
	y.reserve(2 * x.size());
	// The output vector interleaves the elements of x with zero
	for (size_t k = 0; k < x.size(); k++)
	{
		y.push_back(x[k]);
		y.push_back(0);
	}
	return y;
}

template <typename T>
std::vector<T> wavelet<T>::w_extend(const std::vector<T>& x, size_t lenEXT) const
{
	std::vector<T> temp = x;
	// Add extra sample if signal is odd
	if (x.size() % 2 == 1)
	{
		temp.push_back(temp.back());
	}
	// Handle cases when x is shorter than lenEXT
	const size_t   rep = lenEXT / temp.size(); // (size_t)std::floor(lenEXT/temp.size());
	const size_t   len = lenEXT % temp.size();
	std::vector<T> y;
	y.reserve(2 * (len + rep * temp.size()) + temp.size());
	// Copy last len elements at the beginning
	y.insert(y.begin(), temp.end() - len, temp.end());
	for (size_t k = 0; k < 2 * rep + 1; k++)
	{
		y.insert(y.end(), temp.begin(), temp.end());
	}
	// Copy first len elements at the end
	y.insert(y.end(), temp.begin(), temp.begin() + len);
	return y;
}

// Explicit template instantiation
template class bvc_wavelet_decomposition_1d<double>;
template class bvc_wavelet_decomposition_2d<double>;
template class wavelet<double>;
