/* 
 * File:   Mahony.hpp
 * Author: Mikhail
 *
 * Created on April 23, 2024, 12:44 PM
 */

#ifndef MAHONY_HPP
#define	MAHONY_HPP

#include "Quaternion.hpp"
#include "util.hpp"

class Mahony {
public:
    static constexpr float defaultTwoKp {2.0f * 0.5f};
    static constexpr float defaultTwoKi {2.0f * 0.5f};
    
    Mahony(float pGain = defaultTwoKp, float iGain = defaultTwoKi);

    void update(float gx, float gy, float gz, float ax, float ay, float az,
                float mx, float my, float mz, float dt);
    void updateIMU(float gx, float gy, float gz, float ax, float ay, float az,
                   float dt);
    
    float getKp() { return _twoKp / 2.0f; }
    void setKp(float Kp) { _twoKp = 2.0f * Kp; }
    float getKi() { return _twoKi / 2.0f; }
    void setKi(float Ki) { _twoKi = 2.0f * Ki; }
  
protected:
    float _twoKp {}; // 2 * proportional gain (Kp)
    float _twoKi {}; // 2 * integral gain (Ki)
    Quaternion _quat {};
    // Integral error terms scaled by Ki
    float _integralFBx {};
    float _integralFBy {};
    float _integralFBz {};
};

#endif	/* MAHONY_HPP */

