#include "main.hpp"


static uint8_t            maxTorque {0};
constexpr static uint16_t halfRevolution {fullRevolution / 2};
constexpr static uint16_t quarterRevolution {fullRevolution / 4};

static MovementController movementController {};
static Mode               mode {Mode::Sleep};
static uint8_t            calibrationMode = nvm::options->polePairs ? 0 : 3;
static uint32_t           lastTargetTime {0};
static bool               poweredDown {mode == Mode::Sleep || mode == Mode::Idle};

static bool offsetAdjusted {false};

static uint16_t      currentAngle {0};
static float         prevDAngle {0.0f};
static bool          angleReady {false};
static LowPassFilter torqueFilter {1000, 2};

volatile static uint32_t hapticEnd {0};
volatile static uint16_t hapticPower {0};
static uint16_t          hapticDuration {0};
static bool              hapticDirection {false};

volatile static bool     wokenByPin {false};
volatile static uint32_t wakeTime {0};

struct TorqueLUT {
	uint16_t table[16];

	constexpr TorqueLUT():
	  table() {
		table[0] = 0;
		auto factor = (255.0f - idleTorque) / 15.0f;
		for (auto i = 1; i < 16; ++i) {
			table[i] = std::floor(factor * i + idleTorque);
		}
	}
};

constexpr static auto torqueLUT = TorqueLUT();

void powerDown() {
	if (poweredDown) {
		return;
	}
	PORT_REGS->GROUP[0].PORT_OUTCLR = 0x1 << 27u;
	bldc::removeTorque();
	angleReady = false;
	poweredDown = true;
}

void powerUp() {
	if (!poweredDown) {
		return;
	}
	PORT_REGS->GROUP[0].PORT_OUTSET = 0x1 << 27u;
	AS5600::init();
	poweredDown = false;
}

// CAUTION: This function is called in an interrupt, no long-running operations allowed here!
void processWakeup() {
	uart::enable();
	mode = Mode::Idle;
	wokenByPin = true;
	wakeTime = util::getTime();
}

// CAUTION: This function is called in an interrupt, no long-running operations allowed here!
void processCommand(const uart::DefaultCallback::buffer_type& buffer) {
	uint8_t address = buffer.buffer[0] & 0x0f;
	if (address != deviceAddress && address != 0xf) {
		return;  // Command intended for another device
	}

	switch (static_cast<Command::CommandType>(buffer.buffer[1] & 0x0f)) {  // Switch command type
		case (Command::CommandType::Sleep): {
			mode = Mode::Sleep;
			maxTorque = 0;
			powerDown();
			uart::disable();
			eic::enable();
			break;
		}
		case (Command::CommandType::Idle): {
			mode = Mode::Idle;
			maxTorque = 0;
			powerDown();
			break;
		}
		case (Command::CommandType::Wake): {
			mode = Mode::Idle;
			powerUp();
			lastTargetTime = 0;
			break;
		}
		case (Command::CommandType::Move): {
			mode = Mode::Drive;
			powerUp();
			auto time {util::getTime()};
			movementController.extrapolate(time - lastTargetTime, ((buffer.buffer[2] & 0x0f) << 8u) | buffer.buffer[3]);
			if (offsetAdjusted) {
				offsetAdjusted = false;
				prevDAngle = 0.0f;
			}
			lastTargetTime = time;
			maxTorque = torqueLUT.table[buffer.buffer[2] >> 4u];
			break;
		}
		case (Command::CommandType::Tone): {
			bldc::tone((buffer.buffer[2] << 8u) | buffer.buffer[3]);
			break;
		}
		case (Command::CommandType::Haptic): {
			mode = Mode::Haptic;
			powerUp();
			hapticPower = torqueLUT.table[buffer.buffer[2] >> 4u];
			hapticEnd = util::getTime() + (((buffer.buffer[2] & 0x0f) << 8u) | buffer.buffer[3]);
			break;
		}
		case (Command::CommandType::AdjustOffset): {
			if (mode != Mode::Sleep && mode != Mode::Idle) {
				movementController.adjustOffset(currentAngle, ((buffer.buffer[2] & 0x0f) << 8u) | buffer.buffer[3]);
				uint16_t offset = movementController.getOffset();
				offsetAdjusted = true;
				nvm::edit(&nvm::options->zeroOffset, offset);
				nvm::write();
			}
			break;
		}
		case (Command::CommandType::Calibrate): {
			mode = Mode::Calibrate;
			powerUp();
			calibrationMode = buffer.buffer[2];
			break;
		}
		case (Command::CommandType::GetVariable): {
			if (address == 0xf) {  // Get variable commands cannot be issued to all devices at once
				break;
			}
			switch (static_cast<Command::Variable>(buffer.buffer[2])) {  // Switch variable
				case (Command::Variable::Calibration): {
					auto response = ReturnVariableResponse(
					    deviceAddress,
					    buffer.buffer[1] >> 8u,
					    Command::Variable::Calibration,
					    static_cast<uint8_t>((!!nvm::options->polePairs << 1u) | (!!nvm::options->phaseOffset))
					);
					uart::send(response.getBuffer(), response.getLength());
					break;
				}
				case (Command::Variable::Offset): {
					auto response = ReturnVariableResponse(
					    deviceAddress,
					    buffer.buffer[1] >> 8u,
					    Command::Variable::Offset,
					    static_cast<uint16_t>(movementController.getOffset())
					);
					uart::send(response.getBuffer(), response.getLength());
					break;
				}
				case (Command::Variable::Position): {
					auto response = ReturnVariableResponse(
					    deviceAddress,
					    buffer.buffer[1] >> 8u,
					    Command::Variable::Position,
					    static_cast<int16_t>(util::mod(
					        static_cast<int32_t>(currentAngle - movementController.getOffset()),
					        static_cast<int32_t>(fullRevolution)
					    ))
					);
					uart::send(response.getBuffer(), response.getLength());
					break;
				}
				case (Command::Variable::Speed): {
					auto response = ReturnVariableResponse(
					    deviceAddress,
					    buffer.buffer[1] >> 8u,
					    Command::Variable::Speed,
					    movementController.getMaxSpeed()
					);
					uart::send(response.getBuffer(), response.getLength());
				}
				default:
					break;
			}
			break;
		}
		case (Command::CommandType::SetVariable): {
			switch (static_cast<Command::Variable>(buffer.buffer[2])) {  // Switch variable
				case (Command::Variable::Offset): {
					movementController.setOffset((buffer.buffer[3] << 8u) | buffer.buffer[4]);
					uint16_t offset = movementController.getOffset();
					nvm::edit(&nvm::options->zeroOffset, offset);
					nvm::write();
					break;
				}
				case (Command::Variable::Speed): {
					uint8_t maxSpeed = buffer.buffer[3];
					movementController.setMaxSpeed(maxSpeed);
					nvm::edit(&nvm::options->maxSpeed, maxSpeed);
					nvm::write();
				}
				default:
					break;
			}
			break;
		}
	}
}

float countsToRad(int16_t counts) {
	return static_cast<float>(counts) / fullRevolution * F_2_PI;
}

uint16_t measureAngle() {
	static uint16_t rawAngle;

	angleReady = false;
	auto startTime {util::getTime()};

	AS5600::getAngle([](bool success, const i2c::Transfer& transfer) {
		rawAngle = reinterpret_cast<const uint16_t*>(transfer.buf)[0];
		angleReady = true;
	});

	while (!angleReady && util::getTime() - startTime < 5) {
		__WFI();
	}

	return (currentAngle = util::switchEndianness(rawAngle));
}

int16_t normalize(int16_t difference) {
	difference %= fullRevolution;

	if (difference > halfRevolution) {
		return difference - fullRevolution;
	} else if (difference < -halfRevolution) {
		return difference + fullRevolution;
	} else {
		return difference;
	}
}

void applyTorque(uint16_t angle, uint8_t power, bool counterclockwise = true) {
	// Calculate electrical angle from encoder reading
	uint16_t eAngle = (nvm::options->polePairs * (fullRevolution + angle - nvm::options->phaseOffset)) % fullRevolution;
	// Flip the angle if the motor polarity is reversed
	uint16_t eAngleSigned = nvm::options->counterclockwise ? eAngle : fullRevolution - eAngle;

	bldc::applyTorque(
	    counterclockwise == nvm::options->counterclockwise ? eAngleSigned + quarterRevolution
	                                                       : eAngleSigned + (fullRevolution - quarterRevolution),
	    power
	);
}

void moveToTarget(uint16_t target) {
	// Calculating difference between current and target angle
	auto angle {measureAngle()};

	float dAngle {countsToRad(normalize(target - angle))};
	float velocity {(dAngle - prevDAngle) * 1000.0f};
	prevDAngle = dAngle;

	// Calculating and applying torque
	float torque = torqueFilter.process(dAngle * K[0][0] + velocity * K[0][1]);
	float absTorque = util::min(
	    static_cast<uint16_t>(util::abs(torque) + util::min(idleTorque, maxTorque)),
	    static_cast<uint16_t>(maxTorque)
	);

	applyTorque(angle, absTorque, torque > 0);
}

void unwind(int16_t offset) {
	for (; offset > 0; offset -= 8) {
		bldc::applyTorque(offset, 255);
		util::sleep(1);
	}
	bldc::applyTorque(0, 0);
}

int16_t checkCalibration(uint16_t phaseOffset, uint8_t polePairs, bool counterclockwise) {
	uint16_t step {fullRevolution / 4096};
	auto     startAngle {measureAngle()};

	for (uint16_t i {0}; i < fullRevolution; ++i) {
		uint16_t target =
		    util::mod(static_cast<uint16_t>((counterclockwise ? -step * i : step * i) + startAngle), fullRevolution);

		auto angle {measureAngle()};

		// Simplified version of applyTorque()
		uint16_t eAngle = (polePairs * (fullRevolution + angle - phaseOffset)) % fullRevolution;
		uint16_t eAngleSigned = counterclockwise ? eAngle : fullRevolution - eAngle;

		int16_t  difference = normalize(target - angle);
		uint16_t absDifference = util::abs(difference);
		bldc::applyTorque(
		    (difference > 0) == counterclockwise ? eAngleSigned + quarterRevolution
		                                         : eAngleSigned + (fullRevolution - quarterRevolution),
		    util::min(absDifference + idleTorque, 255)
		);

		if (absDifference > (fullRevolution / 32)) {
			return target * polePairs + phaseOffset;
		}

		util::sleep(1);
	}

	return 0;
}

bool calibrate() {
	uint16_t angle {0};
	uint16_t phaseOffset {nvm::options->phaseOffset};
	bldc::applyTorque(0, 255);
	uint16_t timeout {0};

	if (calibrationMode) {
		uint16_t samples {0};
		/* Phase offset calibration
		 *
		 * The purpose of this step is to determine the angle between the
		 * magnet's north pole (when encoder reads zero) and any of the motor phases
		 * (when the motor's electrical angle is zero).
		 *
		 * This is achieved by moving the motor into the zero position
		 * and reading the encoder value.
		 */
		do {
			util::sleep(20);
			phaseOffset = angle;
			angle = measureAngle();

			if (phaseOffset == angle) {
				++samples;
			} else {
				samples = 0;
			}

			if (timeout++ > 100) {  // Quit after a while to avoid overheating the motor
				bldc::applyTorque(0, 0);
				return false;
			}
		} while (samples < 10);  // Wait for a while to ensure the motor has settled
		nvm::edit(&nvm::options->phaseOffset, phaseOffset);
	}

	uint16_t torqueAngle {0};

	if (calibrationMode & (1 << static_cast<uint8_t>(Command::CalibrationMode::Pole))) {
		uint8_t  polePairs {0};
		uint16_t lastPoleAngle {angle};
		int8_t   direction {0};
		uint16_t checkpoint {angle};
		timeout = 0;

		/* Pole pair calibration
		 *
		 * The purpose of this step is to determine how many pole pairs the motor has.
		 *
		 * The motor is rotated through one full revolution and the zero phase
		 * passes are counted (how many times the motor passes through the
		 * zero electrical angle)
		 */
		do {
			torqueAngle += 8;

			if (torqueAngle >= fullRevolution) {  // One full electrical revolution, starting another one
				torqueAngle = 0;
				if (angle > lastPoleAngle) {
					++direction;
				} else {
					--direction;
				}
				lastPoleAngle = angle;
				++polePairs;
			}

			bldc::applyTorque(torqueAngle, 255);
			angle = measureAngle();

			if (util::abs(normalize(angle - checkpoint)) > fullRevolution / 32) {
				checkpoint = angle;
				timeout = 0;
			}

			if (timeout++ > 200) {  // Quit if the motor is stuck to avoid overheating
				bldc::tone(294);
				util::sleep(205);
				bldc::tone(247);
				util::sleep(205);
				bldc::silent();
				unwind(polePairs * fullRevolution + torqueAngle);
				return false;
			}

			util::sleep(1);
		} while (util::abs(angle - phaseOffset) > 10 || !polePairs);

		// Adding the last pole if we're close enough
		polePairs += torqueAngle / halfRevolution;

		/* Calibration check
		 *
		 * The purpose of this step is to determine if calibration
		 * is successful and it can be saved
		 *
		 * During the check the motor has to track a moving target. If the
		 * motor deviates from the target too much, the check is failed.
		 *
		 * The target moves one full revolution in the opposite direction
		 * than that of pole pair calibration step to untangle the wires.
		 */
		auto calibrationCheck {checkCalibration(phaseOffset, polePairs, direction > 0)};
		if (calibrationCheck) {
			bldc::tone(294);
			util::sleep(205);
			bldc::tone(247);
			util::sleep(205);
			bldc::silent();
			unwind(calibrationCheck);
			return false;
		}

		// Saving calibration
		nvm::edit(&nvm::options->polePairs, polePairs);
		nvm::edit(&nvm::options->counterclockwise, direction > 0);
	}

	if (calibrationMode) {
		nvm::write();
	}

	bldc::applyTorque(0, 0);
	return true;
}

void sleep() {
	PM_REGS->PM_SLEEPCFG = PM_SLEEPCFG_SLEEPMODE_STANDBY;
	while (PM_REGS->PM_SLEEPCFG != PM_SLEEPCFG_SLEEPMODE_STANDBY);
	do {
		__WFI();
	} while (mode == Mode::Sleep);
	PM_REGS->PM_SLEEPCFG = PM_SLEEPCFG_SLEEPMODE_IDLE;
	while (PM_REGS->PM_SLEEPCFG != PM_SLEEPCFG_SLEEPMODE_IDLE);
}

int main() {
	util::init();
	uart::init();
	eic::init();
	eic::enable();
	i2c::init();
	bldc::init();
	bldc::enable();

	PORT_REGS->GROUP[0].PORT_DIRSET = 0x1 << 27u;

	uart::setCallback(processCommand);
	eic::setCallback(processWakeup);

	if (mode == Mode::Sleep || mode == Mode::Idle) {
		powerDown();
	}

	while (1) {
		switch (mode) {
			case (Mode::Sleep): {
				sleep();
				break;
			}
			case (Mode::Calibrate): {
				if (calibrate()) {
					mode = Mode::Drive;
				} else {
					mode = Mode::Idle;
					maxTorque = 0;
				}
				break;
			}
			case (Mode::Drive): {
				if (!nvm::options->polePairs) {
					bldc::applyTorque(0, 0);
					mode = Mode::Idle;
					continue;
				}

				auto time {util::getTime()};
				movementController.interpolate(time - lastTargetTime);
				moveToTarget(movementController.getTarget());

				util::sleep(1);  // Waiting until next tick
				break;
			}
			case (Mode::Haptic): {
				if (hapticDuration++ > hapticCycleDuration) {
					hapticDuration = 0;
					hapticDirection = !hapticDirection;
				}

				applyTorque(measureAngle(), hapticPower, hapticDirection);

				if (util::getTime() > hapticEnd) {
					mode = Mode::Drive;
					hapticDuration = 0;
				}

				util::sleep(1);  // Waiting until next tick
				break;
			}
			case (Mode::Idle): {
				if (wokenByPin && util::getTime() - wakeTime > 20) {
					mode = Mode::Sleep;
					uart::disable();
					eic::enable();
				}

				if (!poweredDown) {
					measureAngle();
				}
				util::sleep(1);
				break;
			}
			default: {
				__WFI();
				break;
			}
		}
	}

	return 1;
}
