//
// Created by Mikhail on 1.4.24.
//

#ifndef SANDBOX_IKSOLVER_HPP
#define SANDBOX_IKSOLVER_HPP

#include <cmath>
#include "Matrix.hpp"


class IKSolver {
public:
	IKSolver() = default;

	static constexpr Matrix<float, unsigned, 3, 3> rotX(float angle);
	static constexpr Matrix<float, unsigned, 3, 3> rotY(float angle);
	static constexpr Matrix<float, unsigned, 3, 3> rotZ(float angle);
	static constexpr Matrix<float, unsigned, 3, 3> rot(Matrix<float, unsigned, 3, 1> angles);

	bool solve(float x, float y, float z, bool reusePrevious = true);
	Matrix<float, unsigned, 3, 1> getSolution() const;

protected:
	Matrix<float, unsigned, 3, 1> _solution {};

	static Matrix<float, unsigned, 3, 3> propagate(Matrix<float, unsigned, 3, 1> motorAngles);
	static Matrix<float, unsigned, 3, 1>
	calculateGradient(const Matrix<float, unsigned, 3, 1>& angles, const Matrix<float, unsigned, 3, 3>& target);
};


#endif //SANDBOX_IKSOLVER_HPP
