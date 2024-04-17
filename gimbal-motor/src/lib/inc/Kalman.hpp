//
// Created by mikha on 11.1.22.
//

#ifndef FILTERS_KALMAN_HPP
#define FILTERS_KALMAN_HPP

#include "Matrix.hpp"


template <class scalar = float, class size_type = unsigned int, size_type nx = 1, size_type nu = 1, size_type nz = 1>
class Kalman {
public:
	Kalman() = delete;
	Kalman(const Kalman& kalman) = default;
	Kalman(const Matrix<scalar, size_type, nx, 1>& x0,
	       const Matrix<scalar, size_type, nx, nx>& P0,
	       const Matrix<scalar, size_type, nx, nx>& Q,
	       const Matrix<scalar, size_type, nz, nz>& R);

	void predict(const Matrix<scalar, size_type, nx, nx>& F);
	void predict(const Matrix<scalar, size_type, nx, nx>& F,
	             const Matrix<scalar, size_type, nx, nu>& G,
	             const Matrix<scalar, size_type, nu, 1>& u);
	void correct(const Matrix<scalar, size_type, nz, nx>& H,
	             const Matrix<scalar, size_type, nz, 1>& z);

	Matrix<scalar, size_type, nx, 1> x() const;
	Matrix<scalar, size_type, nx, nx> P() const;

protected:
	Matrix<scalar, size_type, nx, 1> _x {};
	Matrix<scalar, size_type, nx, nx> _P {};
	Matrix<scalar, size_type, nx, nx> _Q {};
	Matrix<scalar, size_type, nz, nz> _R {};
};


template <class scalar, class size_type, size_type nx, size_type nu, size_type nz>
Kalman<scalar, size_type, nx, nu, nz>::Kalman(
		const Matrix<scalar, size_type, nx, 1>& x0,
		const Matrix<scalar, size_type, nx, nx>& P0,
		const Matrix<scalar, size_type, nx, nx>& Q,
		const Matrix<scalar, size_type, nz, nz>& R):
		_x {x0}, _P {P0}, _Q {Q}, _R {R} {
	// Nothing to do
}
        
        
template <class scalar, class size_type, size_type nx, size_type nu, size_type nz>
void Kalman<scalar, size_type, nx, nu, nz>::predict(const Matrix<scalar, size_type, nx, nx>& F) {
	_x = F * _x;
	_P = F * _P * F.transpose() + _Q;
}


template <class scalar, class size_type, size_type nx, size_type nu, size_type nz>
void Kalman<scalar, size_type, nx, nu, nz>::predict(const Matrix<scalar, size_type, nx, nx>& F,
                                                    const Matrix<scalar, size_type, nx, nu>& G,
                                                    const Matrix<scalar, size_type, nu, 1>& u) {
	_x = F * _x + G * u;
	_P = F * _P * F.transpose() + _Q;
}


template <class scalar, class size_type, size_type nx, size_type nu, size_type nz>
void Kalman<scalar, size_type, nx, nu, nz>::correct(const Matrix<scalar, size_type, nz, nx>& H,
                                                    const Matrix<scalar, size_type, nz, 1>& z) {
    auto Ht {H.transpose()};
	auto K {_P * Ht * (H * _P * Ht + _R).inverse()};
	_x = _x + K * (z - H * _x);

	auto IKH {Matrix<scalar, size_type, nx, nx>::identity() - K * H};
	_P = IKH * _P * IKH.transpose() + K * _R * K.transpose();
}


template <class scalar, class size_type, size_type nx, size_type nu, size_type nz>
Matrix<scalar, size_type, nx, 1> Kalman<scalar, size_type, nx, nu, nz>::x() const {
	return _x;
}


template <class scalar, class size_type, size_type nx, size_type nu, size_type nz>
Matrix<scalar, size_type, nx, nx> Kalman<scalar, size_type, nx, nu, nz>::P() const {
	return _P;
}

#endif //FILTERS_KALMAN_HPP
