//
// Created by Mikhail on 1.4.24.
//

#include "lib/inc/IKSolver.hpp"

static constexpr float firstStep = M_PI / 16;


constexpr Matrix<float, unsigned int, 3, 3> IKSolver::rotX(float angle) {
	return {{1, 0,               0},
	        {0, std::cos(angle), -std::sin(angle)},
	        {0, std::sin(angle), std::cos(angle)}};
}

constexpr Matrix<float, unsigned int, 3, 3> IKSolver::rotY(float angle) {
	return {{std::cos(angle),  0, std::sin(angle)},
	        {0,                1, 0},
	        {-std::sin(angle), 0, std::cos(angle)}};
}

constexpr Matrix<float, unsigned int, 3, 3> IKSolver::rotZ(float angle) {
	return {{std::cos(angle), -std::sin(angle), 0},
	        {std::sin(angle), std::cos(angle),  0},
	        {0,               0,                1}};
}

constexpr Matrix<float, unsigned, 3, 3> IKSolver::rot(Matrix<float, unsigned, 3, 1> angles) {
	return rotZ(angles[2][0]) * rotY(angles[1][0]) * rotX(angles[0][0]);
}

static constexpr auto r12 {IKSolver::rotY(-M_PI_4)};
static constexpr auto r34 {IKSolver::rotX(M_PI_2)};
static constexpr auto r56 {IKSolver::rotZ(-M_PI_4) * IKSolver::rotX(-M_PI_2)};


Matrix<float, unsigned, 3, 3> IKSolver::propagate(Matrix<float, unsigned, 3, 1> motorAngles) {
	return rotZ(motorAngles[0][0])
	       * r12
	       * rotZ(motorAngles[1][0])
	       * r34
	       * rotZ(motorAngles[2][0])
	       * r56;
}

Matrix<float, unsigned, 3, 1>
IKSolver::calculateGradient(const Matrix<float, unsigned, 3, 1>& angles, const Matrix<float, unsigned, 3, 3>& target) {
	Matrix<float, unsigned, 3, 1> result {{0},
	                                      {0},
	                                      {0}};
	auto adjustedAngles {angles};
	float magnitudeSquared {0};
	float err {(propagate(angles) - target).norm()};

	for (unsigned j {0}; j < 3; ++j) {
		adjustedAngles[j][0] += 1e-3;
		result[j][0] = (propagate(adjustedAngles) - target).norm() - err;
		adjustedAngles[j][0] = angles[j][0];
		magnitudeSquared += result[j][0] * result[j][0];
	}

	float magnitude {sqrtf(magnitudeSquared)};

	for (unsigned j {0}; j < 3; ++j) {
		result[j][0] /= magnitude;
	}

	return result;
}

bool IKSolver::solve(float x, float y, float z, bool reusePrevious) {
	Matrix<float, unsigned, 3, 1> result {{0},
	                                      {0},
	                                      {0}};
	if (reusePrevious) {
		result = _solution;
	}
	auto r = rot({{x},
	              {y},
	              {z}});

	for (unsigned i {0}; i < 50; ++i) {
		auto err = (propagate(result) - r).norm();
		float step {firstStep * (1.0f / (i + 1.0f))};
		if (step < err) {
			step = err;
		}

		if (err < 0.01f) {
			_solution = result;
			return true;
		}

		auto gradient = calculateGradient(result, r);

		for (unsigned j {0}; j < 3; ++j) {
			result[j][0] -= gradient[j][0] * step;
		}
	}

	return false;
}

Matrix<float, unsigned, 3, 1> IKSolver::getSolution() const {
	return _solution;
}
