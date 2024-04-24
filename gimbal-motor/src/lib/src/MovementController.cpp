//
// Created by Mikhail on 16.11.23.
//

#include "lib/inc/MovementController.hpp"

void MovementController::setRange(uint16_t range) {
	_range = range;
}

MovementController::CalculationResult MovementController::calculateTarget(int32_t newTarget) {
	newTarget = util::mod(newTarget + _offset, static_cast<int32_t>(4096));
	int32_t newDeflection = _deflection + newTarget - _target;
	int32_t newDesiredDeflection = newDeflection;

	// Calculating resulting positions
	if (newDesiredDeflection < _desiredDeflection && _desiredDeflection - newDesiredDeflection > 2048) {
		newDeflection += 4096;
		newDesiredDeflection += 4096;
	} else if (newDesiredDeflection > _desiredDeflection && newDesiredDeflection - _desiredDeflection > 2048) {
		newDeflection -= 4096;
		newDesiredDeflection -= 4096;
	}

	if (newDesiredDeflection - newDeflection > 4095) {
		newDesiredDeflection -= 4096;
	} else if (newDesiredDeflection - newDeflection < -4095) {
		newDesiredDeflection += 4096;
	}

	// Limiting the new values
	if (_range) {
		if (newDeflection > _range) {
			newDeflection = _range;
			newTarget = util::mod(newDeflection, static_cast<int32_t>(4096));
		} else if (newDeflection < -_range) {
			newDeflection = -_range;
			newTarget = util::mod(newDeflection, static_cast<int32_t>(4096));
		}
	}
    
    return {newTarget, newDeflection, newDesiredDeflection};
}

void MovementController::setTarget(int32_t newTarget) {
    CalculationResult result {calculateTarget(newTarget)};
    
	_target = result.target;
    _deflection = result.deflection;
    _desiredDeflection = result.desiredDeflection;
}

void MovementController::setOffset(int32_t newOffset) {
    auto diff = _offset - newOffset;
    if (diff < -2048) {
        diff += 4096;
    } else if (diff >= 2048) {
        diff -= 4096;
    }
	_deflection += diff;
	_desiredDeflection += diff;
	_offset = newOffset;
}

void MovementController::adjustOffset(int32_t sourcePosition, int32_t desiredPosition) {
    sourcePosition = util::mod(sourcePosition, static_cast<int32_t>(4096));
    desiredPosition = util::mod(desiredPosition, static_cast<int32_t>(4096));
    
	int32_t adjustedOffset = sourcePosition - desiredPosition;
	setOffset(adjustedOffset);
}

uint16_t MovementController::getRange() const {
	return _range;
}

int32_t MovementController::getOffset() const {
	return _offset;
}

uint16_t MovementController::getTarget() const {
	return _target;
}

int32_t MovementController::getDeflection() const {
	return _deflection;
}

int32_t MovementController::getDesiredDeflection() const {
	return _desiredDeflection;
}

int32_t MovementController::Interpolator::extrapolate(uint32_t dt, int32_t target) {
    int32_t change {target - _actual};
    
    _actual = target;
    _prev = _extrapolated;
    _extrapolated = _actual + change;
    _dt = dt;
    
    return _extrapolated;
}

int32_t MovementController::Interpolator::interpolate(uint32_t dt) {
    return util::interpolate(_prev, _extrapolated, static_cast<float>(dt) / static_cast<float>(_dt));
}

void MovementController::extrapolate(uint32_t dt, int32_t target) {
    CalculationResult result {calculateTarget(_interpolator.extrapolate(dt, target))};
    
    _deflection = result.deflection;
    _desiredDeflection = result.desiredDeflection;
}

void MovementController::interpolate(uint32_t dt) {
    _target = _interpolator.interpolate(dt);
}
