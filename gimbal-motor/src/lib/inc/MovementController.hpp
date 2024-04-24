//
// Created by Mikhail on 16.11.23.
//

#ifndef MOVEMENTCONTROLLER_HPP
#define MOVEMENTCONTROLLER_HPP


#include "device.h"

#include "util.hpp"
#include "data.hpp"
#include "options.hpp"

class MovementController {
public:    
	void setRange(uint16_t range);
	void setTarget(int32_t newTarget);
	void setOffset(int32_t newOffset);
	void adjustOffset(int32_t sourcePosition, int32_t targetPosition);
    
    void extrapolate(uint32_t dt, int32_t target);
    void interpolate(uint32_t dt);

	uint16_t getRange() const;
	int32_t getOffset() const;
	uint16_t getTarget() const;
	int32_t getDeflection() const;
	int32_t getDesiredDeflection() const;

protected:
    class Interpolator {
    public:
        int32_t extrapolate(int32_t target) const;
        void applyTarget(uint32_t dt, int32_t target);
        int32_t interpolate(uint32_t dt) const;
        
    protected:
        int32_t _prev {};
        int32_t _actual {};
        int32_t _extrapolated {};
        uint32_t _dt {};
    };
    
	static int32_t wrapValue(int32_t value);

	uint16_t _target {0};
	uint16_t _range {data::options.range}; // 0 indicates no limit
	int32_t _offset {data::options.zeroOffset};
	int32_t _deflection {0};
	int32_t _desiredDeflection {0};
    Interpolator _interpolator {};
};

#endif //MOVEMENTCONTROLLER_HPP
