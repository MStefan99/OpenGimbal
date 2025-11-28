//
// Created by Mikhail on 1.4.24.
//

#include "AnalyticIKSolver.hpp"

Vector3<float, uint8_t> AnalyticIKSolver::solve(const Vector3<float, uint8_t>& eulerAngles) {
	float a = eulerAngles[0][0];
	float b = util::clamp(eulerAngles[1][0], -F_PI_3, F_PI_3);
	float g = eulerAngles[2][0];

	float cosA = std::cos(a);
	float sinB = std::sin(b);
	float cosB = std::cos(b);
	float sinG = std::sin(g);

	float cos2A = cosA * cosA;
	float cos2B = cosB * cosB;
	float sin2G = sinG * sinG;

	/* All this IK math takes about 500us of CPU time at 48MHz.
	 * This is not great but it still comfortably fits within the 10ms time budget
	 * and even one tick.
	 * Still, these formulas come straight from MATLAB and probably can (and should)
	 * be optimized further if possible
	 */
	return {
	  {a < 0
	       ? -2
	             * atanf(
	                 (0.5f
	                  * (4.6188f * sinB
	                     + 1.4142f * sqrtf(32.536f - 32 * cos2A * cos2B - 32 * powf(sinB - 0.12941f, 2) - 8.2822f * sinB)
	                     - 2.3909f))
	                 / (3.8637f * sqrtf(1 - 1.4291f * pow(sinB - 0.12941f, 2)) + 4 * cosA * cosB)
	             )
	       : -2
	             * atanf(
	                 -(0.5f
	                   * (1.4142f * sqrtf(32.536f - 32 * cos2A * cos2B - 32 * powf(sinB - 0.12941f, 2) - 8.2822f * sinB)
	                      - 4.6188f * sinB + 2.3909f))
	                 / (3.8637f * sqrtf(1 - 1.4291f * powf(sinB - 0.12941f, 2)) + 4 * cosA * cosB)
	             )},
	  {asinf(1.1954f * sinB - 0.1547f)},
	  {util::abs(g) > F_PI_2
	       ? 2
	             * atanf(
	                 (0.25f
	                  * (1.4142f * sqrtf(32.536f - 32 * cos2B * sin2G - 32 * powf(sinB - 0.12941f, 2) - 8.2822f * sinB)
	                     - 2.1436f * sinB + 4.1411f))
	                 / (1.7321f * sqrtf(1 - 1.4291f * powf(sinB - 0.12941f, 2)) - 2 * cosB * sinG)
	             )
	       : 2
	             * atanf(
	                 -(0.25f
	                   * (2.1436f * sinB
	                      + 1.4142f * sqrtf(32.536f - 32 * cos2B * sin2G - 32 * powf(sinB - 0.12941f, 2) - 8.2822f * sinB)
	                      - 4.1411f))
	                 / (1.7321f * sqrtf(1 - 1.4291f * powf(sinB - 0.12941f, 2)) - 2 * cosB * sinG)
	             )}
	};
}
