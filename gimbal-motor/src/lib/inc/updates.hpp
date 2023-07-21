/* 
 * File:   updates.h
 * Author: mikha
 *
 * Created on December 11, 2021, 11:33 AM
 */

#ifndef UPDATES_H
#define	UPDATES_H

#include "device.h"

#include "util.hpp"
#include "data.hpp"
#include "servo.hpp"
#include "usb.hpp"
#include "PID.hpp"
#include "as5600.hpp"
#include "bldc.hpp"

namespace updates {
	void init() __attribute__((optimize("-O0")));

	void ms(); // Called every millisecond
	void fast(); // Called every 20ms
	void slow(); // Called every second
}

#endif	/* UPDATES_H */

