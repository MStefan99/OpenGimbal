#include "MovementController.hpp"

void MovementController::setTarget(int32_t newTarget) {
	_target = util::mod(newTarget + _offset, static_cast<int32_t>(4096));
}

void MovementController::setOffset(int32_t newOffset) {
	_interpolator.offset(newOffset - _offset);
	_offset = newOffset;
}

void MovementController::setMaxSpeed(uint8_t maxSpeed) {
	_interpolator._maxSpeed = maxSpeed;
}

void MovementController::adjustOffset(int32_t sourcePosition, int32_t desiredPosition) {
	sourcePosition = util::mod(sourcePosition, static_cast<int32_t>(4096));
	desiredPosition = util::mod(desiredPosition, static_cast<int32_t>(4096));

	int32_t adjustedOffset = sourcePosition - desiredPosition;
	setOffset(adjustedOffset);
}

int32_t MovementController::getOffset() const {
	return _offset;
}

uint16_t MovementController::getTarget() const {
	return _target;
}

uint8_t MovementController::getMaxSpeed() const {
	return _interpolator._maxSpeed;
}

MovementController::Interpolator::Interpolator(int32_t offset):
  _prev(offset),
  _actual(offset),
  _extrapolated(offset) {
	// Nothing to do
}

int32_t MovementController::Interpolator::extrapolate(int32_t target) const {
	auto    actual {_actual};
	int32_t change {target - _actual};

	if (change < -2048) {
		actual -= 4096;
		change = target - actual;
	} else if (change > 2048) {
		actual += 4096;
		change = target - actual;
	}

	return _actual + change;
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

	if (_maxSpeed > 0 && util::abs(change / dt) > _maxSpeed / 32) {
		dt = util::abs(change / _maxSpeed * 32);
	}

	_actual = target;
	_prev = _extrapolated;
	_extrapolated = _actual + change;
	_dt = dt;
}

int32_t MovementController::Interpolator::interpolate(uint32_t dt) const {
	return util::interpolate(_prev, _extrapolated, static_cast<float>(dt) / static_cast<float>(_dt));
}

void MovementController::extrapolate(uint32_t dt, int32_t target) {
	if (dt <= 0 || dt > maxInterpolationTime) {
		dt = 1;
	}

	setTarget(_interpolator.extrapolate(target));

	_interpolator.applyTarget(dt, _target);
}

void MovementController::interpolate(uint32_t dt) {
	_target = util::mod(_interpolator.interpolate(dt), static_cast<int32_t>(4096));
}
