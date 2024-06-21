#include "Quaternion.hpp"


Quaternion::Quaternion(float w, float x, float y, float z): _w(w), _x(x), _y(y), _z(z) {
    normalize();
}

void Quaternion::normalize() {
    float norm = util::invSqrt(_w * _w + _x * _x + _y * _y + _z * _z);
    _w *= norm;
    _x *= norm;
    _y *= norm;
    _z *= norm;
}

Quaternion Quaternion::conjugate() const {
    return {
        _w,
        -_x,
        -_y,
        -_z
    };
}

Quaternion Quaternion::operator *(const Quaternion& q) const {
    return {
        _w * q._w - _x * q._x - _y * q._y - _z * q._z,
        _w * q._x + _x * q._w + _y * q._z - _z * q._y,
        _w * q._y - _x * q._z + _y * q._w + _z * q._x,
        _w * q._z + _x * q._y - _y * q._x + _z * q._w
    };
}

Vector3<float, uint8_t> Quaternion::toEuler() const {
    float yaw, pitch, roll;

    roll = atan2f(2.0f * (_w * _x + _y * _z), 1.0f - 2.0f * (_x * _x + _y * _y));
    pitch = asinf(2.0f * (_w * _y - _z * _x));
    yaw = atan2f(2.0f * (_w * _z + _x * _y), 1.0f - 2.0f * (_y * _y + _z * _z));

    return {{yaw}, {pitch}, {roll}};
}

Quaternion Quaternion::fromEuler(float yaw, float pitch, float roll) {
    float sy = sinf(yaw * 0.5f);
    float cy = cosf(yaw * 0.5f);
    float sp = sinf(pitch * 0.5f);
    float cp = cosf(pitch * 0.5f);
    float sr = sinf(roll * 0.5f);
    float cr = cosf(roll * 0.5f);

    return Quaternion(
        sy * sp * sr + cy * cp * cr,
        cy * cp * sr - sy * sp * cr,
        cy * sp * cr + sy * cp * sr,
        sy * cp * cr - cy * sp * sr
    );
}

Matrix<float, uint8_t, 3, 3> Quaternion::toRotationMatrix() const {
    float xx = _x * _x;
    float yy = _y * _y;
    float zz = _z * _z;
    float wx = _w * _x;
    float wy = _w * _y;
    float wz = _w * _z;
    float xy = _x * _y;
    float xz = _x * _z;
    float yz = _y * _z;

    return {{1.0f - 2.0f * (yy + zz), 2.0f * (xy - wz), 2.0f * (xz + wy)},
            {2.0f * (xy + wz), 1.0f - 2.0f * (xx + zz), 2.0f * (yz - wx)},
            {2.0f * (xz - wy), 2.0f * (yz + wx), 1.0f - 2.0f * (xx + yy)}};
}

float Quaternion::getW() const {
    return _w;
}

float Quaternion::getX() const {
    return _x;
}

float Quaternion::getY() const {
    return _y;
}

float Quaternion::getZ() const {
    return _z;
}

void Quaternion::setW(float w) {
    _w = w;
    normalize();
}

void Quaternion::setX(float x) {
    _x = x;
    normalize();
}

void Quaternion::setY(float y) {
    _y = y;
    normalize();
}

void Quaternion::setZ(float z) {
    _z = z;
    normalize();
}

void Quaternion::set(float w, float x, float y, float z) {
    _w = w;
    _x = x;
    _y = y;
    _z = z;
    normalize();
}
