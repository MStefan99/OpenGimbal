#include "lib/inc/Mahony.hpp"

Mahony::Mahony(float pGain, float iGain): 
_twoKp {pGain}, _twoKi {iGain}, _quat {} {
    // Nothing to do
}

void Mahony::update(float gx, float gy, float gz, float ax, float ay,
                             float az, float mx, float my, float mz, float dt) {
    // Use IMU algorithm if magnetometer measurement invalid
    // (avoids NaN in magnetometer normalization)
    if ((mx == 0.0f) && (my == 0.0f) && (mz == 0.0f)) {
      updateIMU(gx, gy, gz, ax, ay, az, dt);
      return;
    }

    // Convert gyroscope degrees/sec to radians/sec
    gx *= DEG_TO_RAD;
    gy *= DEG_TO_RAD;
    gz *= DEG_TO_RAD;

    // Compute feedback only if accelerometer measurement valid
    // (avoids NaN in accelerometer normalization)
    if (!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {
        {
            // Normalize accelerometer measurement
            float recipNorm = util::invSqrt(ax * ax + ay * ay + az * az);
            ax *= recipNorm;
            ay *= recipNorm;
            az *= recipNorm;
        }

        {   
            // Normalize magnetometer measurement
            float recipNorm = util::invSqrt(mx * mx + my * my + mz * mz);
            mx *= recipNorm;
            my *= recipNorm;
            mz *= recipNorm;
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
            float hx = 2.0f *
               (mx * (0.5f - yy - zz) + my * (xy - wz) + mz * (xz + wy));
            float hy = 2.0f *
               (mx * (xy + wz) + my * (0.5f - xx - zz) + mz * (yz - wx));
            float bx = std::sqrt(hx * hx + hy * hy);
            float bz = 2.0f *
               (mx * (xz - wy) + my * (yz + wx) + mz * (0.5f - xx - yy));

            // Estimated direction of gravity and magnetic field
            float halfvx = xz - wy;
            float halfvy = wx + yz;
            float halfvz = ww - 0.5f + zz;
            float halfwx = bx * (0.5f - yy - zz) + bz * (xz - wy);
            float halfwy = bx * (xy - wz) + bz * (wx + yz);
            float halfwz = bx * (wy + xz) + bz * (0.5f - xx - yy);

            // Error is sum of cross product between estimated direction
            // and measured direction of field vectors
            float halfex = (ay * halfvz - az * halfvy) + (my * halfwz - mz * halfwy);
            float halfey = (az * halfvx - ax * halfvz) + (mz * halfwx - mx * halfwz);
            float halfez = (ax * halfvy - ay * halfvx) + (mx * halfwy - my * halfwx);

            // Compute and apply integral feedback if enabled
            if (_twoKi > 0.0f) {
            // integral error scaled by Ki
            _integralFBx += _twoKi * halfex * dt;
            _integralFBy += _twoKi * halfey * dt;
            _integralFBz += _twoKi * halfez * dt;
            gx += _integralFBx; // apply integral feedback
            gy += _integralFBy;
            gz += _integralFBz;
            } else {
            _integralFBx = 0.0f; // prevent integral windup
            _integralFBy = 0.0f;
            _integralFBz = 0.0f;
            }

            // Apply proportional feedback
            gx += _twoKp * halfex;
            gy += _twoKp * halfey;
            gz += _twoKp * halfez;
        }
    }

    // Integrate rate of change of quaternion
    gx *= (0.5f * dt); // pre-multiply common factors
    gy *= (0.5f * dt);
    gz *= (0.5f * dt);
    _quat.set(
        _quat.getW() + (-_quat.getX() * gx - _quat.getY() * gy - _quat.getZ() * gz),
        _quat.getX() + ( _quat.getW() * gx + _quat.getY() * gz - _quat.getZ() * gy),
        _quat.getY() + ( _quat.getW() * gy - _quat.getX() * gz + _quat.getZ() * gx),
        _quat.getZ() + ( _quat.getW() * gz + _quat.getX() * gy - _quat.getY() * gx)
    );
}

//-------------------------------------------------------------------------------------------
// IMU algorithm update

void Mahony::updateIMU(float gx, float gy, float gz, float ax,
                                float ay, float az, float dt) {
    // Convert gyroscope degrees/sec to radians/sec
    gx *= DEG_TO_RAD;
    gy *= DEG_TO_RAD;
    gz *= DEG_TO_RAD;

    // Compute feedback only if accelerometer measurement valid
    // (avoids NaN in accelerometer normalization)
    if (!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {
        {
            // Normalize accelerometer measurement
            float recipNorm = util::invSqrt(ax * ax + ay * ay + az * az);
            ax *= recipNorm;
            ay *= recipNorm;
            az *= recipNorm;
        }

        // Estimated direction of gravity
        float halfvx = _quat.getX() * _quat.getZ() - _quat.getW() * _quat.getY();
        float halfvy = _quat.getW() * _quat.getX() + _quat.getY() * _quat.getZ();
        float halfvz = _quat.getW() * _quat.getW() - 0.5f + _quat.getZ() * _quat.getZ();

        // Error is sum of cross product between estimated
        // and measured direction of gravity
        float halfex = (ay * halfvz - az * halfvy);
        float halfey = (az * halfvx - ax * halfvz);
        float halfez = (ax * halfvy - ay * halfvx);

        // Compute and apply integral feedback if enabled
        if (_twoKi > 0.0f) {
        // integral error scaled by Ki
        _integralFBx += _twoKi * halfex * dt;
        _integralFBy += _twoKi * halfey * dt;
        _integralFBz += _twoKi * halfez * dt;
        gx += _integralFBx; // apply integral feedback
        gy += _integralFBy;
        gz += _integralFBz;
        } else {
        _integralFBx = 0.0f; // prevent integral windup
        _integralFBy = 0.0f;
        _integralFBz = 0.0f;
        }

        // Apply proportional feedback
        gx += _twoKp * halfex;
        gy += _twoKp * halfey;
        gz += _twoKp * halfez;
    }

    // Integrate rate of change of quaternion
    gx *= (0.5f * dt); // pre-multiply common factors
    gy *= (0.5f * dt);
    gz *= (0.5f * dt);
    
    _quat.set(
        (-_quat.getX() * gx - _quat.getY() * gy - _quat.getZ() * gz),
        ( _quat.getW() * gx + _quat.getY() * gz - _quat.getZ() * gy),
        ( _quat.getW() * gy - _quat.getX() * gz + _quat.getZ() * gx),
        ( _quat.getW() * gz + _quat.getX() * gy - _quat.getY() * gx)
    );
}
