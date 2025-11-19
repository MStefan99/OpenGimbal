/*
 * File:   PID.hpp
 * Author: MStefan99
 *
 * Created on April 23, 2022, 5:24 PM
 */

#ifndef PID_HPP
#define PID_HPP

#include "util.hpp"

template <class T>
struct PID {
	PID() = default;
	PID(T kp, T ki, T kd, T iLim);

	T process(T val, T sp = 0, float dt = 0.001f);

	T kp {};
	T ki {};
	T kd {};

	T iLim {};

protected:
	T _prev {};
	T _sum {};
};

template <class T>
PID<T>::PID(T kp, T ki, T kd, T iLim):
  kp {kp},
  ki {ki},
  kd {kd},
  iLim {iLim} {
	// Nothing to do
}

template <class T>
T PID<T>::process(T val, T sp, float dt) {
	T error {val - sp};

	_sum = util::clamp(ki * error * dt + _sum, -iLim, iLim);
	T out = kp * error + _sum + kd * (val - _prev) / dt;
	_prev = val;

	return out;
}

#endif /* PID_HPP */
