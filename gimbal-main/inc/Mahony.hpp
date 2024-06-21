/*
 * File:   Mahony.hpp
 * Author: Mikhail
 *
 * Created on April 23, 2024, 12:44 PM
 */

#ifndef MAHONY_HPP
#define MAHONY_HPP

#include "Quaternion.hpp"
#include "util.hpp"

class Mahony {
public:
	constexpr static float defaultKp {0.5f};
	constexpr static float defaultKi {0.5f};

	Mahony(float Kp = defaultKp, float Ki = defaultKi);

	void update(Vector3<float, uint8_t> rot, Vector3<float, uint8_t> acc, Vector3<float, uint8_t> mag, float dt);
	void updateIMU(Vector3<float, uint8_t> rot, Vector3<float, uint8_t> acc, float dt);

	float getKp();
	float getKi();
	void  setKp(float Kp);
	void  setKi(float Ki);

	Quaternion getQuat() const;

protected:
	float      _twoKp {};  // 2 * proportional gain (Kp)
	float      _twoKi {};  // 2 * integral gain (Ki)
	Quaternion _quat {};
	// Integral error terms scaled by Ki
	float      _integralFBx {};
	float      _integralFBy {};
	float      _integralFBz {};
};

#endif /* MAHONY_HPP */
