/*
 * File:   eic.hpp
 * Author: MStefan99
 *
 * Created on March 26, 2025, 2:52 PM
 */

#ifndef EIC_HPP
#define EIC_HPP

#include "device.h"

namespace eic {
	void init();
	void enable();

	void setCallback(void (*cb)());
}

#endif /* EIC_HPP */
