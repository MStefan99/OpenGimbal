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
#include "sbus.hpp"
#include "usb.hpp"

#include "PID.hpp"
#include "AttitudeEstimator.hpp"

#include "lsm6dso.hpp"

#include "bldc.hpp"

namespace updates {
	void init();

	void ms(); // Called every millisecond
	void fast(); // Called every 20ms
	void slow(); // Called every second
}

#endif	/* UPDATES_H */

