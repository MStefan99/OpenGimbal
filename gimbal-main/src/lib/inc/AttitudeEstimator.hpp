//
// Created by MStefan99 on 21.3.22.
//

#ifndef FILTERS_ATTITUDEESTIMATOR_H
#define FILTERS_ATTITUDEESTIMATOR_H

#include <cmath>

#include "util.hpp"



class AttitudeEstimator {
public:
	explicit AttitudeEstimator(float roll, float pitch);

	void update(
			const float* rot,
			const float* acc,
			float dt);

	float getRoll() const;
	float getPitch() const;

protected:
	float _roll {0};
	float _pitch {0};
};


#endif //FILTERS_ATTITUDEESTIMATOR_H
