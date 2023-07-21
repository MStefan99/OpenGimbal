/* 
 * File:   PID.hpp
 * Author: MStefan99
 *
 * Created on April 23, 2022, 5:24 PM
 */

#ifndef PID_HPP
#define	PID_HPP

#include "util.hpp"


struct PID {
	PID() = default;
	PID(float kp, float ki, float kd, float iLim, float outLim);
	
	void setParameters(float kp, float ki, float kd, float iLim, float outLim);

	float update(float sp, float val);


protected:
	float _kp {};
	float _ki {};
	float _kd {};
	
	float _iLim {};
	float _outLim {};

	float _prev {};
	float _sum {};
};


#endif	/* PID_HPP */
