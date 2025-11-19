/*
 * File:   adc.hpp
 * Author: Mikhail
 *
 * Created on December 19, 2023, 9:06 PM
 */

#ifndef ADC_HPP
#define ADC_HPP

#include "device.h"

#include "RingBuffer.hpp"

namespace adc {
	using Callback = void (*)(uint16_t value);

	void init();

	void measureTemperature(Callback cb);

	void measureBattery(Callback cb);

	void measureX(Callback cb);
	void measureY(Callback cb);
}

#endif /* ADC_HPP */
