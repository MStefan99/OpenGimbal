//
// Created by Mikhail on 16.11.23.
//

#ifndef SANDBOX_MOVEMENTCONTROLLER_HPP
#define SANDBOX_MOVEMENTCONTROLLER_HPP


#include <cstdint>
#include <algorithm>

class MovementController {
public:
	void setRange(uint16_t range);
	void setTarget(uint16_t newTarget);
	void setOffset(int32_t newOffset);
	void adjustOffset(uint16_t sourcePosition, uint16_t targetPosition);

	uint16_t getRange() const;
	int32_t getOffset() const;
	uint16_t getTarget() const;
	int32_t getDeflection() const;
	int32_t getDesiredDeflection() const;

#ifdef DEBUG

	void reset() {
		offset = target = deflection = desiredDeflection = 0;
	}

#endif

protected:
	static int32_t wrapValue(int32_t value);

	int32_t offset {0};
	uint16_t target {0};
	uint16_t allowedRange {2048}; // 0 indicates no limit
	int32_t deflection {0};
	int32_t desiredDeflection {0};
};

#endif //SANDBOX_MOVEMENTCONTROLLER_HPP
