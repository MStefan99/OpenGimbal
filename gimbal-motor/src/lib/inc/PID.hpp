/* 
 * File:   PID.hpp
 * Author: MStefan99
 *
 * Created on April 23, 2022, 5:24 PM
 */

#ifndef PID_HPP
#define	PID_HPP

#include "util.hpp"

template <class T>
struct PID {
	PID() = default;
	PID(T kp, T ki, T kd, T iLim);

	T process(T sp, T val);

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
	kp {kp}, ki {ki}, kd {kd}, iLim {iLim / ki} {
	// Nothing to do
}
    
template <class T>
T PID<T>::process(T val, T sp) {
	T error {val - sp};
	
	_sum = util::clamp(_sum + error, -iLim, iLim);
	T out = kp * error + ki * _sum + kd * (val - _prev);
	_prev = val;
	
	return out;
}

#endif	/* PID_HPP */
