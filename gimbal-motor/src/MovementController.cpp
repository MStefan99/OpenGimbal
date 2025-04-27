#include "MovementController.hpp"

void MovementController::setTarget(int32_t newTarget) {
	_target = util::mod(newTarget + _offset, static_cast<int32_t>(fullRevolution));
}

void MovementController::setOffset(int32_t newOffset) {
	_interpolator.offset(newOffset - _offset);
	_offset = newOffset;
}

void MovementController::adjustOffset(int32_t sourcePosition, int32_t desiredPosition) {
	sourcePosition = util::mod(sourcePosition, static_cast<int32_t>(fullRevolution));
	desiredPosition = util::mod(desiredPosition, static_cast<int32_t>(fullRevolution));

	int32_t adjustedOffset = sourcePosition - desiredPosition;
	setOffset(adjustedOffset);
}

int32_t MovementController::getOffset() const {
	return _offset;
}

uint16_t MovementController::getTarget() const {
	return _target;
}

MovementController::Interpolator::Interpolator(int32_t offset):
  _prev(offset),
  _actual(offset),
  _extrapolated(offset) {
	// Nothing to do
}

void MovementController::Interpolator::offset(int32_t offset) {
	_prev += offset;
	_actual += offset;
}

void MovementController::Interpolator::applyTarget(uint32_t dt, int32_t target) {
	int32_t change {target - _actual};

	if (change < -2048) {
		_actual -= 4096;
		_extrapolated -= 4096;
		change = target - _actual;
	} else if (change > 2048) {
		_actual += 4096;
		_extrapolated += 4096;
		change = target - _actual;
	}

	if (dt == 0 || dt > maxInterpolationTime) {
		_actual = _prev = _extrapolated = target;
		_dt = 1;
	} else {
		_actual = target;
		_prev = _extrapolated;
		_extrapolated = _actual + change;
		_dt = dt;
	}
}

int32_t MovementController::Interpolator::interpolate(uint32_t dt) const {
	return util::interpolate(_prev, _extrapolated, static_cast<float>(dt) / static_cast<float>(_dt));
}

void MovementController::extrapolate(uint32_t dt, int32_t target) {
	setTarget(util::mod(target, static_cast<int32_t>(fullRevolution)));
	_interpolator.applyTarget(dt, _target);
}

void MovementController::interpolate(uint32_t dt) {
	_target = util::mod(_interpolator.interpolate(dt), static_cast<int32_t>(fullRevolution));
}
