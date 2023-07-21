//
// Created by MStefan99 on 23.7.21.
//

#ifndef TRADER_MATRIX_H
#define TRADER_MATRIX_H

#include <cstdint>
#include <initializer_list>

#include "lib/inc/tl/allocator.hpp"


#ifdef MATRIX_IO

#include <iostream>

#endif


template<class scalar = float, class size_type = unsigned int>
class Matrix {
public:
	using allocator_type = tl::allocator<scalar>;

	Matrix() = default;
	explicit Matrix(size_type w, size_type h);
	explicit Matrix(size_type w, size_type h, scalar* values);
	Matrix(const std::initializer_list<std::initializer_list<scalar>>& values);

	Matrix(const Matrix& matrix);
	Matrix& operator=(const Matrix& matrix);

	~Matrix();

	static Matrix identity(size_type order);

	scalar* operator[](size_type i);
	const scalar* operator[](size_type i) const;

	Matrix transpose() const;
	Matrix invert() const;

	Matrix operator*(scalar multiplier) const;
	Matrix& operator*=(scalar divisor);

	Matrix operator/(scalar multiplier) const;
	Matrix& operator/=(scalar divisor);

	Matrix operator+(const Matrix& matrix) const;
	Matrix operator-(const Matrix& matrix) const;
	Matrix operator*(const Matrix& matrix) const;

    Matrix multiplyChannels(const Matrix& matrix) const;
	Matrix multiplyComponents(const Matrix& matrix) const;
	Matrix concat(const Matrix& matrix) const;

	size_type getWidth() const;
	size_type getHeight() const;

#ifdef MATRIX_IO
	template<class sc, class st>
	friend std::ostream& operator<<(std::ostream& out, const Matrix<sc, st>& matrix);
#endif

protected:
	size_type _h {0};
	size_type _w {0};
	scalar* _values {};
};

template<class scalar, class size_type>
Matrix<scalar, size_type>::Matrix(size_type w, size_type h):
		_h {h}, _w {w} {
	_values = allocator_type().allocate(_h * _w);

	for (size_type i {0}; i < _h * _w; ++i) {
		_values[i] = scalar(0);
	}
}

template<class scalar, class size_type>
Matrix<scalar, size_type>::Matrix(size_type w, size_type h, scalar* values):
		_h {h}, _w {w}, _values(values) {
	for (size_type i {0}; i < _h * _w; ++i) {
		_values[i] = scalar(0);
	}
}


template<class scalar, class size_type>
Matrix<scalar, size_type>::Matrix(const std::initializer_list<std::initializer_list<scalar>>& values):
		_h {static_cast<size_type>(values.size())}, _w {static_cast<size_type>(values.begin()->size())} {
	_values = allocator_type().allocate(_h * _w);

	size_type j = 0;
	for (auto& row: values) {
		size_type i = 0;
		for (auto& e: row) {
			this->operator[](j)[i++] = e;
		}
		++j;
	}
}


template<class scalar, class size_type>
Matrix<scalar, size_type>::Matrix(const Matrix& matrix): Matrix(matrix._w, matrix._h) {
	for (size_type j {0}; j < _h; ++j) {
		for (size_type i {0}; i < _w; ++i) {
			this->operator[](j)[i] = matrix[j][i];
		}
	}
}


template<class scalar, class size_type>
Matrix<scalar, size_type>& Matrix<scalar, size_type>::operator=(const Matrix& matrix) {
	if (this != &matrix) {
		allocator_type().deallocate(_values, _h * _w);

		_h = matrix._h;
		_w = matrix._w;
		_values = allocator_type().allocate(_h * _w);

		for (size_type j {0}; j < _h; ++j) {
			for (size_type i {0}; i < _w; ++i) {
				this->operator[](j)[i] = matrix[j][i];
			}
		}
	}
	return *this;
}


template<class scalar, class size_type>
Matrix<scalar, size_type>::~Matrix() {
	allocator_type().deallocate(reinterpret_cast<scalar*>(_values), _w * _h);
}


template<class scalar, class size_type>
Matrix<scalar, size_type> Matrix<scalar, size_type>::identity(size_type order) {
	Matrix result {order, order};

	for (size_type i {0}; i < order; ++i) {
		result[i][i] = scalar(1);
	}

	return result;
}


template<class scalar, class size_type>
scalar* Matrix<scalar, size_type>::operator[](size_type i) {
	return _values + (i * _w);
}


template<class scalar, class size_type>
const scalar* Matrix<scalar, size_type>::operator[](size_type i) const {
	return _values + (i * _w);
}


template<class scalar, class size_type>
Matrix<scalar, size_type> Matrix<scalar, size_type>::transpose() const {
	Matrix result {_h, _w};

	for (size_type j {0}; j < _h; ++j) {
		for (size_type i {0}; i < _w; ++i) {
			result[i][j] = this->operator[](j)[i];
		}
	}
	return result;
}


template<class scalar, class size_type>
Matrix<scalar, size_type> Matrix<scalar, size_type>::invert() const {
	if (_w != _h) {
		return *this;
	}

	Matrix temp {*this};
	Matrix augmented {Matrix<scalar, size_type>::identity(_w)};

	// Gaussian elimination
	for (size_type r1 {0}; r1 < _w; ++r1) {
		for (size_type r2 {0}; r2 < _w; ++r2) {
			if (r1 == r2) {
				continue;
			}

			scalar factor {temp[r2][r1] / temp[r1][r1]};

			for (size_type i {0}; i < _w; ++i) {
				temp[r2][i] -= factor * temp[r1][i];
				augmented[r2][i] -= factor * augmented[r1][i];
			}
		}
	}

	// Gaining identity matrix
	for (size_type r {0}; r < _w; ++r) {
		scalar factor = scalar(1) / temp[r][r];

		for (size_type i {0}; i < _w; ++i) {
			augmented[r][i] *= factor;
		}
	}

	return augmented;
}


template<class scalar, class size_type>
Matrix<scalar, size_type> Matrix<scalar, size_type>::operator*(scalar multiplier) const {
	Matrix result {_w, _h};

	for (size_type j {0}; j < _h; ++j) {
		for (size_type i {0}; i < _w; ++i) {
			result[j][i] = this->operator[](j)[i] * multiplier;
		}
	}
	return result;
}


template<class scalar, class size_type>
Matrix<scalar, size_type> Matrix<scalar, size_type>::operator/(scalar multiplier) const {
	Matrix result {_w, _h};

	for (size_type j {0}; j < _h; ++j) {
		for (size_type i {0}; i < _w; ++i) {
			result[j][i] = this->operator[](j)[i] / multiplier;
		}
	}
	return result;
}


template<class scalar, class size_type>
Matrix<scalar, size_type>& Matrix<scalar, size_type>::operator/=(scalar divisor) {
	for (size_type j {0}; j < _h; ++j) {
		for (size_type i {0}; i < _w; ++i) {
			this->operator[](j)[i] /= divisor;
		}
	}
	return *this;
}


template<class scalar, class size_type>
Matrix<scalar, size_type>& Matrix<scalar, size_type>::operator*=(scalar divisor) {
	for (size_type j {0}; j < _h; ++j) {
		for (size_type i {0}; i < _w; ++i) {
			this->operator[](j)[i] = this->operator[](j)[i] * divisor;
		}
	}
	return *this;
}


template<class scalar, class size_type>
Matrix<scalar, size_type> Matrix<scalar, size_type>::operator*(const Matrix& matrix) const {
	if (matrix._w == 1 && matrix._h == 1) {
		return operator*(matrix[0][0]);
	} else if (_w != matrix._h) {
		return *this;
	}

	Matrix result {matrix._w, _h};
	for (size_type j {0}; j < matrix._w; ++j) {
		for (size_type i {0}; i < _h; ++i) {
			scalar sum {0};
			for (size_type k {0}; k < _w; ++k) {
				sum += this->operator[](i)[k] * matrix[k][j];
			}
			result[i][j] = sum;
		}
	}
	return result;
}


template<class scalar, class size_type>
Matrix<scalar, size_type> Matrix<scalar, size_type>::operator+(const Matrix& matrix) const {
	if (_w != matrix._w || _h != matrix._h) {
		return *this;
	}

	Matrix result {_w, _h};

	for (size_type j {0}; j < _h; ++j) {
		for (size_type i {0}; i < _w; ++i) {
			result[j][i] = this->operator[](j)[i] + matrix[j][i];
		}
	}
	return result;
}


template<class scalar, class size_type>
Matrix<scalar, size_type> Matrix<scalar, size_type>::operator-(const Matrix& matrix) const {
	if (_w != matrix._w || _h != matrix._h) {
		return *this;
	}

	Matrix result {_w, _h};

	for (size_type j {0}; j < _h; ++j) {
		for (size_type i {0}; i < _w; ++i) {
			result[j][i] = this->operator[](j)[i] - matrix[j][i];
		}
	}
	return result;
}


template<class scalar, class size_type>
Matrix<scalar, size_type> Matrix<scalar, size_type>::multiplyChannels(const Matrix& matrix) const {
	if (matrix._w == 1 && matrix._h == 1) {
		return operator*(matrix[0][0]);
	} else if (_w != matrix._h) {
		return *this;
	}

	Matrix result {matrix._w, _h};
	for (size_type j {0}; j < matrix._w; ++j) {
		for (size_type i {0}; i < _h; ++i) {
			scalar sum {0};
			for (size_type k {0}; k < _w; ++k) {
				sum += (this->operator[](i)[k] * matrix[k][j]) >> 10u;
			}
			result[i][j] = sum;
		}
	}
	return result;
}


template<class scalar, class size_type>
Matrix<scalar, size_type> Matrix<scalar, size_type>::multiplyComponents(const Matrix& matrix) const {
	if (_w != matrix._w || _h != matrix._h) {
		return *this;
	}

	Matrix result {_w, _h};

	for (size_type j {0}; j < _h; ++j) {
		for (size_type i {0}; i < _w; ++i) {
			result[j][i] = this->operator[](j)[i] * matrix[j][i];
		}
	}
	return result;
}


template<class scalar, class size_type>
Matrix<scalar, size_type> Matrix<scalar, size_type>::concat(const Matrix& matrix) const {
	if (_h != matrix._h) {
		return *this;
	}

	Matrix result {static_cast<size_type>(_w + matrix._w), _h};

	for (size_type j {0}; j < _h; ++j) {
		for (size_type i {0}; i < _w; ++i) {
			result[j][i] = this->operator[](j)[i];
		}
		for (size_type i {0}; i < matrix._w; ++i) {
			result[j][i + _w] = matrix[j][i];
		}
	}
	return result;
}


template<class scalar, class size_type>
size_type Matrix<scalar, size_type>::getWidth() const {
	return _w;
}


template<class scalar, class size_type>
size_type Matrix<scalar, size_type>::getHeight() const {
	return _h;
}


#ifdef MATRIX_IO


template<class scalar, class size_type>
std::ostream& operator<<(std::ostream& out, const Matrix<scalar, size_type>& matrix) {
	out << matrix._w << ':' << matrix._h << std::endl;

	for (size_type j {0}; j < matrix._h; ++j) {
		for (size_type i {0}; i < matrix._w; ++i) {
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

#endif //TRADER_MATRIX_H
