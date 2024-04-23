//
// Created by Mikhail on 16.11.23.
//

#include "lib/inc/MovementController.hpp"

int32_t mod(int32_t a, int32_t b) {
	return ((a % b) + b) % b;
}

void MovementController::setRange(uint16_t range) {
	allowedRange = range;
}

void MovementController::setTarget(int32_t newTarget) {
	newTarget = mod(newTarget + offset, 4096);
	int32_t newDeflection = deflection + newTarget - target;
	int32_t newDesiredDeflection = newDeflection;

	// Calculating resulting positions
	if (newDesiredDeflection < desiredDeflection && desiredDeflection - newDesiredDeflection > 2048) {
		newDeflection += 4096;
		newDesiredDeflection += 4096;
	} else if (newDesiredDeflection > desiredDeflection && newDesiredDeflection - desiredDeflection > 2048) {
		newDeflection -= 4096;
		newDesiredDeflection -= 4096;
	}

	if (newDesiredDeflection - newDeflection > 4095) {
		newDesiredDeflection -= 4096;
	} else if (newDesiredDeflection - newDeflection < -4095) {
		newDesiredDeflection += 4096;
	}

	// Limiting the new values
	if (allowedRange) {
		if (newDeflection > allowedRange) {
			newDeflection = allowedRange;
			newTarget = mod(newDeflection, 4096);
		} else if (newDeflection < -allowedRange) {
			newDeflection = -allowedRange;
			newTarget = mod(newDeflection, 4096);
		}
	}

	target = newTarget;
	deflection = newDeflection;
	desiredDeflection = newDesiredDeflection;
}

void MovementController::setOffset(int32_t newOffset) {
    auto diff = offset - newOffset;
    if (diff < -2048) {
        diff += 4096;
    } else if (diff >= 2048) {
        diff -= 4096;
    }
	deflection += diff;
	desiredDeflection += diff;
	offset = newOffset;
}

void MovementController::adjustOffset(int32_t sourcePosition, int32_t desiredPosition) {
    sourcePosition = mod(sourcePosition, 4096);
    desiredPosition = mod(desiredPosition, 4096);
    
	int32_t adjustedOffset = sourcePosition - desiredPosition;
	setOffset(adjustedOffset);
}

uint16_t MovementController::getRange() const {
	return allowedRange;
}

int32_t MovementController::getOffset() const {
	return offset;
}

uint16_t MovementController::getTarget() const {
	return target;
}

int32_t MovementController::getDeflection() const {
	return deflection;
}

int32_t MovementController::getDesiredDeflection() const {
	return desiredDeflection;
}
