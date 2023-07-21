#include "lib/inc/PID.hpp"


PID::PID(float kp, float ki, float kd, float iLim, float outLim):
	_kp {kp}, _ki{ki}, _kd{kd}, _iLim {iLim}, _outLim {outLim} {
	// Nothing to do
}

	
void PID::setParameters(float kp, float ki, float kd, float iLim, float outLim) {
	_kp = kp;
	_ki = ki;
	_kd = kd;
	_iLim = iLim;
	_outLim = outLim;
}
	
	

float PID::update(float sp, float val) {
	float error {sp - val};
	
	_sum = CLAMP(-_iLim, _sum + error, _iLim);
	
	float out = _kp * error + _ki * _sum + _kd * (val - _prev);
	_prev = val;
	
	return CLAMP(-_outLim, out, _outLim);
}
