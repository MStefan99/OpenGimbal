#include "main.hpp"

static PowerMode  powerMode {PowerMode::Sleep};
static GimbalMode gimbalMode {GimbalMode::Follow};

constexpr static float attFactor {2048 / F_PI};
constexpr static float maxRestoringVelocity {F_2_PI / 100.0f};  // One revolution per second (100 iterations)

static float yawTarget {0};
static float pitchTarget {0};
static float rollTarget {0};

static float yawOffset {0};
static float pitchOffset {0};
static float rollOffset {0};

constexpr float sqrt2 = sqrtf(2);

// CAUTION: This function is called in an interrupt, no long-running operations allowed here!
void processControlCommand(const uart::DefaultCallback::buffer_type& buffer) {
	switch (static_cast<ControlCommand::CommandType>(buffer.buffer[0] & 0x0f)) {
		case (ControlCommand::CommandType::Move): {
			yawOffset = static_cast<int16_t>((buffer.buffer[1] << 8u) | buffer.buffer[2]);
			pitchOffset = static_cast<int16_t>((buffer.buffer[3] << 8u) | buffer.buffer[4]);
			break;
		}
		case (ControlCommand::CommandType::Calibrate): {
			// TODO: start calibration
			break;
		}
		case (ControlCommand::CommandType::GetVariable): {
			switch (static_cast<ControlCommand::Variable>(buffer.buffer[1])) {  // Switch variable
				case (ControlCommand::Variable::PowerMode): {
					auto response = ReturnVariableResponse(ControlResponse::Variable::PowerMode, static_cast<uint8_t>(powerMode));
					uart::sendToControl(response.getBuffer(), response.getLength());
					break;
				}
				case (ControlCommand::Variable::GimbalMode): {
					auto response =
					    ReturnVariableResponse(ControlResponse::Variable::GimbalMode, static_cast<uint8_t>(gimbalMode));
					uart::sendToControl(response.getBuffer(), response.getLength());
					break;
				}
				case (ControlCommand::Variable::BatteryVoltage): {
					adc::measureBattery([](uint16_t voltage) {
						uint8_t scaledVoltage = static_cast<uint8_t>(util::scale(
						    util::clamp(voltage, MIN_VOLTAGE, MAX_VOLTAGE),
						    MIN_VOLTAGE,
						    MAX_VOLTAGE,
						    static_cast<uint16_t>(0),
						    static_cast<uint16_t>(255)
						));
						auto    response = ReturnVariableResponse(ControlResponse::Variable::BatteryVoltage, scaledVoltage);
						uart::sendToControl(response.getBuffer(), response.getLength());
					});
					break;
				}
				case (ControlCommand::Variable::YawOffset): {
					auto response =
					    ReturnVariableResponse(ControlResponse::Variable::YawOffset, static_cast<int16_t>(yawOffset * attFactor));
					uart::sendToControl(response.getBuffer(), response.getLength());
					break;
				}
				case (ControlCommand::Variable::PitchOffset): {
					auto response = ReturnVariableResponse(
					    ControlResponse::Variable::PitchOffset,
					    static_cast<int16_t>(pitchOffset * attFactor)
					);
					uart::sendToControl(response.getBuffer(), response.getLength());
					break;
				}
				case (ControlCommand::Variable::RollOffset): {
					auto response = ReturnVariableResponse(
					    ControlResponse::Variable::RollOffset,
					    static_cast<int16_t>(rollOffset * attFactor)
					);
					uart::sendToControl(response.getBuffer(), response.getLength());
					break;
				}
			}
			break;
		}
		case (ControlCommand::CommandType::SetVariable): {
			switch (static_cast<ControlCommand::Variable>(buffer.buffer[1])) {  // Switch variable
				case (ControlCommand::Variable::PowerMode): {
					powerMode = static_cast<PowerMode>(buffer.buffer[2]);
					break;
				}
				case (ControlCommand::Variable::GimbalMode): {
					gimbalMode = static_cast<GimbalMode>(buffer.buffer[2]);
					break;
				}
				case (ControlCommand::Variable::YawOffset): {
					yawOffset = ((buffer.buffer[2] << 8u) | buffer.buffer[3]) / attFactor;
					break;
				}
				case (ControlCommand::Variable::PitchOffset): {
					pitchOffset = ((buffer.buffer[2] << 8u) | buffer.buffer[3]) / attFactor;
					break;
				}
				case (ControlCommand::Variable::RollOffset): {
					rollOffset = ((buffer.buffer[2] << 8u) | buffer.buffer[3]) / attFactor;
					break;
				}
			}
			break;
		}
	}
}

Vector3<float, uint8_t> calculateAngles(const Vector3<float, uint8_t>& eulerAngles) {
	float a = eulerAngles[0][0];
	float b = util::clamp(eulerAngles[1][0], -F_PI_4 + 1e-3f, F_PI_4 - 1e-3f);
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
	float sin2M2 = sinM2 * sinM2;

	float cos2M2 = 1 - sin2M2;
	float cosM2 = sqrtf(cos2M2);

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

float normalize(float difference) {
	if (difference > F_PI) {
		return difference - F_2_PI;
	} else if (difference < -F_PI) {
		return difference + F_2_PI;
	} else {
		return difference;
	}
}

void sleep() {  // TODO: disable IMU
	motor::sleep();
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
	dma::init();
	i2c::init();
	LSM6DSO32::init();
	uart::init();
	adc::init();

	uart::setControlCallback(processControlCommand);

	PORT_REGS->GROUP[0].PORT_DIRSET = (0x1 << 17u);

	Mahony mahony {};

	float yawTarget {0};
	float pitchTarget {0};

	while (1) {
		switch (powerMode) {
			case (PowerMode::Active): {
				LSM6DSO32::update();
#if DV_OUT
				auto startUs = SysTick->VAL;
				auto startMs = util::getTime();
#endif
				mahony.updateIMU(LSM6DSO32::getRot(), LSM6DSO32::getAcc(), 0.01f);

				Quaternion handleOrientation {mahony.getQuat()};
				auto       handleAngles {handleOrientation.toEuler()};

				yawTarget += util::clamp(
				    normalize(handleAngles[0][0] - yawTarget) / 100.0f,
				    -maxRestoringVelocity,
				    maxRestoringVelocity
				);
				pitchTarget += util::clamp(
				    normalize(handleAngles[1][0] - pitchTarget) / 100.0f,
				    -maxRestoringVelocity,
				    maxRestoringVelocity
				);
				yawTarget = normalize(yawTarget);

				Quaternion phoneOrientation {Quaternion::fromEuler(yawTarget, pitchTarget, 0)};
				Quaternion gimbalRotation {handleOrientation.conjugate() * phoneOrientation};

				auto eulerAngles {gimbalRotation.toEuler()};
				auto motorAngles {calculateAngles(eulerAngles)};

#if DV_OUT
				Data data {};

				data.dt = (util::getTime() - startMs) * 1000 + (startUs - SysTick->VAL) / 48;
				data.x = eulerAngles[0][0];
				data.y = eulerAngles[1][0];
				data.z = eulerAngles[2][0];
				data.m1 = motorAngles[0][0];
				data.m2 = motorAngles[1][0];
				data.m3 = motorAngles[2][0];
				uart::sendToControl(reinterpret_cast<uint8_t*>(&data), sizeof(data));
#endif

				for (uint8_t i {0}; i < 3; ++i) {
					int16_t position = motorAngles[i][0] * attFactor;
					if (position < 0) {
						position += 4096;
					}

					motor::move(i + 1, position);
				}

				util::sleep(10);
				break;
			}
			case (PowerMode::Idle):
			default: {
				motor::sleep();
				__WFI();
				break;
			}
			case (PowerMode::Sleep): {
				sleep();
				break;
			}
		}
	}

	return 1;
}
