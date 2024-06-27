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
void processControlCommand(const uart::DefaultCallback::buffer_type& buffer) {
	switch (static_cast<ControlCommand::CommandType>(buffer.buffer[0] & 0x0f)) {
		case (ControlCommand::CommandType::GetVariable): {
			switch (static_cast<ControlCommand::Variable>(buffer.buffer[1])) {  // Switch variable
				case (ControlCommand::Variable::PowerMode): {
					auto response = ReturnVariableResponse(ControlResponse::Variable::PowerMode, static_cast<uint8_t>(powerMode));
					uart::sendToControl(response.getBuffer(), response.getLength());
					break;
				}
				case (ControlCommand::Variable::GimbalMode): {
					ControlResponse::PowerMode responseMode;

					switch (powerMode) {
						case (PowerMode::Sleep): {
							responseMode = ControlResponse::PowerMode::Sleep;
							break;
						}
						case (PowerMode::Idle): {
							responseMode = ControlResponse::PowerMode::Idle;
							break;
						}
						default: {
							responseMode = ControlResponse::PowerMode::Active;
							break;
						}
					}

					auto response =
					    ReturnVariableResponse(ControlResponse::Variable::GimbalMode, static_cast<uint8_t>(responseMode));
					uart::sendToControl(response.getBuffer(), response.getLength());
					break;
				}
				case (ControlCommand::Variable::BatteryVoltage): {
					adc::measureBattery([](uint16_t voltage) {
						auto response = ReturnVariableResponse(
						    ControlResponse::Variable::BatteryVoltage,
						    static_cast<uint8_t>(util::scale(
						        util::clamp(voltage, MIN_VOLTAGE, MAX_VOLTAGE),
						        MIN_VOLTAGE,
						        MAX_VOLTAGE,
						        static_cast<uint16_t>(0),
						        static_cast<uint16_t>(255)
						    ))
						);
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
					switch (static_cast<ControlCommand::PowerMode>(buffer.buffer[2])) {
						case (ControlCommand::PowerMode::Sleep): {
							powerMode = PowerMode::Sleep;
							break;
						}
						case (ControlCommand::PowerMode::Idle): {
							powerMode = PowerMode::Idle;
							break;
						}
						default:
							powerMode = PowerMode::Startup;
							break;
					}
					break;
				}
				case (ControlCommand::Variable::GimbalMode): {
					gimbalMode = static_cast<GimbalMode>(buffer.buffer[2]);
					break;
				}
				case (ControlCommand::Variable::YawOffset): {
					yawOffset = static_cast<int16_t>((buffer.buffer[2] << 8u) | buffer.buffer[3]) / attFactor;
					break;
				}
				case (ControlCommand::Variable::PitchOffset): {
					pitchOffset = static_cast<int16_t>((buffer.buffer[2] << 8u) | buffer.buffer[3]) / attFactor;
					break;
				}
				case (ControlCommand::Variable::RollOffset): {
					rollOffset = static_cast<int16_t>((buffer.buffer[2] << 8u) | buffer.buffer[3]) / attFactor;
					break;
				}
				default:
					break;
			}
			break;
		}
		default:
			break;
	}
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
	dma::init();
	i2c::init();
	LSM6DSO32::init();
	uart::init();
	adc::init();

	uart::setControlCallback(processControlCommand);
	uart::setMotorCallback(processMotorResponse);

	PORT_REGS->GROUP[0].PORT_DIRSET = (0x1 << 17u);

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
				mahony.updateIMU(LSM6DSO32::getRot(), LSM6DSO32::getAcc(), 0.01f);

				Quaternion handleOrientation {mahony.getQuat()};
				auto       handleAngles {handleOrientation.toEuler()};

				if (gimbalMode == GimbalMode::Follow) {
					handleAngles[2][0] = 0;  // Do not follow handle roll
				}
				if (gimbalMode == GimbalMode::Horizon) {
					handleAngles[1][0] = handleAngles[2][0] = 0;  // Do not follow handle pitch or roll
				}

				yawTarget += util::clamp(
				    normalize(handleAngles[0][0] - (yawTarget + yawOffset)) / 50.0f,
				    -maxRestoringVelocity,
				    maxRestoringVelocity
				);
				yawTarget = normalize(yawTarget);

				pitchTarget += util::clamp(
				    normalize(handleAngles[1][0] - (pitchTarget + pitchOffset)) / 50.0f,
				    -maxRestoringVelocity,
				    maxRestoringVelocity
				);

				rollTarget += util::clamp(
				    normalize(handleAngles[2][0] - (rollTarget + rollOffset)) / 50.0f,
				    -maxRestoringVelocity,
				    maxRestoringVelocity
				);

				Quaternion cameraOrientation {Quaternion::fromEuler(yawTarget, pitchTarget, rollTarget)};
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
