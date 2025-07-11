#include "main.hpp"


volatile static DisplayState displayState {DisplayState::GimbalMode};
volatile static PowerMode    powerMode {PowerMode::Sleep};
volatile static GimbalMode   gimbalMode {GimbalMode::Follow};

constexpr static float    motorScalingFactor {2048 / F_PI};
constexpr static float    intScalingFactor {32768 / F_PI};
constexpr static float    ATT_LSB {10430.0f};
constexpr static float    joystickFactor {F_DEG_TO_RAD * (F_PI / 100.0f)};
constexpr static float    sqrt2 = sqrtf(2);
constexpr static uint16_t deltaTime {1000 / updateRate};
constexpr static float    maxVelocityPerStep {maxRestoringVelocity / updateRate};

static Mahony mahony {};

volatile static bool sensorReady {false};
volatile static bool sensorDataReady {false};

volatile static uint8_t  motorPositionRequest {0};
volatile static uint32_t motorRequestTime {0};

static float          currentAngles[3] {0};
volatile static float offsetAngles[3] {0};
volatile static float resetAngles[3] {0};
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

void showBattery(uint16_t value) {
	setLEDs(0);
	stateChangeTime = util::getTime();
	displayState = DisplayState::BatteryLevel;
	voltageBars = util::scale(
	                  util::clamp(value, MIN_VOLTAGE, MAX_VOLTAGE),
	                  MIN_VOLTAGE,
	                  MAX_VOLTAGE,
	                  static_cast<uint16_t>(0),
	                  static_cast<uint16_t>(65535)
	              ) / (65535 / 8)
	            + 1;
}

void enable() {
	adc::measureBattery([](uint16_t value) {
		if (value > MIN_VOLTAGE) {
			uart::enable();
			displayState = DisplayState::GimbalMode;
			PORT_REGS->GROUP[0].PORT_OUTSET = 0x1 << 27u;
			joystick::updateCenter();
			LSM6DSO32::enable();
			powerMode = PowerMode::Active;

			softStartActive = true;
			softStartTime = util::getTime();

			motor::wake();
			motor::tone();  // In case tone was previously set
			motorPositionRequest = 0;
			motorRequestTime = util::getTime();
			showMode();

			WDT_REGS->WDT_CTRLA = WDT_CTRLA_ENABLE(1);
		} else {
			showBattery(value);
		}
	});
}

void disable() {
	powerMode = PowerMode::Sleep;
	PORT_REGS->GROUP[0].PORT_OUTCLR = 0x1 << 27u;
	joystick::saveValues();
	offsetAngles[0] = offsetAngles[1] = offsetAngles[2] = resetAngles[0] = resetAngles[2] = 0;
	LSM6DSO32::disable();
	showMode();
}

// CAUTION: This function is called in an interrupt, no long-running operations allowed here!
void processMotorResponse(const uart::DefaultCallback::buffer_type& buffer) {
	if (usbPassthrough) {
		uint8_t buf[20];
		buf[0] = static_cast<uint8_t>(USBResponse::ResponseType::MotorPassthrough);
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
		float angle = static_cast<uint16_t>(((buffer.buffer[3] & 0xf) << 8u) | buffer.buffer[4]) / motorScalingFactor;
		motorAngles[srcAddress - 1] = angle;
	}
}

// CAUTION: This function is called in an interrupt, no long-running operations allowed here!
void processUSBCommand(const usb::usb_device_endpoint1_request& request, uint16_t len) {
	switch (static_cast<USBCommand::CommandType>(request.bRequest)) {
		case (USBCommand::CommandType::Enable): {
			enable();
			break;
		}
		case (USBCommand::CommandType::Disable): {
			disable();
			break;
		}
		case (USBCommand::CommandType::GetVariable): {
			switch (static_cast<USBCommand::Variable>(request.bData[0])) {
				case (USBCommand::Variable::Orientation): {
					int16_t data[3];
					for (uint8_t i {0}; i < 3; ++i) {
						data[i] = __REV16(static_cast<int16_t>(offsetAngles[i] * intScalingFactor));
					}

					auto response = USBReturnVariableResponse {
					  USBResponse::Variable::Orientation,
					  reinterpret_cast<uint8_t*>(data),
					  sizeof(data)
					};
					usb::write(response.getBuffer(), response.getLength());
					break;
				}
				case (USBCommand::Variable::HandleOrientation): {
					int16_t    data[3];
					Quaternion handleOrientation {mahony.getQuat() * Quaternion::fromEuler(0, controlBoardAngle, 0)};
					auto       handleAngles {handleOrientation.toEuler()};

					for (uint8_t i {0}; i < 3; ++i) {
						data[i] = __REV16(static_cast<int16_t>(handleAngles[i][0] * intScalingFactor));
					}

					auto response = USBReturnVariableResponse {
					  USBResponse::Variable::HandleOrientation,
					  reinterpret_cast<uint8_t*>(data),
					  sizeof(data)
					};
					usb::write(response.getBuffer(), response.getLength());
					break;
				}
				case (USBCommand::Variable::Mode): {
					auto response = USBReturnVariableResponse {USBResponse::Variable::Mode, static_cast<uint8_t>(gimbalMode)};
					usb::write(response.getBuffer(), response.getLength());
					break;
				}
				case (USBCommand::Variable::BatteryVoltage): {
					adc::measureBattery([](uint16_t value) {
						uint8_t voltage = util::scale(
						    util::clamp(value, MIN_VOLTAGE, MAX_VOLTAGE),
						    MIN_VOLTAGE,
						    MAX_VOLTAGE,
						    static_cast<uint16_t>(0),
						    static_cast<uint16_t>(255)
						);

						auto response = USBReturnVariableResponse {USBResponse::Variable::BatteryVoltage, voltage};
						usb::write(response.getBuffer(), response.getLength());
					});
					break;
				}
				default:
					break;
			}
			break;
		}
		case (USBCommand::CommandType::SetVariable): {
			switch (static_cast<USBCommand::Variable>(request.bData[0])) {
				case (USBCommand::Variable::Orientation): {
					for (uint8_t i {0}; i < 3; ++i) {
						offsetAngles[i] = static_cast<int16_t>(__REV16(reinterpret_cast<const int16_t*>(request.bData + 1)[i]))
						                / intScalingFactor;
					}
					break;
				}
				case (USBCommand::Variable::Mode): {
					gimbalMode = static_cast<GimbalMode>(util::min(request.bData[1], static_cast<uint8_t>(4)));
					showMode();
					break;
				}
				default:
					break;
			}
			break;
		}
		case (USBCommand::CommandType::MotorPassthrough): {
			usbPassthrough = true;
			usbPassthroughTime = util::getTime();
			motor::send(request.bData, len - 1);
			break;
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

// CAUTION: This function is called in an interrupt, no long-running operations allowed here!
void processSensor() {
	sensorReady = true;
}

// CAUTION: This function is called in an interrupt, no long-running operations allowed here!
void processButtons(bool left, bool pressed) {
	if (pressed) {
		if (powerMode == PowerMode::Sleep) {
			powerMode = PowerMode::Idle;
		} else if (displayState == DisplayState::BatteryLevel) {
			displayState = DisplayState::GimbalMode;
			showMode();
		}
	} else if (util::getTime() - eventTime < MAX_PRESS_WAIT_TIME) {
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
					enable();
				} else {
					disable();
				}
				showMode();

				return true;
			}
		} else if (shortPresses == 3) {
			__NVIC_SystemReset();
		} else {
			return true;
		}
	} else if (shortPresses == 1 && (!leftButton || isOff)) {
		adc::measureBattery([](uint16_t value) {
			if (powerMode == PowerMode::Sleep) {
				powerMode = PowerMode::Idle;
			}

			showBattery(value);
		});
	} else if (!isOff) {
		if (leftButton) {
			if (shortPresses == 1) {
				offsetAngles[0] = resetAngles[0];
				offsetAngles[1] = 0;
				offsetAngles[2] = resetAngles[2];
			} else if (shortPresses == 2) {
				resetAngles[2] += F_PI_2;
				if (resetAngles[2] > F_PI + 0.05f) {
					resetAngles[2] = 0;
				}
				offsetAngles[2] = resetAngles[2];
			} else if (shortPresses == 3) {
				resetAngles[0] += F_PI;
				if (resetAngles[0] > F_PI + 0.05f) {
					resetAngles[0] = 0;
				}
				offsetAngles[0] = resetAngles[0];
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
	WDT_REGS->WDT_CTRLA = WDT_CTRLA_ENABLE(0);
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
	eic::init();
	adc::init();
	pwm::init();
	i2c::init();
	uart::init();
	usb::init();

	uart::setCallback(processMotorResponse);
	eic::setButtonCallback(processButtons);
	eic::setSensor1Callback(processSensor);
	usb::setCallback(processUSBCommand);

	PORT_REGS->GROUP[0].PORT_DIRSET = 0x1 | (0x1 << 27u);
	WDT_REGS->WDT_CONFIG = WDT_CONFIG_PER_CYC64;

	usb::setSuspendCallback([](bool suspended) {
		if (suspended) {
			powerMode = PowerMode::Idle;
			disable();
		} else {
			uart::enable();
			wokenByUSB = true;
		}
	});

	{
		uint8_t steps {150};

		for (uint8_t step {0}; step < steps; ++step) {
			auto percentage = util::scale(static_cast<int>(step), 0, static_cast<int>(steps), 0, 255);
			auto brightnessInner = util::clamp(util::min(percentage, 192 - percentage), 0, 192) * (65535 / 192);
			auto brightnessOuter = util::clamp(util::min(percentage - 63, 255 - percentage), 0, 192) * (65535 / 192);

			pwm::setBrightness(1, brightnessInner);
			pwm::setBrightness(2, brightnessInner);

			pwm::setBrightness(0, brightnessOuter);
			pwm::setBrightness(3, brightnessOuter);

			util::sleep(1000 / steps);
		}
		setLEDs(0);
	}

	while (1) {
		switch (powerMode) {
			case (PowerMode::Active): {
				auto startTime {util::getTime()};

				LSM6DSO32::update([]() {
					sensorDataReady = true;
				});

				while (!sensorDataReady && util::getTime() - startTime < deltaTime / 4) {
					__WFI();
				}
				sensorDataReady = false;

				mahony.updateIMU(LSM6DSO32::getAngularRates(), LSM6DSO32::getAccelerations(), deltaTime / 1000.0f);

				joystick::update([](int16_t x, int16_t y) {
					if (gimbalMode != GimbalMode::Tilt) {
						offsetAngles[0] -= y / 40000.0f;
					} else {
						offsetAngles[2] -= y / 40000.0f;
					}
					offsetAngles[1] += x / 40000.0f;
				});

				Quaternion handleOrientation {mahony.getQuat() * Quaternion::fromEuler(0, controlBoardAngle, 0)};
				auto       handleAngles {handleOrientation.toEuler()};

				float yawTarget {handleAngles[0][0] + offsetAngles[0]};

				float sy {sinf(offsetAngles[0])};
				float cy {cosf(offsetAngles[0])};

				float pitchTarget {
				  (gimbalMode == GimbalMode::Horizon ? 0 : cy * handleAngles[1][0] - sy * handleAngles[2][0]) + offsetAngles[1]
				};
				float rollTarget {
				  (gimbalMode <= GimbalMode::Follow ? 0 : cy * handleAngles[2][0] + sy * handleAngles[1][0]) - offsetAngles[2]
				};

				currentAngles[0] +=
				    util::clamp(normalize(yawTarget - currentAngles[0]) / 50.0f, -maxVelocityPerStep, maxVelocityPerStep);

				currentAngles[1] +=
				    util::clamp(normalize(pitchTarget - currentAngles[1]) / 50.0f, -maxVelocityPerStep, maxVelocityPerStep);

				currentAngles[2] +=
				    util::clamp(normalize(rollTarget - currentAngles[2]) / 50.0f, -maxVelocityPerStep, maxVelocityPerStep);

				Quaternion cameraOrientation {Quaternion::fromEuler(currentAngles[0], currentAngles[1], currentAngles[2])};
				Quaternion gimbalRotation {handleOrientation.conjugate() * cameraOrientation};

				auto rotationAngles {gimbalRotation.toEuler()};
				auto calculatedAngles {calculateAngles(rotationAngles)};

				if (softStartActive) {
					for (uint8_t i {0}; i < motorPositionRequest; ++i) {
						motorAngles[i] += normalize(calculatedAngles[i][0] - motorAngles[i])
						                / static_cast<float>(softStartDuration - (util::getTime() - softStartTime))
						                * (deltaTime * 2);

						if (std::isnan(calculatedAngles[i][0])) {
							continue;
						}

						motor::move(i + 1, util::mod(static_cast<int16_t>(motorAngles[i] * motorScalingFactor), fullRevolution));

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

						motor::move(i + 1, util::mod(static_cast<int16_t>(motorAngles[i] * motorScalingFactor), fullRevolution));
					}
				}

				while (!sensorReady && util::getTime() - startTime < deltaTime / 4) {
					__WFI();
				}
				sensorReady = false;

				auto elapsedTime {util::getTime() - startTime};
				if (elapsedTime < deltaTime) {
					util::sleep(deltaTime - elapsedTime);
				}
				break;
			}
			case (PowerMode::Idle):
			default: {
				if (!usb::isActive() && wokenByUSB) {
					powerMode = PowerMode::Sleep;
					uart::disable();
					wokenByUSB = false;
				}

				util::sleep(deltaTime);
				break;
			}
			case (PowerMode::Sleep): {
				uart::enable();
				motor::sleep();
				if (usb::isActive()) {
					powerMode = PowerMode::Idle;
					uart::enable();
					wokenByUSB = true;
					break;
				}

				sleep();
				break;
			}
		}

		if (usb::isActive() && usbPassthrough && util::getTime() - usbPassthroughTime > USB_PASSTHROUGH_TIMEOUT) {
			uint8_t buf[1] {static_cast<uint8_t>(USBResponse::ResponseType::MotorPassthrough)};

			usb::write(buf, sizeof(buf));
			usbPassthrough = false;
		}

		if (displayState == DisplayState::BatteryLevel) {
			uint8_t step = (util::getTime() - stateChangeTime) / VOLTAGE_DISPLAY_TIME;
			uint8_t leds = voltageBars / 2;


			if (step > 15) {
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

		WDT_REGS->WDT_CLEAR = WDT_CLEAR_CLEAR_KEY;
	}

	return 1;
}
