//
// Created by Mikhail on 16.11.23.
//

#ifndef MOVEMENT_CONTROLLER_HPP
#define MOVEMENT_CONTROLLER_HPP


#include "device.h"

#include "nvm.hpp"
#include "options.hpp"
#include "util.hpp"

class MovementController {
public:
	void setRange(uint16_t range);
	void setTarget(int32_t newTarget);
	void setOffset(int32_t newOffset);
	void setMaxSpeed(uint8_t maxSpeed);
	void adjustOffset(int32_t sourcePosition, int32_t targetPosition);

	void extrapolate(uint32_t dt, int32_t target);
	void interpolate(uint32_t dt);

	uint16_t getRange() const;
	int32_t  getOffset() const;
	uint16_t getTarget() const;
	uint8_t  getMaxSpeed() const;

protected:
	class Interpolator {
	public:
		Interpolator() = delete;
		Interpolator(int32_t offset);

		void    offset(int32_t offset);
		void    applyTarget(uint32_t dt, int32_t target);
		int32_t interpolate(uint32_t dt) const;

		uint16_t _maxSpeed {nvm::options->maxSpeed};

	protected:
		int32_t  _prev {};
		int32_t  _actual {};
		int32_t  _extrapolated {};
		uint32_t _dt {1};
	};

	static int32_t wrapValue(int32_t value);

	uint16_t     _target {0};
	int32_t      _offset {nvm::options->zeroOffset};
	Interpolator _interpolator {nvm::options->zeroOffset};
};

#endif  // MOVEMENT_CONTROLLER_HPP
