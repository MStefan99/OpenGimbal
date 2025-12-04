//
// Created by Mikhail on 1.4.24.
//

#ifndef NUMERIC_IKSOLVER_HPP
#define NUMERIC_IKSOLVER_HPP

#include <cmath>

#include "IKSolver.hpp"
#include "Matrix.hpp"
#include "util.hpp"

class NumericIKSolver: public IKSolver {
public:
	NumericIKSolver() = default;
	using PropagateCallback = Matrix<float, uint8_t, 3, 3> (*)(const Vector3<float, uint8_t>& motorAngles);

	constexpr static Matrix<float, uint8_t, 3, 3> rotX(float angle);
	constexpr static Matrix<float, uint8_t, 3, 3> rotY(float angle);
	constexpr static Matrix<float, uint8_t, 3, 3> rotZ(float angle);
	constexpr static Matrix<float, uint8_t, 3, 3> rot(Vector3<float, uint8_t> angles);

	void setCallback(PropagateCallback cb);

	Vector3<float, uint8_t> solve(const Vector3<float, uint8_t>& eulerAngles);

protected:
	Vector3<float, uint8_t> _solution {};
	PropagateCallback       propagateCallback {nullptr};

	Matrix<float, uint8_t, 3, 3> propagate(Vector3<float, uint8_t> motorAngles);
	Vector3<float, uint8_t>
	    calculateGradient(const Vector3<float, uint8_t>& angles, const Matrix<float, uint8_t, 3, 3>& target);
};


#endif  // NUMERIC_IKSOLVER_HPP
