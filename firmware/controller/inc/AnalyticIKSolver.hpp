//
// Created by Mikhail on 1.4.24.
//

#ifndef ANALYTIC_IKSOLVER_HPP
#define ANALYTIC_IKSOLVER_HPP

#include <cmath>

#include "IKSolver.hpp"
#include "Matrix.hpp"
#include "util.hpp"

class AnalyticIKSolver: public IKSolver {
public:
	AnalyticIKSolver() = default;

	Vector3<float, uint8_t> solve(const Vector3<float, uint8_t>& eulerAngles);
};


#endif  // ANALYTIC_IKSOLVER_HPP
