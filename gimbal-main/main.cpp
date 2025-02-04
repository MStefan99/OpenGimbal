#include "main.hpp"

static PowerMode  powerMode {PowerMode::Active};
static GimbalMode gimbalMode {GimbalMode::Follow};

constexpr static float attFactor {2048 / F_PI};
constexpr static float maxRestoringVelocity {F_PI / 100.0f};  // Half revolution per second (100 iterations)
constexpr static float ATT_LSB {10430.0f};

static float yawCurrent {0};
static float pitchCurrent {0};
static float rollCurrent {0};

static float yawOffset {0};
static float pitchOffset {0};
static float rollOffset {0};

static float motorAngles[3] {0};

constexpr float sqrt2 = sqrtf(2);

float normalize(float difference) {
	if (difference > F_PI) {
		difference -= F_2_PI;
	} else if (difference < -F_PI) {
		difference += F_2_PI;
	}
	return difference;
}

// CAUTION: This function is called in an interrupt, no long-running operations allowed here!
void processMotorResponse(const uart::DefaultCallback::buffer_type& buffer) {}

Vector3<float, uint8_t> calculateAngles(const Vector3<float, uint8_t>& eulerAngles) {
	float a = eulerAngles[0][0];
	float b = util::clamp(eulerAngles[1][0], -F_PI_3, F_PI_3);
	float g = eulerAngles[2][0];

	float cosA = std::cos(a);
	float sinB = std::sin(b);
	float cosB = std::cos(b);
	float sinG = std::sin(g);

	float cos2A = cosA * cosA;
	float sin2B = sinB * sinB;
	float cos2B = cosB * cosB;
	float sin2G = sinG * sinG;

	float sinM2 = sqrt2 * sinB;

	auto signG {util::sign(util::abs(g) - F_PI_2)};

	return {
	  {-2
	   * atan2f(
	       (sqrt2 * (sqrt2 * sinB - util::sign(a) * sqrtf(2 - 2 * cos2A * cos2B - 2 * sin2B))),
	       (2 * (sqrtf(1 - 2 * sin2B) + cosA * cosB))
	   )},
	  {asinf(sinM2)},
	  {F_PI_4
	   + signG * 2
	         * atan2f(
	             (std::sqrt(2 - 2 * cos2B * sin2G - 2 * sin2B) + signG * 1),
	             (sqrtf(1 - 2 * sin2B) - sqrt2 * cosB * sinG)
	         )}
	};
}

#if DV_OUT
struct Data {
	uint8_t  header {0x03};
	uint16_t dt {};
	float    x {};
	float    y {};
	float    z {};
	float    m1 {};
	float    m2 {};
	float    m3 {};
	uint8_t  footer {0xfc};
} __attribute__((packed));
#endif

void sleep() {  // TODO: disable IMU
	PM_REGS->PM_SLEEPCFG = PM_SLEEPCFG_SLEEPMODE_STANDBY;
	while (PM_REGS->PM_SLEEPCFG != PM_SLEEPCFG_SLEEPMODE_STANDBY);
	do {
		__WFI();
	} while (powerMode == PowerMode::Sleep);
	PM_REGS->PM_SLEEPCFG = PM_SLEEPCFG_SLEEPMODE_IDLE;
	while (PM_REGS->PM_SLEEPCFG != PM_SLEEPCFG_SLEEPMODE_IDLE);
}

int main() {
	util::init();
	i2c::init();
	LSM6DSO32::init();
	uart::init();
	adc::init();
	usb::init();

	uart::setMotorCallback(processMotorResponse);

	PORT_REGS->GROUP[0].PORT_DIRSET = 0x8000F00;

	Mahony mahony {};

	while (1) {
		switch (powerMode) {
			case (PowerMode::Startup): {
				// TODO: check motor state

				motor::move();
				motor::tone(motor::all, 247);
				util::sleep(205);

				for (uint8_t i {15}; i > 4; i -= 5) {
					motor::tone(motor::all, 294);
					motor::move(motor::all, 0, i);
					util::sleep(205);
					motor::tone(motor::all, 392);
					motor::move(motor::all, 0, i);
					util::sleep(205);
				}
				motor::tone();

				powerMode = PowerMode::Active;
				break;
			}
			case (PowerMode::Active): {
				LSM6DSO32::update();
#if DV_OUT
				auto startUs = SysTick->VAL;
				auto startMs = util::getTime();
#endif
				mahony.updateIMU(LSM6DSO32::getAngularRates(), LSM6DSO32::getAccelerations(), 0.01f);

				Quaternion handleOrientation {mahony.getQuat()};
				auto       handleAngles {handleOrientation.toEuler()};

				for (uint8_t i {0}; i < 3; ++i) {
					data::usbSensorsResponse.accelerations[i] = LSM6DSO32::getRawAccelerations()[2 - i][0];
					data::usbSensorsResponse.angularRates[i] = LSM6DSO32::getRawAngularRates()[2 - i][0];
				}

				data::usbStatusResponse.pitch = handleAngles[1][0] * ATT_LSB;
				data::usbStatusResponse.roll = handleAngles[2][0] * ATT_LSB;

				float yawTarget {handleAngles[0][0] + yawOffset};

				float sy {sinf(yawOffset)};
				float cy {cosf(yawOffset)};

				float pitchTarget {
				  (gimbalMode == GimbalMode::Horizon ? 0 : cy * handleAngles[1][0] - sy * handleAngles[2][0]) + pitchOffset
				};
				float rollTarget {
				  (gimbalMode <= GimbalMode::Follow ? 0 : cy * handleAngles[2][0] + sy * handleAngles[1][0]) - rollOffset
				};

				yawCurrent +=
				    util::clamp(normalize(yawTarget - yawCurrent) / 50.0f, -maxRestoringVelocity, maxRestoringVelocity);
				yawCurrent = normalize(yawCurrent);

				pitchCurrent +=
				    util::clamp(normalize(pitchTarget - pitchCurrent) / 50.0f, -maxRestoringVelocity, maxRestoringVelocity);

				rollCurrent +=
				    util::clamp(normalize(rollTarget - rollCurrent) / 50.0f, -maxRestoringVelocity, maxRestoringVelocity);

				Quaternion cameraOrientation {Quaternion::fromEuler(yawCurrent, pitchCurrent, rollCurrent)};
				Quaternion gimbalRotation {handleOrientation.conjugate() * cameraOrientation};

				auto rotationAngles {gimbalRotation.toEuler()};
				auto calculatedAngles {calculateAngles(rotationAngles)};

				for (uint8_t i {0}; i < 3; ++i) {
					if (!std::isnan(calculatedAngles[i][0])) {
						motorAngles[i] = calculatedAngles[i][0];
					}
				}

#if DV_OUT
				Data data {};

				data.dt = (util::getTime() - startMs) * 1000 + (startUs - SysTick->VAL) / 48;
				data.x = rotationAngles[0][0];
				data.y = rotationAngles[1][0];
				data.z = rotationAngles[2][0];
				data.m1 = motorAngles[0];
				data.m2 = motorAngles[1];
				data.m3 = motorAngles[2];
				uart::sendToControl(reinterpret_cast<uint8_t*>(&data), sizeof(data));
#endif

				for (uint8_t i {0}; i < 3; ++i) {
					int16_t position = motorAngles[i] * attFactor;
					if (position < 0) {
						position += 4096;
					}

					motor::move(i + 1, position);
				}

				util::sleep(10);
				break;
			}
			case (PowerMode::Shutdown): {
				motor::move();
				motor::tone(motor::all, 294);
				util::sleep(205);
				motor::tone(motor::all, 247);
				util::sleep(205);
				motor::tone();
				util::sleep(10);  // TODO: remove, send while in sleep mode

				powerMode = PowerMode::Sleep;
				break;
			}
			case (PowerMode::Idle):
			default: {
				motor::sleep();
				__WFI();
				break;
			}
			case (PowerMode::Sleep): {
				motor::sleep();
				sleep();
				break;
			}
		}
	}

	return 1;
}
