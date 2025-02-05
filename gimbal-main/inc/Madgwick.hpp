// Adafruit Madgwick filter implementation
// https://github.com/adafruit/Adafruit_AHRS
//
//=============================================================================================
//
// Madgwick's implementation of Mayhony's AHRS algorithm.
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

#ifndef MADGWICK_HPP
#define MADGWICK_HPP

#include "Quaternion.hpp"
#include "util.hpp"

class Madgwick {
public:
	constexpr static float defaultBeta {0.1f};

	Madgwick(float gain = defaultBeta);

	void update(Vector3<float, uint8_t> rot, Vector3<float, uint8_t> acc, Vector3<float, uint8_t> mag, float dt);
	void updateIMU(Vector3<float, uint8_t> rot, Vector3<float, uint8_t> acc, float dt);

	float getBeta() const;

	void setBeta(float beta);

	Quaternion getQuaternion() const;
	void       setQuaternion(const Quaternion& quat);

	//	void getGravityVector(float* x, float* y, float* z) {
	//		if (!anglesComputed) {
	//			computeAngles();
	//		}
	//		*x = grav[0];
	//		*y = grav[1];
	//		*z = grav[2];
	//	}

protected:
	float      _beta {defaultBeta};
	Quaternion _quat {};
	Quaternion _grav {};
};

#endif
