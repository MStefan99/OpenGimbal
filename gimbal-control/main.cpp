#include "main.hpp"

volatile static DisplayState displayState {DisplayState::GimbalMode};
volatile static PowerMode    powerMode {PowerMode::Sleep};
volatile static GimbalMode   gimbalMode {GimbalMode::Follow};

constexpr static float    attFactor {2048 / F_PI};
constexpr static float    ATT_LSB {10430.0f};
constexpr static float    joystickFactor {F_DEG_TO_RAD * (F_PI / 100.0f)};
constexpr static float    sqrt2 = sqrtf(2);
constexpr static uint16_t deltaTime {1000 / updateRate};
constexpr static float    maxVelocityPerStep {maxRestoringVelocity / updateRate};

volatile static uint8_t  motorPositionRequest {0};
volatile static uint32_t motorRequestTime {0};

static float yawCurrent {0};
static float pitchCurrent {0};
static float rollCurrent {0};

volatile static float yawOffset {0};
volatile static float pitchOffset {0};
volatile static float rollOffset {0};

volatile static float yawReset {0};
volatile static float rollReset {0};

volatile static float motorAngles[3] {0};

volatile static uint32_t softStartTime {0};
volatile static bool     softStartActive {false};

volatile static uint32_t stateChangeTime {0};
volatile static uint8_t  voltageBars {0};

volatile static uint8_t  shortPresses {0};
volatile static uint32_t eventTime {0};
volatile static bool     buttonPressed {false};
volatile static bool     leftButton {true};

volatile static bool     usbPassthrough {false};
volatile static uint32_t usbPassthroughTime {0};

volatile static bool wokenByUSB {false};

float normalize(float difference) {
	if (difference > F_PI) {
		difference -= F_2_PI;
	} else if (difference < -F_PI) {
		difference += F_2_PI;
	}
	return difference;
}

// CAUTION: This function is called in an interrupt, no long-running operations allowed here!
void processMotorResponse(const uart::DefaultCallback::buffer_type& buffer) {
	if (usbPassthrough) {
		uint8_t buf[20];
		buf[0] = static_cast<uint8_t>(data::ResponseType::MotorPassthrough);
		util::copy(buf + 1, buffer.buffer, buffer.transferred);

		usb::write(buf, buffer.transferred + 1);

		usbPassthrough = false;
		return;
	}

	if ((buffer.buffer[0] & 0xf) != 0) {
		return;  // Command intended for another device
	}

	uint8_t srcAddress = (buffer.buffer[1] >> 4u);
	if (static_cast<MotorResponse::ResponseType>(buffer.buffer[1] & 0xf) == MotorResponse::ResponseType::ReturnVariable
	    && static_cast<MotorResponse::Variable>(buffer.buffer[2]) == MotorResponse::Variable::Position
	    && srcAddress < 4) {
		float angle = normalize(((buffer.buffer[3] << 8u) | buffer.buffer[4]) / attFactor);
		motorAngles[srcAddress - 1] = angle;
	}
}

// CAUTION: This function is called in an interrupt, no long-running operations allowed here!
void processUSBCommand(const usb::usb_device_endpoint1_request& request, uint16_t len) {
	switch (static_cast<data::CommandType>(request.bRequest)) {
		case (data::CommandType::GetVariable): {
			switch (request.bData[0]) {
				case static_cast<uint8_t>(data::VariableID::Status):
					usb::write(reinterpret_cast<uint8_t*>(&data::usbStatusResponse), sizeof(data::USBStatusResponse));
					break;
				case static_cast<uint8_t>(data::VariableID::Sensors):
					usb::write(reinterpret_cast<uint8_t*>(&data::usbSensorsResponse), sizeof(data::USBSensorsResponse));
					break;
				default:
					usb::write(nullptr, 0);
					break;
			}
			break;
			case (data::CommandType::MotorPassthrough): {
				usbPassthrough = true;
				usbPassthroughTime = util::getTime();
				motor::send(request.bData, len - 1);
				break;
			}
		}
		default:
			break;
	}
}

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

void setLEDs(uint32_t brightnesses) {
	for (uint8_t i {0}; i < 4; ++i) {
		pwm::setBrightness(i, brightnesses << 8u);
		brightnesses >>= 8u;
	}
}

void showMode() {
	if (powerMode == PowerMode::Idle || powerMode == PowerMode::Sleep) {
		setLEDs(0);
	} else {
		setLEDs(0xff << static_cast<uint8_t>(gimbalMode) * 8);
	}
}

// CAUTION: This function is called in an interrupt, no long-running operations allowed here!
void processButtons(bool left, bool pressed) {
	if (pressed && powerMode == PowerMode::Sleep) {
		powerMode = PowerMode::Idle;
	}

	if (!pressed && util::getTime() - eventTime < MAX_PRESS_WAIT_TIME) {
		++shortPresses;
	}

	eventTime = util::getTime();
	buttonPressed = pressed;
	leftButton = left;

	if (powerMode == PowerMode::Idle) {
		if (!buttonPressed) {
			showMode();
		} else {
			setLEDs(0xffffffff);
		}
	} else if (displayState == DisplayState::GimbalMode) {
		if (!buttonPressed) {
			showMode();
		} else {
			setLEDs(0);
		}
	}
}

bool triggerAction() {
	bool isOff {powerMode != PowerMode::Active};

	if (buttonPressed) {  // buttonPressed being true means this is a long press
		if (shortPresses == 1) {
			uint8_t led = (util::getTime() - (eventTime + MAX_SHORT_PRESS_TIME)) / LONG_PRESS_STEP_TIME;
			pwm::setBrightness(led - 1, isOff ? 0xffff : 0);

			if (!led) {
				setLEDs(isOff ? 0 : 0xffffffff);
			} else if (led > 3) {
				if (isOff) {
					uart::enable();
					displayState = DisplayState::GimbalMode;
					PORT_REGS->GROUP[0].PORT_OUTSET = 0x1 << 27u;
					joystick::updateCenter();
					LSM6DSO32::enable();
					powerMode = PowerMode::Active;

					softStartActive = true;
					softStartTime = util::getTime();

					motor::wake(motor::all);
					motorPositionRequest = 0;
					motorRequestTime = util::getTime();

				} else {
					powerMode = PowerMode::Sleep;
					PORT_REGS->GROUP[0].PORT_OUTCLR = 0x1 << 27u;
					joystick::saveValues();
					yawOffset = pitchOffset = rollOffset = yawReset = rollReset = 0;
					LSM6DSO32::disable();
				}
				showMode();

				return true;
			}
		} else {
			return true;
		}
	} else if (shortPresses == 1 && (!leftButton || isOff)) {
		adc::measureBattery([](uint16_t value) {
			if (powerMode == PowerMode::Sleep) {
				powerMode = PowerMode::Idle;
			}

			setLEDs(0);
			displayState = DisplayState::BatteryLevel;
			stateChangeTime = util::getTime();
			voltageBars = util::scale(
			                  util::clamp(value, MIN_VOLTAGE, MAX_VOLTAGE),
			                  MIN_VOLTAGE,
			                  MAX_VOLTAGE,
			                  static_cast<uint16_t>(0),
			                  static_cast<uint16_t>(65535)
			              ) / (65535 / 8)
			            + 1;
		});
	} else if (!isOff) {
		if (leftButton) {
			if (shortPresses == 1) {
				yawOffset = yawReset;
				pitchOffset = 0;
				rollOffset = rollReset;
			} else if (shortPresses == 2) {
				rollReset += F_PI_2;
				if (rollReset > F_PI + 0.05f) {
					rollReset = 0;
				}
				rollOffset = rollReset;
			} else if (shortPresses == 3) {
				yawReset += F_PI;
				if (yawReset > F_PI + 0.05f) {
					yawReset = 0;
				}
				yawOffset = yawReset;
			}
		} else {
			if (shortPresses == 2) {
				if (gimbalMode == GimbalMode::Tilt) {
					gimbalMode = GimbalMode::Horizon;
				} else {
					gimbalMode = static_cast<GimbalMode>(static_cast<uint8_t>(gimbalMode) + 1);
				}
				showMode();
			}
		}
	} else if (powerMode == PowerMode::Idle) {
		powerMode = PowerMode::Sleep;
	}

	return !buttonPressed;
}

void sleep() {
	while (uart::busy() || i2c::busy()) {
		__WFI();
	}
	uart::disable();
	PM_REGS->PM_SLEEPCFG = PM_SLEEPCFG_SLEEPMODE_STANDBY;
	while (PM_REGS->PM_SLEEPCFG != PM_SLEEPCFG_SLEEPMODE_STANDBY);
	do {
		__WFI();
	} while (powerMode == PowerMode::Sleep && !usb::isActive());
	PM_REGS->PM_SLEEPCFG = PM_SLEEPCFG_SLEEPMODE_IDLE;
	while (PM_REGS->PM_SLEEPCFG != PM_SLEEPCFG_SLEEPMODE_IDLE);
}

int main() {
	util::init();
	buttons::init();
	adc::init();
	pwm::init();
	i2c::init();
	uart::init();
	usb::init();

	uart::setCallback(processMotorResponse);
	buttons::setCallback(processButtons);
	usb::setCallback(processUSBCommand);

	PORT_REGS->GROUP[0].PORT_DIRSET = 0x1 << 27u;

	Mahony mahony {};

	while (1) {
		switch (powerMode) {
			case (PowerMode::Active): {
				LSM6DSO32::update();
				mahony.updateIMU(LSM6DSO32::getAngularRates(), LSM6DSO32::getAccelerations(), 0.01f);

				joystick::update([](int16_t x, int16_t y) {
					if (gimbalMode != GimbalMode::Tilt) {
						yawOffset -= y / 40000.0f;
					} else {
						rollOffset -= y / 40000.0f;
					}
					pitchOffset += x / 40000.0f;
				});

				Quaternion handleOrientation {mahony.getQuat() * Quaternion::fromEuler(0, controlBoardAngle, 0)};
				auto       handleAngles {handleOrientation.toEuler()};

				for (uint8_t i {0}; i < 3; ++i) {
					data::usbSensorsResponse.accelerations[i] = LSM6DSO32::getRawAccelerations()[2 - i][0];
					data::usbSensorsResponse.angularRates[i] = LSM6DSO32::getRawAngularRates()[2 - i][0];
				}

				data::usbStatusResponse.pitch = yawOffset * ATT_LSB;
				data::usbStatusResponse.roll = pitchOffset * ATT_LSB;

				float yawTarget {handleAngles[0][0] + yawOffset};

				float sy {sinf(yawOffset)};
				float cy {cosf(yawOffset)};

				float pitchTarget {
				  (gimbalMode == GimbalMode::Horizon ? 0 : cy * handleAngles[1][0] - sy * handleAngles[2][0]) + pitchOffset
				};
				float rollTarget {
				  (gimbalMode <= GimbalMode::Follow ? 0 : cy * handleAngles[2][0] + sy * handleAngles[1][0]) - rollOffset
				};

				yawCurrent += util::clamp(normalize(yawTarget - yawCurrent) / 50.0f, -maxVelocityPerStep, maxVelocityPerStep);
				yawCurrent = normalize(yawCurrent);

				pitchCurrent +=
				    util::clamp(normalize(pitchTarget - pitchCurrent) / 50.0f, -maxVelocityPerStep, maxVelocityPerStep);

				rollCurrent +=
				    util::clamp(normalize(rollTarget - rollCurrent) / 50.0f, -maxVelocityPerStep, maxVelocityPerStep);

				Quaternion cameraOrientation {Quaternion::fromEuler(yawCurrent, pitchCurrent, rollCurrent)};
				Quaternion gimbalRotation {handleOrientation.conjugate() * cameraOrientation};

				auto rotationAngles {gimbalRotation.toEuler()};
				auto calculatedAngles {calculateAngles(rotationAngles)};

				if (softStartActive) {
					for (uint8_t i {0}; i < motorPositionRequest; ++i) {
						motorAngles[i] += normalize(calculatedAngles[i][0] - motorAngles[i])
						                / static_cast<float>(softStartDuration - (util::getTime() - softStartTime)) * deltaTime;

						if (std::isnan(calculatedAngles[i][0])) {
							continue;
						}

						motor::move(i + 1, util::mod(static_cast<int16_t>(motorAngles[i] * attFactor), fullRevolution));

						if (util::getTime() - softStartTime >= softStartDuration) {
							softStartActive = false;
						}
					}

					if (motorPositionRequest < 3 && util::getTime() - motorRequestTime > MOTOR_REQUEST_TIMEOUT) {
						motor::getVariable(++motorPositionRequest, MotorCommand::Variable::Position);
						motorRequestTime = util::getTime();
					}
				} else {
					for (uint8_t i {0}; i < 3; ++i) {
						motorAngles[i] = calculatedAngles[i][0];

						if (std::isnan(calculatedAngles[i][0])) {
							continue;
						}

						motor::move(i + 1, util::mod(static_cast<int16_t>(motorAngles[i] * attFactor), fullRevolution));
					}
				}


				util::sleep(deltaTime);
				break;
			}
			case (PowerMode::Idle):
			default: {
				if (!usb::isActive() && wokenByUSB) {
					powerMode = PowerMode::Sleep;
					PORT_REGS->GROUP[0].PORT_OUTCLR = 0x1 << 27u;
					uart::disable();
					wokenByUSB = false;
				}
				__WFI();
				break;
			}
			case (PowerMode::Sleep): {
				if (usb::isActive()) {
					powerMode = PowerMode::Idle;
					PORT_REGS->GROUP[0].PORT_OUTSET = 0x1 << 27u;
					uart::enable();
					wokenByUSB = true;
					break;
				}
				uart::enable();
				motor::sleep();
				sleep();
				break;
			}
		}

		if (usb::isActive() && usbPassthrough && util::getTime() - usbPassthroughTime > USB_PASSTHROUGH_TIMEOUT) {
			uint8_t buf[1] {static_cast<uint8_t>(data::ResponseType::MotorPassthrough)};

			usb::write(buf, sizeof(buf));
			usbPassthrough = false;
		}

		if (displayState == DisplayState::BatteryLevel) {
			uint8_t step = (util::getTime() - stateChangeTime) / VOLTAGE_DISPLAY_TIME;
			uint8_t leds = voltageBars / 2;


			if (step > 15 || buttonPressed) {
				displayState = DisplayState::GimbalMode;
				showMode();
				if (powerMode == PowerMode::Idle) {
					powerMode = PowerMode::Sleep;
				}
			} else if (step >= leds) {
				// The calculation steps + leds + 1 is just to ensure the same timing for all LEDs
				pwm::setBrightness(leds, voltageBars % 2 && (step + leds + 1) % 2 ? 0xffff : 0);
			} else {
				pwm::setBrightness(step, leds ? 0xffff : 0);
			}
		}

		if (buttonPressed && util::getTime() - eventTime > MAX_LONG_PRESS_TIME) {
			shortPresses = 0;
			showMode();
			if (powerMode == PowerMode::Idle) {
				powerMode = PowerMode::Sleep;
			}
		}

		if ((buttonPressed && util::getTime() - eventTime > MAX_SHORT_PRESS_TIME)
		    || (shortPresses && util::getTime() - eventTime > MAX_PRESS_WAIT_TIME)) {
			if (triggerAction()) {
				shortPresses = 0;
			}
		}
	}

	return 1;
}
