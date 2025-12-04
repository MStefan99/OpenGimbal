//
// Created by Mikhail on 1.4.24.
//

#include "NumericIKSolver.hpp"

constexpr static float firstStep = M_PI / 16;

constexpr Matrix<float, uint8_t, 3, 3> NumericIKSolver::rotX(float angle) {
	return {
	  {1, 0,	             0	             },
	  {0, std::cos(angle), -std::sin(angle)},
	  {0, std::sin(angle), std::cos(angle) }
	};
}

constexpr Matrix<float, uint8_t, 3, 3> NumericIKSolver::rotY(float angle) {
	return {
	  {std::cos(angle),  0, std::sin(angle)},
	  {0,	              1, 0	            },
	  {-std::sin(angle), 0, std::cos(angle)}
	};
}

constexpr Matrix<float, uint8_t, 3, 3> NumericIKSolver::rotZ(float angle) {
	return {
	  {std::cos(angle), -std::sin(angle), 0},
	  {std::sin(angle), std::cos(angle),  0},
	  {0,	             0,	              1}
	};
}

constexpr Matrix<float, uint8_t, 3, 3> NumericIKSolver::rot(Vector3<float, uint8_t> angles) {
	return rotZ(angles[2][0]) * rotY(angles[1][0]) * rotX(angles[0][0]);
}

constexpr static auto r12 {NumericIKSolver::rotX(-F_PI_3)};
constexpr static auto r34 {NumericIKSolver::rotY(75 * F_PI / 180)};
constexpr static auto r56 {NumericIKSolver::rotY(F_PI_2)};

Matrix<float, uint8_t, 3, 3> NumericIKSolver::propagate(Vector3<float, uint8_t> motorAngles) {
	return propagateCallback
	         ? propagateCallback(motorAngles)
	         : rotZ(motorAngles[0][0]) * r12 * rotZ(motorAngles[1][0]) * r34 * rotZ(motorAngles[2][0]) * r56;
}

Vector3<float, uint8_t> NumericIKSolver::calculateGradient(
    const Vector3<float, uint8_t>&      angles,
    const Matrix<float, uint8_t, 3, 3>& target
) {
	Vector3<float, uint8_t> result {{0}, {0}, {0}};
	auto                    adjustedAngles {angles};
	float                   magnitudeSquared {0};
	float                   err {(propagate(angles) - target).norm()};

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

void NumericIKSolver::setCallback(PropagateCallback cb) {
	propagateCallback = cb;
}

Vector3<float, uint8_t> NumericIKSolver::solve(const Vector3<float, uint8_t>& angles) {
	Vector3<float, uint8_t> result {_solution};

	auto r = rot({{angles[0][0]}, {angles[1][0]}, {angles[2][0]}});

	for (unsigned i {0}; i < 50; ++i) {
		auto  err = (propagate(result) - r).norm();
		float step {firstStep * (1.0f / (i + 1.0f))};
		if (step < err) {
			step = err;
		}

		if (err < 0.05f) {
			_solution = result;
			return result;
		}

		auto gradient = calculateGradient(result, r);

		for (unsigned j {0}; j < 3; ++j) {
			result[j][0] -= gradient[j][0] * step;
		}
	}

	return {{std::nan("0")}, {std::nan("1")}, {std::nan("2")}};
}
