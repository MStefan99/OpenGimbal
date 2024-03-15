/* 
 * File:   LowPassFilter.hpp
 * Author: Mikhail
 *
 * Created on March 5, 2024, 5:47 PM
 */

#ifndef LOWPASSFILTER_HPP
#define	LOWPASSFILTER_HPP

#include "util.hpp"


class LowPassFilter {
public:
    LowPassFilter(float sampleRate, float cutoffFrequency) {
        setCoefficients(sampleRate, cutoffFrequency);
    }

    void setCoefficients(float sampleRate, float cutoffFrequency) {
        // Calculate the filter coefficient
        float RC = 1.0 / (cutoffFrequency * TWO_PI);
        float dt = 1.0 / sampleRate;
        alpha = dt / (RC + dt);
    }

    float process(float inputSample) {
        // Apply the filter: simple low-pass IIR filter equation
        y = y + alpha * (inputSample - y);
        return y;
    }
    
    float getState() {
        return y;
    }
    
    float force(float state) {
        y = state;
        return y;
    }

private:
    float y = 0; // Filter state
    float alpha = 0; // Filter coefficient
};

#endif	/* LOWPASSFILTER_HPP */

