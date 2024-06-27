/*
 * File:   adc.hpp
 * Author: Mikhail
 *
 * Created on December 19, 2023, 9:06 PM
 */

#ifndef ADC_HPP
#define ADC_HPP

#include "device.h"

namespace adc {
	void init();

	void getX(void (*cb)(uint16_t value));
	void getY(void (*cb)(uint16_t value));
}

#endif /* ADC_HPP */
