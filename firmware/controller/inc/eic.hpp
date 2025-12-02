/*
 * File:   eic.hpp
 * Author: Mikhail
 *
 * Created on January 24, 2024, 8:22 PM
 */

#ifndef EIC_HPP
#define EIC_HPP

#include "device.h"

#include "util.hpp"

namespace eic {
	using ButtonCallback = void (*)(bool left, bool pressed);
	using Callback = void       (*)();

	void init();

	void setButtonCallback(ButtonCallback cb);
	void setSensor1Callback(Callback cb);
	void setSensor2Callback(Callback cb);
	void setHostCallback(Callback cb);
}

#endif /* EIC_HPP */
