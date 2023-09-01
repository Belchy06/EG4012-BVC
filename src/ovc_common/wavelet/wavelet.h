#pragma once

#include <array>
#include <stdexcept>
#include <vector>
#include <string>

#include "ovc_common/math/matrix.h"
#include "ovc_common/wavelet/wavelet_config.h"

namespace ovc
{
	typedef enum
	{
		WAVELET_FAMILY_SKIP,
		WAVELET_FAMILY_BIORTHOGONAL,
		WAVELET_FAMILY_COIFLETS,
		WAVELET_FAMILY_DAUBECHIES,
		WAVELET_FAMILY_HAAR,
		WAVELET_FAMILY_REVERSE_BIORTHOGONAL,
		WAVELET_FAMILY_SYMLETS
	} wavelet_family;

	inline std::string wavelet_family_to_string(wavelet_family in_wavelet_family)
	{
		switch (in_wavelet_family)
		{
			case WAVELET_FAMILY_SKIP:
				return "WAVELET_FAMILY_SKIP";
			case WAVELET_FAMILY_BIORTHOGONAL:
				return "WAVELET_FAMILY_BIORTHOGONAL";
			case WAVELET_FAMILY_COIFLETS:
				return "WAVELET_FAMILY_COIFLETS";
			case WAVELET_FAMILY_DAUBECHIES:
				return "WAVELET_FAMILY_DAUBECHIES";
			case WAVELET_FAMILY_HAAR:
				return "WAVELET_FAMILY_HAAR";
			case WAVELET_FAMILY_REVERSE_BIORTHOGONAL:
				return "WAVELET_FAMILY_REVERSE_BIORTHOGONAL";
			case WAVELET_FAMILY_SYMLETS:
				return "WAVELET_FAMILY_SYMLETS";
			default:
				return "";
		}
	}

	typedef enum
	{
		WAVELET_SUBBAND_DIAGONAL,
		WAVELET_SUBBAND_VERTICAL,
		WAVELET_SUBBAND_HORIZONTAL
	} wavelet_subbdand;

	template <typename T>
	class wavelet_decomposition_1d : public std::vector<std::vector<T>>
	{
	public:
		explicit wavelet_decomposition_1d(size_t in_num_levels);
		~wavelet_decomposition_1d();

		size_t num_levels() const;

		const std::vector<T>& get_app_coefficients() const;
		const std::vector<T>& get_det_coefficients(size_t in_level) const;

		void set_det_coefficients(const std::vector<T>& in_d, size_t in_level);
		void set_app_coefficients(const std::vector<T>& in_a);
	};

	template <typename T>
	class wavelet_decomposition_2d : public std::vector<matrix<T>>
	{
	public:
		explicit wavelet_decomposition_2d(size_t in_num_levels);
		~wavelet_decomposition_2d();

		size_t num_levels() const;

		const matrix<T>&				   get_app_coefficients() const;
		const matrix<T>&				   get_det_coefficients(wavelet_subbdand in_subband, size_t in_level) const;
		matrix<T>						   to_matrix();
		static wavelet_decomposition_2d<T> from_matrix(matrix<T> in_x, size_t in_num_levels)
		{
			wavelet_decomposition_2d<T> decomposition(in_num_levels);
			matrix<T>					a = in_x;
			for (size_t k = 0; k < in_num_levels; k++)
			{
				// Compute size of subbands
				const size_t num_rows_subband = static_cast<size_t>(std::ceil(static_cast<double>(a.get_num_rows()) / 2.0));
				const size_t num_cols_subband = static_cast<size_t>(std::ceil(static_cast<double>(a.get_num_columns()) / 2.0));

				matrix<T> ak, hk, vk, dk;
				ak = matrix<T>(num_rows_subband, num_cols_subband);
				hk = matrix<T>(num_rows_subband, num_cols_subband);
				vk = matrix<T>(num_rows_subband, num_cols_subband);
				dk = matrix<T>(num_rows_subband, num_cols_subband);

				for (size_t x = 0; x < num_cols_subband; x++)
				{
					for (size_t y = 0; y < num_rows_subband; y++)
					{
						ak(y, x) = a(y, x);
					}
				}

				for (size_t x = 0; x < num_cols_subband; x++)
				{
					for (size_t y = 0; y < num_rows_subband; y++)
					{
						hk(y, x) = a(y, x + num_cols_subband);
					}
				}

				for (size_t x = 0; x < num_cols_subband; x++)
				{
					for (size_t y = 0; y < num_rows_subband; y++)
					{
						vk(y, x) = a(y + num_rows_subband, x);
					}
				}

				for (size_t x = 0; x < num_cols_subband; x++)
				{
					for (size_t y = 0; y < num_rows_subband; y++)
					{
						dk(y, x) = a(y + num_rows_subband, x + num_cols_subband);
					}
				}
				// Save approximation coefficients for next iteration
				a = ak;
				// Store subbands obtained at current level k to Decomposition object
				decomposition.set_det_coefficients(dk, WAVELET_SUBBAND_DIAGONAL, k);
				decomposition.set_det_coefficients(vk, WAVELET_SUBBAND_VERTICAL, k);
				decomposition.set_det_coefficients(hk, WAVELET_SUBBAND_HORIZONTAL, k);
			}
			// Store final approximation
			decomposition.set_app_coefficients(a);
			return decomposition;
		}

		void set_det_coefficients(const matrix<T>& in_d, wavelet_subbdand in_subband, size_t in_level);
		void set_app_coefficients(const matrix<T>& in_a);
	};

	//=============================================================================
	// wavelet
	// Implementation of 1D and 2D single-level and multi-level wavelet transform.
	//=============================================================================

	template <typename T>
	class wavelet
	{
	public:
		// Constructor requires the decomposition/reconstruction filters
		explicit wavelet(const std::vector<T> in_lo_d, const std::vector<T> in_hi_d, const std::vector<T> in_lo_r, const std::vector<T> in_hi_r);
		~wavelet();

	public:
		//===========================================================================
		// 1D Discrete wavelet Transform (DWT)
		//===========================================================================

		// Multi-level wavelet decomposition of 1D input vector x
		wavelet_decomposition_1d<T> decompose(const std::vector<T>& in_x, size_t in_num_levels) const;

		// Multi-level wavelet reconstruction of 1D vector having length_rec
		std::vector<T> reconstruct(const wavelet_decomposition_1d<T>& in_decomposition, size_t in_length_rec) const;

		// Single-level DWT transform of 1D vector x. The output vectors contain the
		// approximation coefficients a and the detail coefficients d
		void discrete_wavelet_transform(const std::vector<T>& in_x, std::vector<T>& out_a, std::vector<T>& out_d) const;

		// Single-level inverse DWT transform which computes a 1D vector of length l
		// from the given approximation and detail coefficients
		std::vector<T> inverse_discrete_wavelet_transform(const std::vector<T>& in_a, const std::vector<T>& in_d, size_t in_l) const;

		//===========================================================================
		// 2D Discrete wavelet Transform (DWT)
		//===========================================================================

		// Multi-level wavelet decomposition of 2D input matrix x
		wavelet_decomposition_2d<T> decompose(const matrix<T>& in_x, size_t in_num_levels) const;

		// Multi-level wavelet reconstruction of 2D matrix having size_rec
		matrix<T> reconstruct(const wavelet_decomposition_2d<T>& in_decomposition, const matrix_size& in_size_rec) const;

		// Single-level DWT transform of 2D matrix x. The output matrices contain the
		// approximation coefficients a, the horizontal subband h, the vertical
		// subband v, and the diagonal subband d
		void discrete_wavelet_transform(const matrix<T>& in_x, matrix<T>& out_a, matrix<T>& out_h, matrix<T>& out_v, matrix<T>& out_d) const;

		// Single-level inverse DWT transform which computes a 2D matrix of size s
		// from the given approximation and subband coefficients
		matrix<T> inverse_discrete_wavelet_transform(const matrix<T>& in_a, const matrix<T>& in_h, const matrix<T>& in_v, const matrix<T>& in_d, const matrix_size& in_s) const;

		//===========================================================================
		// Utils
		//===========================================================================

		// Linearly reconstruct 1D vector assuming the given input a represents the
		// approximation coefficients of a multi-level wavelet decomposition with
		// num_levels. Thus, if vector a has length l, the output will have length
		// equal to l*(2^num_levels)
		std::vector<T> lin_rec(const std::vector<T>& a, size_t num_levels) const;

		// Same as above, but this time the reconstruction is performed on 2D matrix
		matrix<T> lin_rec(const matrix<T>& a, size_t num_levels) const;

	private:
		//===========================================================================
		// Utils for 1D DWT
		//===========================================================================

		// Reconstruct 1D approximation of a vector at given level n from the input
		// decomposition. Using level=0 means full reconstruction, level = 1 means
		// reconstruction at the second-most coarse level, and so on. The length of
		// the reconstructed vector is inferred by the input wavelet_decomposition_1d and it
		// is equal to the length of the detail coefficients at the next higher
		// level. If level=0 (and thus there is no higher level), the length will be
		// defined as double the length of level zero.
		std::vector<T> app_coefficients(const wavelet_decomposition_1d<T>& decomposition, size_t level) const;

		// Reconstruct the vector approximation at given level and specify the length
		// of the reconstruction using the third argument
		std::vector<T> app_coefficients(const wavelet_decomposition_1d<T>& decomposition, size_t level, size_t length_rec) const;

		// Convolution and decimation
		std::vector<T> conv_down(const std::vector<T>& x, const std::vector<T>& f) const;

		// Upsampling with zero-padding, dyadup, and convolution
		std::vector<T> up_conv(const std::vector<T>& x, const std::vector<T>& f, size_t s) const;

		//===========================================================================
		// Utils for 2D DWT
		//===========================================================================

		// Analogous to the 1D case, but here a 2D matrix will be reconstructed
		matrix<T> app_coefficients(const wavelet_decomposition_2d<T>& decomposition, size_t level) const;

		// As above, but with the extra argument specifying the size of the matrix
		matrix<T> app_coefficients(const wavelet_decomposition_2d<T>& decomposition, size_t level, const matrix_size& size_rec) const;

		// Convolution and decimation
		void conv_down(const matrix<T>& x, const std::vector<T>& f, matrix<T>& xL, matrix<T>& xH) const;

		// Upsampling with zero-padding and 2D separable convolution, applying f1
		// along the columns and f2 along the rows
		matrix<T> up_conv(const matrix<T>& x, const std::vector<T>& f1, const std::vector<T>& f2, const matrix_size& s) const;

		//===========================================================================
		// Utils
		//===========================================================================

		// Double the size of x interleaving zeros as  {x1, 0, x2, 0, x3, ... xn, 0}
		std::vector<T> dyadup(const std::vector<T>& x) const;

		// Periodized extension of x
		std::vector<T> w_extend(const std::vector<T>& x, size_t lenEXT) const;

	private:
		//===========================================================================
		// Member variables
		//===========================================================================

		std::vector<T> lo_d; // Decomposition low-pass filter
		std::vector<T> hi_d; // Decomposition high-pass filter
		std::vector<T> lo_r; // Reconstruction low-pass filter
		std::vector<T> hi_r; // Reconstruction high-pass filter

		size_t half_length_filter;
	};
} // namespace ovc