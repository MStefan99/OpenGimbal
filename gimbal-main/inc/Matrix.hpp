//
// Created by MStefan99 on 23.7.21.
//

#ifndef FILTERS_MATRIX_HPP
#define FILTERS_MATRIX_HPP

#include <cstdint>
#include <initializer_list>


#ifdef MATRIX_IO

#include <iostream>

#endif


template <class scalar = float, class size_type = unsigned int, size_type h = 1, size_type w = 1>
class Matrix {
public:
	constexpr Matrix() = default;
	constexpr explicit Matrix(scalar* values);
	constexpr Matrix(const std::initializer_list<std::initializer_list<scalar>>& values);
	constexpr Matrix(const Matrix& matrix);
	constexpr Matrix& operator=(const Matrix& matrix);

	static constexpr Matrix identity();

	scalar constexpr* operator[](size_type i);

	const constexpr scalar* operator[](size_type i) const;

	constexpr Matrix<scalar, size_type, w, h> transpose() const;
	constexpr Matrix inverse() const;

	constexpr Matrix operator*(scalar multiplier) const;
	constexpr Matrix& operator*=(scalar multiplier);
	constexpr Matrix operator/(scalar multiplier) const;
	constexpr Matrix& operator/=(scalar divisor);
	constexpr Matrix operator-() const;

	constexpr Matrix operator+(const Matrix& matrix) const;
	constexpr Matrix operator-(const Matrix& matrix) const;
	template <size_type mw>
	constexpr Matrix<scalar, size_type, h, mw> operator*(const Matrix<scalar, size_type, w, mw>& matrix) const;

	constexpr Matrix multiplyComponents(const Matrix& matrix) const;
	constexpr Matrix concat(const Matrix& matrix) const;

	constexpr size_type getWidth() const;
	constexpr size_type getHeight() const;
	constexpr scalar norm() const;

#ifdef MATRIX_IO
	template <class sc, class st, int sh, int sw>
	friend std::ostream& operator<<(std::ostream& out, const Matrix<sc, st, sh, sw>& matrix);
#endif

protected:
	scalar _values[h * w] {};
};

template <class scalar = float, class size_type = unsigned int>
using Vector2 = Matrix<scalar, size_type, 2, 1>;

template <class scalar = float, class size_type = unsigned int>
using Vector3 = Matrix<scalar, size_type, 3, 1>;


template <class scalar, class size_type, size_type h, size_type w>
constexpr Matrix<scalar, size_type, h, w>::Matrix(scalar* values) {
	for (size_type i {0}; i < h * w; ++i) {
		_values[i] = values[i];
	}
}


template <class scalar, class size_type, size_type h, size_type w>
constexpr Matrix<scalar, size_type, h, w>::Matrix(const std::initializer_list<std::initializer_list<scalar>>& values) {
	size_type j = 0;
	for (auto& row: values) {
		size_type i = 0;
		for (auto& e: row) {
			this->operator[](j)[i++] = e;
		}
		++j;
	}
}


template <class scalar, class size_type, size_type h, size_type w>
constexpr Matrix<scalar, size_type, h, w>::Matrix(const Matrix& matrix) {
	for (size_type j {0}; j < h; ++j) {
		for (size_type i {0}; i < w; ++i) {
			this->operator[](j)[i] = matrix[j][i];
		}
	}
}


template <class scalar, class size_type, size_type h, size_type w>
constexpr Matrix<scalar, size_type, h, w>& Matrix<scalar, size_type, h, w>::operator=(const Matrix& matrix) {
	if (this != &matrix) {
		for (size_type j {0}; j < h; ++j) {
			for (size_type i {0}; i < w; ++i) {
				this->operator[](j)[i] = matrix[j][i];
			}
		}
	}
	return *this;
}


template <class scalar, class size_type, size_type h, size_type w>
constexpr Matrix<scalar, size_type, h, w> Matrix<scalar, size_type, h, w>::identity() {
	Matrix<scalar, size_type, h, w> result {};

	for (size_type i {0}; i < h; ++i) {
		result[i][i] = scalar(1);
	}

	return result;
}


template <class scalar, class size_type, size_type h, size_type w>
constexpr scalar* Matrix<scalar, size_type, h, w>::operator[](size_type i) {
	return _values + (i * w);
}


template <class scalar, class size_type, size_type h, size_type w>
constexpr const scalar* Matrix<scalar, size_type, h, w>::operator[](size_type i) const {
	return _values + (i * w);
}


template <class scalar, class size_type, size_type h, size_type w>
constexpr Matrix<scalar, size_type, w, h> Matrix<scalar, size_type, h, w>::transpose() const {
	Matrix<scalar, size_type, w, h> result {};

	for (size_type j {0}; j < h; ++j) {
		for (size_type i {0}; i < w; ++i) {
			result[i][j] = this->operator[](j)[i];
		}
	}
	return result;
}


template <class scalar, class size_type, size_type h, size_type w>
constexpr Matrix<scalar, size_type, h, w> Matrix<scalar, size_type, h, w>::inverse() const {
	Matrix temp {*this};
	auto augmented {Matrix<scalar, size_type, h, h>::identity()};

	// Gaussian elimination
	for (size_type r1 {0}; r1 < w; ++r1) {
		for (size_type r2 {0}; r2 < w; ++r2) {
			if (r1 == r2) {
				continue;
			}

			scalar factor {temp[r2][r1] / temp[r1][r1]};

			for (size_type i {0}; i < w; ++i) {
				temp[r2][i] -= factor * temp[r1][i];
				augmented[r2][i] -= factor * augmented[r1][i];
			}
		}
	}

	// Gaining identity matrix
	for (size_type r {0}; r < w; ++r) {
		scalar factor = scalar(1) / temp[r][r];

		for (size_type i {0}; i < w; ++i) {
			augmented[r][i] *= factor;
		}
	}

	return augmented;
}


template <class scalar, class size_type, size_type h, size_type w>
constexpr Matrix<scalar, size_type, h, w> Matrix<scalar, size_type, h, w>::operator*(scalar multiplier) const {
	Matrix<scalar, size_type, h, w> result {};

	for (size_type j {0}; j < h; ++j) {
		for (size_type i {0}; i < w; ++i) {
			result[j][i] = this->operator[](j)[i] * multiplier;
		}
	}
	return result;
}


template <class scalar, class size_type, size_type h, size_type w>
constexpr Matrix<scalar, size_type, h, w>& Matrix<scalar, size_type, h, w>::operator*=(scalar multiplier) {
	for (size_type j {0}; j < h; ++j) {
		for (size_type i {0}; i < w; ++i) {
			this->operator[](j)[i] *= multiplier;
		}
	}
	return *this;
}


template <class scalar, class size_type, size_type h, size_type w>
constexpr Matrix<scalar, size_type, h, w> Matrix<scalar, size_type, h, w>::operator/(scalar multiplier) const {
	Matrix<scalar, size_type, h, w> result {};

	for (size_type j {0}; j < h; ++j) {
		for (size_type i {0}; i < w; ++i) {
			result[j][i] = this->operator[](j)[i] / multiplier;
		}
	}
	return result;
}


template <class scalar, class size_type, size_type h, size_type w>
constexpr Matrix<scalar, size_type, h, w>& Matrix<scalar, size_type, h, w>::operator/=(scalar divisor) {
	for (size_type j {0}; j < h; ++j) {
		for (size_type i {0}; i < w; ++i) {
			this->operator[](j)[i] /= divisor;
		}
	}
	return *this;
}


template <class scalar, class size_type, size_type h, size_type w>
constexpr Matrix<scalar, size_type, h, w> Matrix<scalar, size_type, h, w>::operator-() const {
	Matrix<scalar, size_type, h, w> result {};

	for (size_type j {0}; j < h; ++j) {
		for (size_type i {0}; i < w; ++i) {
			result[j][i] = -this->operator[](j)[i];
		}
	}
	return result;
}


template <class scalar, class size_type, size_type h, size_type w>
template <size_type mw>
constexpr Matrix<scalar, size_type, h, mw>
Matrix<scalar, size_type, h, w>::operator*(const Matrix<scalar, size_type, w, mw>& matrix) const {
	Matrix<scalar, size_type, h, mw> result {};
	for (size_type j {0}; j < matrix.getWidth(); ++j) {
		for (size_type i {0}; i < h; ++i) {
			scalar sum {0};
			for (size_type k {0}; k < w; ++k) {
				sum += this->operator[](i)[k] * matrix[k][j];
			}
			result[i][j] = sum;
		}
	}
	return result;
}


template <class scalar, class size_type, size_type h, size_type w>
constexpr Matrix<scalar, size_type, h, w> Matrix<scalar, size_type, h, w>::operator+(const Matrix& matrix) const {
	Matrix<scalar, size_type, h, w> result {};

	for (size_type j {0}; j < h; ++j) {
		for (size_type i {0}; i < w; ++i) {
			result[j][i] = this->operator[](j)[i] + matrix[j][i];
		}
	}
	return result;
}


template <class scalar, class size_type, size_type h, size_type w>
constexpr Matrix<scalar, size_type, h, w> Matrix<scalar, size_type, h, w>::operator-(const Matrix& matrix) const {
	Matrix<scalar, size_type, h, w> result {};

	for (size_type j {0}; j < h; ++j) {
		for (size_type i {0}; i < w; ++i) {
			result[j][i] = this->operator[](j)[i] - matrix[j][i];
		}
	}
	return result;
}


template <class scalar, class size_type, size_type h, size_type w>
constexpr Matrix<scalar, size_type, h, w>
Matrix<scalar, size_type, h, w>::multiplyComponents(const Matrix& matrix) const {
	Matrix<scalar, size_type, h, w> result {};

	for (size_type j {0}; j < h; ++j) {
		for (size_type i {0}; i < w; ++i) {
			result[j][i] = this->operator[](j)[i] * matrix[j][i];
		}
	}
	return result;
}


template <class scalar, class size_type, size_type h, size_type w>
constexpr Matrix<scalar, size_type, h, w> Matrix<scalar, size_type, h, w>::concat(const Matrix& matrix) const {
	Matrix<scalar, size_type, h, w + matrix.getWidth()> result {};

	for (size_type j {0}; j < h; ++j) {
		for (size_type i {0}; i < w; ++i) {
			result[j][i] = this->operator[](j)[i];
		}
		for (size_type i {0}; i < matrix.w; ++i) {
			result[j][i + w] = matrix[j][i];
		}
	}
	return result;
}


template <class scalar, class size_type, size_type h, size_type w>
constexpr size_type Matrix<scalar, size_type, h, w>::getWidth() const {
	return w;
}


template <class scalar, class size_type, size_type h, size_type w>
constexpr size_type Matrix<scalar, size_type, h, w>::getHeight() const {
	return h;
}


template <class scalar, class size_type, size_type h, size_type w>
constexpr scalar Matrix<scalar, size_type, h, w>::norm() const {
	scalar norm {0};

	for (size_type j {0}; j < h; ++j) {
		for (size_type i {0}; i < w; ++i) {
			scalar abs = this->operator[](j)[i] > 0 ? this->operator[](j)[i] : -this->operator[](j)[i];
			norm = norm > abs ? norm : abs;
		}
	}

	return norm;
}


#ifdef MATRIX_IO


template <class scalar, class size_type, size_type h, size_type w>
std::ostream& operator<<(std::ostream& out, const Matrix<scalar, size_type, h, w>& matrix) {
	out << matrix.getWidth() << ':' << matrix.getHeight() << std::endl;

	for (size_type j {0}; j < matrix.getHeight(); ++j) {
		for (size_type i {0}; i < matrix.getWidth(); ++i) {
			if (i) {
				out << ", ";
			}
			out << static_cast<double>(matrix[j][i]);
		}
		out << std::endl;
	}

	return out;
}

#endif

#endif //FILTERS_MATRIX_HPP
