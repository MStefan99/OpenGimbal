#include "lib/inc/Mahony.hpp"

Mahony::Mahony(float Kp, float Ki): 
        _twoKp {Kp * 2.0f}, _twoKi {Ki * 2.0f}, _quat {} {
    // Nothing to do
}

float Mahony::getKp() {
    return _twoKp / 2.0f;
}
        
float Mahony::getKi() {
    return _twoKi / 2.0f;
}

void Mahony::setKp(float Kp) {
    _twoKp = Kp * 2.0f;
}
        
void Mahony::setKi(float Ki) {
    _twoKi = Ki * 2.0f;
}

Quaternion Mahony::getQuat() const {
    return _quat;
}

// Thanks to Adafruit for Mahony filter implementation
// https://github.com/adafruit/Adafruit_AHRS
void Mahony::update(
        Vector3<float, uint8_t> rot, 
        Vector3<float, uint8_t> acc,
        Vector3<float, uint8_t> mag,
        float dt) {
    // Use IMU algorithm if magnetometer measurement invalid
    // (avoids NaN in magnetometer normalization)
    if ((mag[0][0] == 0.0f) && (mag[1][0] == 0.0f) && (mag[2][0] == 0.0f)) {
      updateIMU(rot, acc, dt);
      return;
    }

    // Convert gyroscope degrees/sec to radians/sec
    rot[0][0] *= F_DEG_TO_RAD;
    rot[1][0] *= F_DEG_TO_RAD;
    rot[2][0] *= F_DEG_TO_RAD;

    // Compute feedback only if accelerometer measurement valid
    // (avoids NaN in accelerometer normalization)
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

        {
            // Auxiliary variables to avoid repeated arithmetic
            float ww = _quat.getW() * _quat.getW();
            float wx = _quat.getW() * _quat.getX();
            float wy = _quat.getW() * _quat.getY();
            float wz = _quat.getW() * _quat.getZ();
            float xx = _quat.getX() * _quat.getX();
            float xy = _quat.getX() * _quat.getY();
            float xz = _quat.getX() * _quat.getZ();
            float yy = _quat.getY() * _quat.getY();
            float yz = _quat.getY() * _quat.getZ();
            float zz = _quat.getZ() * _quat.getZ();

            // Reference direction of Earth's magnetic field
            float hx = 2.0f * (mag[0][0] * (0.5f - yy - zz) + mag[1][0] * (xy - wz) + mag[2][0] * (xz + wy));
            float hy = 2.0f * (mag[0][0] * (xy + wz) + mag[1][0] * (0.5f - xx - zz) + mag[2][0] * (yz - wx));
            float bx = std::sqrt(hx * hx + hy * hy);
            float bz = 2.0f * (mag[0][0] * (xz - wy) + mag[1][0] * (yz + wx) + mag[2][0] * (0.5f - xx - yy));

            // Estimated direction of gravity and magnetic field
            float halfvx = xz - wy;
            float halfvy = wx + yz;
            float halfvz = ww - 0.5f + zz;
            float halfwx = bx * (0.5f - yy - zz) + bz * (xz - wy);
            float halfwy = bx * (xy - wz) + bz * (wx + yz);
            float halfwz = bx * (wy + xz) + bz * (0.5f - xx - yy);

            // Error is sum of cross product between estimated direction
            // and measured direction of field vectors
            float halfex = (acc[1][0] * halfvz - acc[2][0] * halfvy) + (mag[1][0] * halfwz - mag[2][0] * halfwy);
            float halfey = (acc[2][0] * halfvx - acc[0][0] * halfvz) + (mag[2][0] * halfwx - mag[0][0] * halfwz);
            float halfez = (acc[0][0] * halfvy - acc[1][0] * halfvx) + (mag[0][0] * halfwy - mag[1][0] * halfwx);

            // Compute and apply integral feedback if enabled
            if (_twoKi > 0.0f) {
                // integral error scaled by Ki
                _integralFBx += _twoKi * halfex * dt;
                _integralFBy += _twoKi * halfey * dt;
                _integralFBz += _twoKi * halfez * dt;
                rot[0][0] += _integralFBx; // apply integral feedback
                rot[1][0] += _integralFBy;
                rot[2][0] += _integralFBz;
            } else {
                _integralFBx = 0.0f; // prevent integral windup
                _integralFBy = 0.0f;
                _integralFBz = 0.0f;
            }

            // Apply proportional feedback
            rot[0][0] += _twoKp * halfex;
            rot[1][0] += _twoKp * halfey;
            rot[2][0] += _twoKp * halfez;
        }
    }

    // Integrate rate of change of quaternion
    rot[0][0] *= (0.5f * dt); // pre-multiply common factors
    rot[1][0] *= (0.5f * dt);
    rot[2][0] *= (0.5f * dt);
    _quat.set(
        _quat.getW() + (-_quat.getX() * rot[0][0] - _quat.getY() * rot[1][0] - _quat.getZ() * rot[2][0]),
        _quat.getX() + ( _quat.getW() * rot[0][0] + _quat.getY() * rot[2][0] - _quat.getZ() * rot[1][0]),
        _quat.getY() + ( _quat.getW() * rot[1][0] - _quat.getX() * rot[2][0] + _quat.getZ() * rot[0][0]),
        _quat.getZ() + ( _quat.getW() * rot[2][0] + _quat.getX() * rot[1][0] - _quat.getY() * rot[0][0])
    );
}

void Mahony::updateIMU(
        Vector3<float, uint8_t> rot, 
        Vector3<float, uint8_t> acc,
        float dt) {
    // Convert gyroscope degrees/sec to radians/sec
    rot[0][0] *= F_DEG_TO_RAD;
    rot[1][0] *= F_DEG_TO_RAD;
    rot[2][0] *= F_DEG_TO_RAD;

    // Compute feedback only if accelerometer measurement valid
    // (avoids NaN in accelerometer normalization)
    if (!(util::abs(acc[0][0]) < 1e-5 && util::abs(acc[1][0]) < 1e-5 && util::abs(acc[2][0]) < 1e-5)) {
        {
            // Normalize accelerometer measurement
            float recipNorm = util::invSqrt(acc[0][0] * acc[0][0] + acc[1][0] * acc[1][0] + acc[2][0] * acc[2][0]);
            acc[0][0] *= recipNorm;
            acc[1][0] *= recipNorm;
            acc[2][0] *= recipNorm;
        }

        // Estimated direction of gravity
        float halfvx = _quat.getX() * _quat.getZ() - _quat.getW() * _quat.getY();
        float halfvy = _quat.getW() * _quat.getX() + _quat.getY() * _quat.getZ();
        float halfvz = _quat.getW() * _quat.getW() - 0.5f + _quat.getZ() * _quat.getZ();

        // Error is sum of cross product between estimated
        // and measured direction of gravity
        float halfex = (acc[1][0] * halfvz - acc[2][0] * halfvy);
        float halfey = (acc[2][0] * halfvx - acc[0][0] * halfvz);
        float halfez = (acc[0][0] * halfvy - acc[1][0] * halfvx);

        // Compute and apply integral feedback if enabled
        if (_twoKi > 0.0f) {
            // integral error scaled by Ki
            _integralFBx += _twoKi * halfex * dt;
            _integralFBy += _twoKi * halfey * dt;
            _integralFBz += _twoKi * halfez * dt;
            rot[0][0] += _integralFBx; // apply integral feedback
            rot[1][0] += _integralFBy;
            rot[2][0] += _integralFBz;
        } else {
            _integralFBx = 0.0f; // prevent integral windup
            _integralFBy = 0.0f;
            _integralFBz = 0.0f;
        }

        // Apply proportional feedback
        rot[0][0] += _twoKp * halfex;
        rot[1][0] += _twoKp * halfey;
        rot[2][0] += _twoKp * halfez;
    }

    // Integrate rate of change of quaternion
    rot[0][0] *= (0.5f * dt); // pre-multiply common factors
    rot[1][0] *= (0.5f * dt);
    rot[2][0] *= (0.5f * dt);
    
    _quat.set(
        _quat.getW() + (-_quat.getX() * rot[0][0] - _quat.getY() * rot[1][0] - _quat.getZ() * rot[2][0]),
        _quat.getX() + ( _quat.getW() * rot[0][0] + _quat.getY() * rot[2][0] - _quat.getZ() * rot[1][0]),
        _quat.getY() + ( _quat.getW() * rot[1][0] - _quat.getX() * rot[2][0] + _quat.getZ() * rot[0][0]),
        _quat.getZ() + ( _quat.getW() * rot[2][0] + _quat.getX() * rot[1][0] - _quat.getY() * rot[0][0])
    );
}
