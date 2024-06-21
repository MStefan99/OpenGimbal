/*
 * File:   LowPassFilter.hpp
 * Author: Mikhail
 *
 * Created on March 5, 2024, 5:47 PM
 */

#ifndef LOW_PASS_FILTER_HPP
#define LOW_PASS_FILTER_HPP

#include "util.hpp"

class LowPassFilter {
public:
	LowPassFilter(float sampleRate, float cutoffFrequency) {
		setCoefficients(sampleRate, cutoffFrequency);
	}

	void setCoefficients(float sampleRate, float cutoffFrequency) {
		// Calculate the filter coefficient
		float RC = 1.0f / (cutoffFrequency * F_2_PI);
		float dt = 1.0f / sampleRate;
		_alpha = dt / (RC + dt);
	}

	float process(float inputSample) {
		// Apply the filter: simple low-pass IIR filter equation
		_y += _alpha * (inputSample - _y);
		return _y;
	}

	float getState() {
		return _y;
	}

	float force(float state) {
		_y = state;
		return _y;
	}

private:
	float _y {0};      // Filter state
	float _alpha {0};  // Filter coefficient
};

#endif /* LOW_PASS_FILTER_HPP */
