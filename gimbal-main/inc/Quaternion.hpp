/*
 * File:   Quaternion.hpp
 * Author: Mikhail
 *
 * Created on April 23, 2024, 12:12 PM
 */

#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include <cmath>

#include "Matrix.hpp"
#include "util.hpp"

class Quaternion {
public:
	Quaternion() = default;
	Quaternion(float w, float x, float y, float z);

	void       normalize();
	Quaternion conjugate() const;

	// This represents rotation q followed by this quaternion
	Quaternion operator* (const Quaternion& q) const;

	// Conversion to Tait-Bryan angles (yaw, pitch, roll)
	Vector3<float, uint8_t> toEuler() const;

	// Conversion from Tait-Bryan angles (yaw, pitch, roll)
	static Quaternion fromEuler(float yaw, float pitch, float roll);

	Matrix<float, uint8_t, 3, 3> toRotationMatrix() const;

	float getW() const;
	float getX() const;
	float getY() const;
	float getZ() const;

	void setW(float w);
	void setX(float x);
	void setY(float y);
	void setZ(float z);

	void set(float w, float x, float y, float z);

protected:
	float _w {1};
	float _x {0};
	float _y {0};
	float _z {0};
};

#endif /* QUATERNION_HPP */
