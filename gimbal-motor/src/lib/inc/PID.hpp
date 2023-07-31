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

	T update(T sp, T val);

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
	kp {kp}, ki {ki}, kd {kd}, iLim {iLim} {
	// Nothing to do
}

    static float dt {0.00002};
    
template <class T>
T PID<T>::update(T val, T sp) {
	T error {sp - val};
	
	_sum = util::clamp(-iLim, _sum + error, iLim);
	T out = kp * error + ki * _sum * dt + kd * (val - _prev) / dt;
	_prev = val;
	
	return out;
}

#endif	/* PID_HPP */
