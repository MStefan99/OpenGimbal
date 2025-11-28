//
// Created by Mikhail on 1.4.24.
//

#ifndef IKSOLVER_HPP
#define IKSOLVER_HPP

#include "Matrix.hpp"

class IKSolver {
public:
	virtual Vector3<float, uint8_t> solve(const Vector3<float, uint8_t>& eulerAngles) = 0;
};


#endif  // IKSOLVER_HPP
