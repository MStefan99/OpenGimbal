// Adafruit Madgwick filter implementation
// https://github.com/adafruit/Adafruit_AHRS
//
//=============================================================================================
//
// Madgwick's implementation of Macc[1][0]hony's AHRS algorithm.
// See: http://www.x-io.co.uk/open-source-imu-and-ahrs-algorithms/
//
// From the x-io website "Open-source resources available on this website are
// provided under the GNU General Public Licence unless an alternative licence
// is provided in source."
//
// Date			Author			Notes
// 29/09/2011	SOH Madgwick    Initial release
// 02/10/2011	SOH Madgwick	Optimised for reduced CPU load
//
// Algorithm paper:
// http://ieeexplore.ieee.org/xpl/login.jsp?tp=&arnumber=4608934&url=http%3A%2F%2Fieeexplore.ieee.org%2Fstamp%2Fstamp.jsp%3Ftp%3D%26arnumber%3D4608934
//
//=============================================================================================

#include "Madgwick.hpp"

Madgwick::Madgwick(float gain) {
	_beta = gain;
}

float Madgwick::getBeta() const {
	return _beta;
}

void Madgwick::setBeta(float beta) {
	_beta = beta;
}

Quaternion Madgwick::getQuaternion() const {
	return _quat;
}

void Madgwick::setQuaternion(const Quaternion& quat) {
	_quat = quat;
}

void Madgwick::update(Vector3<float, uint8_t> rot, Vector3<float, uint8_t> acc, Vector3<float, uint8_t> mag, float dt) {
	// Use IMU algorithm if magnetometer measurement invalid (avoids NaN in
	// magnetometer normalization)
	if ((mag[0][0] == 0.0f) && (mag[1][0] == 0.0f) && (mag[2][0] == 0.0f)) {
		updateIMU(rot, acc, dt);
		return;
	}

	// Convert gyroscope degrees/sec to radians/sec
	rot[0][0] *= F_DEG_TO_RAD;
	rot[1][0] *= F_DEG_TO_RAD;
	rot[2][0] *= F_DEG_TO_RAD;

	// Rate of change of quaternion from gyroscope
	float qDot1 = 0.5f * (-_quat.getX() * rot[0][0] - _quat.getY() * rot[1][0] - _quat.getZ() * rot[2][0]);
	float qDot2 = 0.5f * (_quat.getW() * rot[0][0] + _quat.getY() * rot[2][0] - _quat.getZ() * rot[1][0]);
	float qDot3 = 0.5f * (_quat.getW() * rot[1][0] - _quat.getX() * rot[2][0] + _quat.getZ() * rot[0][0]);
	float qDot4 = 0.5f * (_quat.getW() * rot[2][0] + _quat.getX() * rot[1][0] - _quat.getY() * rot[0][0]);

	// Compute feedback only if accelerometer measurement valid (avoids NaN in
	// accelerometer normalization)
	if (!((acc[0][0] == 0.0f) && (acc[1][0] == 0.0f) && (acc[2][0] == 0.0f))) {
		{
			// Normalize accelerometer measurement
			float recipNorm = util::invSqrt(acc[0][0] * acc[0][0] + acc[1][0] * acc[1][0] + acc[2][0] * acc[2][0]);
			acc[0][0] *= recipNorm;
			acc[1][0] *= recipNorm;
			acc[2][0] *= recipNorm;
		}

		{
			// Normalize magnetometer measurement
			float recipNorm = util::invSqrt(mag[0][0] * mag[0][0] + mag[1][0] * mag[1][0] + mag[2][0] * mag[2][0]);
			mag[0][0] *= recipNorm;
			mag[1][0] *= recipNorm;
			mag[2][0] *= recipNorm;
		}

		// Auxiliary variables to avoid repeated arithmetic
		float _2qwmx = 2.0f * _quat.getW() * mag[0][0];
		float _2qwmy = 2.0f * _quat.getW() * mag[1][0];
		float _2qwmz = 2.0f * _quat.getW() * mag[2][0];
		float _2qxmx = 2.0f * _quat.getX() * mag[0][0];
		float _2qw = 2.0f * _quat.getW();
		float _2qx = 2.0f * _quat.getX();
		float _2qy = 2.0f * _quat.getY();
		float _2qz = 2.0f * _quat.getZ();
		float _2qwqy = 2.0f * _quat.getW() * _quat.getY();
		float _2qyqz = 2.0f * _quat.getY() * _quat.getZ();
		float qwqw = _quat.getW() * _quat.getW();
		float qwqx = _quat.getW() * _quat.getX();
		float qwqy = _quat.getW() * _quat.getY();
		float qwqz = _quat.getW() * _quat.getZ();
		float qxqx = _quat.getX() * _quat.getX();
		float qxqy = _quat.getX() * _quat.getY();
		float qxqz = _quat.getX() * _quat.getZ();
		float qyqy = _quat.getY() * _quat.getY();
		float qyqz = _quat.getY() * _quat.getZ();
		float qzqz = _quat.getZ() * _quat.getZ();

		// Reference direction of Earth's magnetic field
		float hx = mag[0][0] * qwqw - _2qwmy * _quat.getZ() + _2qwmz * _quat.getY() + mag[0][0] * qxqx
		         + _2qx * mag[1][0] * _quat.getY() + _2qx * mag[2][0] * _quat.getZ() - mag[0][0] * qyqy - mag[0][0] * qzqz;
		float hy = _2qwmx * _quat.getZ() + mag[1][0] * qwqw - _2qwmz * _quat.getX() + _2qxmx * _quat.getY()
		         - mag[1][0] * qxqx + mag[1][0] * qyqy + _2qy * mag[2][0] * _quat.getZ() - mag[1][0] * qzqz;
		float _2bx = sqrtf(hx * hx + hy * hy);
		float _2bz = -_2qwmx * _quat.getY() + _2qwmy * _quat.getX() + mag[2][0] * qwqw + _2qxmx * _quat.getZ()
		           - mag[2][0] * qxqx + _2qy * mag[1][0] * _quat.getZ() - mag[2][0] * qyqy + mag[2][0] * qzqz;
		float _4bx = 2.0f * _2bx;
		float _4bz = 2.0f * _2bz;

		// Gradient decent algorithm corrective step
		float s0 = -_2qy * (2.0f * qxqz - _2qwqy - acc[0][0]) + _2qx * (2.0f * qwqx + _2qyqz - acc[1][0])
		         - _2bz * _quat.getY() * (_2bx * (0.5f - qyqy - qzqz) + _2bz * (qxqz - qwqy) - mag[0][0])
		         + (-_2bx * _quat.getZ() + _2bz * _quat.getX()) * (_2bx * (qxqy - qwqz) + _2bz * (qwqx + qyqz) - mag[1][0])
		         + _2bx * _quat.getY() * (_2bx * (qwqy + qxqz) + _2bz * (0.5f - qxqx - qyqy) - mag[2][0]);
		float s1 = _2qz * (2.0f * qxqz - _2qwqy - acc[0][0]) + _2qw * (2.0f * qwqx + _2qyqz - acc[1][0])
		         - 4.0f * _quat.getX() * (1 - 2.0f * qxqx - 2.0f * qyqy - acc[2][0])
		         + _2bz * _quat.getZ() * (_2bx * (0.5f - qyqy - qzqz) + _2bz * (qxqz - qwqy) - mag[0][0])
		         + (_2bx * _quat.getY() + _2bz * _quat.getW()) * (_2bx * (qxqy - qwqz) + _2bz * (qwqx + qyqz) - mag[1][0])
		         + (_2bx * _quat.getZ() - _4bz * _quat.getX())
		               * (_2bx * (qwqy + qxqz) + _2bz * (0.5f - qxqx - qyqy) - mag[2][0]);
		float s2 = -_2qw * (2.0f * qxqz - _2qwqy - acc[0][0]) + _2qz * (2.0f * qwqx + _2qyqz - acc[1][0])
		         - 4.0f * _quat.getY() * (1 - 2.0f * qxqx - 2.0f * qyqy - acc[2][0])
		         + (-_4bx * _quat.getY() - _2bz * _quat.getW())
		               * (_2bx * (0.5f - qyqy - qzqz) + _2bz * (qxqz - qwqy) - mag[0][0])
		         + (_2bx * _quat.getX() + _2bz * _quat.getZ()) * (_2bx * (qxqy - qwqz) + _2bz * (qwqx + qyqz) - mag[1][0])
		         + (_2bx * _quat.getW() - _4bz * _quat.getY())
		               * (_2bx * (qwqy + qxqz) + _2bz * (0.5f - qxqx - qyqy) - mag[2][0]);
		float s3 = _2qx * (2.0f * qxqz - _2qwqy - acc[0][0]) + _2qy * (2.0f * qwqx + _2qyqz - acc[1][0])
		         + (-_4bx * _quat.getZ() + _2bz * _quat.getX())
		               * (_2bx * (0.5f - qyqy - qzqz) + _2bz * (qxqz - qwqy) - mag[0][0])
		         + (-_2bx * _quat.getW() + _2bz * _quat.getY()) * (_2bx * (qxqy - qwqz) + _2bz * (qwqx + qyqz) - mag[1][0])
		         + _2bx * _quat.getX() * (_2bx * (qwqy + qxqz) + _2bz * (0.5f - qxqx - qyqy) - mag[2][0]);

		{
			// Normalize step magnitude
			float recipNorm = util::invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3);
			s0 *= recipNorm;
			s1 *= recipNorm;
			s2 *= recipNorm;
			s3 *= recipNorm;
		}

		// Apply feedback step
		qDot1 -= _beta * s0;
		qDot2 -= _beta * s1;
		qDot3 -= _beta * s2;
		qDot4 -= _beta * s3;
	}

	// Integrate rate of change of quaternion to yield quaternion
	_quat.set(_quat.getW() + qDot1 * dt, _quat.getX() + qDot2 * dt, _quat.getY() + qDot3 * dt, _quat.getZ() + qDot4 * dt);
}

void Madgwick::updateIMU(Vector3<float, uint8_t> rot, Vector3<float, uint8_t> acc, float dt) {
	// Convert gyroscope degrees/sec to radians/sec
	rot[0][0] *= F_DEG_TO_RAD;
	rot[1][0] *= F_DEG_TO_RAD;
	rot[2][0] *= F_DEG_TO_RAD;

	// Rate of change of quaternion from gyroscope
	float qDot1 = 0.5f * (-_quat.getX() * rot[0][0] - _quat.getY() * rot[1][0] - _quat.getZ() * rot[2][0]);
	float qDot2 = 0.5f * (_quat.getW() * rot[0][0] + _quat.getY() * rot[2][0] - _quat.getZ() * rot[1][0]);
	float qDot3 = 0.5f * (_quat.getW() * rot[1][0] - _quat.getX() * rot[2][0] + _quat.getZ() * rot[0][0]);
	float qDot4 = 0.5f * (_quat.getW() * rot[2][0] + _quat.getX() * rot[1][0] - _quat.getY() * rot[0][0]);

	// Compute feedback only if accelerometer measurement valid (avoids NaN in
	// accelerometer normalization)
	if (!((acc[0][0] == 0.0f) && (acc[1][0] == 0.0f) && (acc[2][0] == 0.0f))) {
		{
			// Normalize accelerometer measurement
			float recipNorm = util::invSqrt(acc[0][0] * acc[0][0] + acc[1][0] * acc[1][0] + acc[2][0] * acc[2][0]);
			acc[0][0] *= recipNorm;
			acc[1][0] *= recipNorm;
			acc[2][0] *= recipNorm;
		}

		// Auxiliary variables to avoid repeated arithmetic
		float _2qw = 2.0f * _quat.getW();
		float _2qx = 2.0f * _quat.getX();
		float _2qy = 2.0f * _quat.getY();
		float _2qz = 2.0f * _quat.getZ();
		float _4qw = 4.0f * _quat.getW();
		float _4qx = 4.0f * _quat.getX();
		float _4qy = 4.0f * _quat.getY();
		float _8qx = 8.0f * _quat.getX();
		float _8qy = 8.0f * _quat.getY();
		float qwqw = _quat.getW() * _quat.getW();
		float qxqx = _quat.getX() * _quat.getX();
		float qyqy = _quat.getY() * _quat.getY();
		float qzqz = _quat.getZ() * _quat.getZ();

		// Gradient decent algorithm corrective step
		float s0 = _4qw * qyqy + _2qy * acc[0][0] + _4qw * qxqx - _2qx * acc[1][0];
		float s1 = _4qx * qzqz - _2qz * acc[0][0] + 4.0f * qwqw * _quat.getX() - _2qw * acc[1][0] - _4qx + _8qx * qxqx
		         + _8qx * qyqy + _4qx * acc[2][0];
		float s2 = 4.0f * qwqw * _quat.getY() + _2qw * acc[0][0] + _4qy * qzqz - _2qz * acc[1][0] - _4qy + _8qy * qxqx
		         + _8qy * qyqy + _4qy * acc[2][0];
		float s3 = 4.0f * qxqx * _quat.getZ() - _2qx * acc[0][0] + 4.0f * qyqy * _quat.getZ() - _2qy * acc[1][0];

		{
			// Normalize step magnitude
			float recipNorm = util::invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3);
			s0 *= recipNorm;
			s1 *= recipNorm;
			s2 *= recipNorm;
			s3 *= recipNorm;
		}

		// Apply feedback step
		qDot1 -= _beta * s0;
		qDot2 -= _beta * s1;
		qDot3 -= _beta * s2;
		qDot4 -= _beta * s3;
	}

	// Integrate rate of change of quaternion to yield quaternion
	_quat.set(_quat.getW() + qDot1 * dt, _quat.getX() + qDot2 * dt, _quat.getY() + qDot3 * dt, _quat.getZ() + qDot4 * dt);
}
