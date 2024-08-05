#include "main.hpp"

#include "joystick.hpp"

/* Pinouts
 *
 * PA02 - Joystick analog input 1
 * PA04 - Joystick analog input 2
 *
 * PA05 - Supply, enables joystick and backlight
 *
 * PA08 - UART
 *
 * PA14 - LED 3
 * PA15 - LED 4
 *
 * PA24 - Button 1
 * PA25 - Button 2
 *
 * PA30 - LED 1 (shared with SWCLK)
 * PA31 - LED 2 (shared with SWDIO)
 *
 */


volatile DisplayState displayState {DisplayState::Off};
PowerMode             powerMode {PowerMode::Sleep};
GimbalMode            gimbalMode {GimbalMode::Follow};

int16_t yawOffset {0};
int16_t pitchOffset {0};
int16_t rollOffset {0};

int16_t yawReset {0};
int16_t rollReset {0};

volatile uint32_t stateChangeTime {0};
volatile uint8_t  voltageBars {0};

volatile uint8_t  shortPresses {0};
volatile uint32_t eventTime {0};
volatile bool     buttonPressed {false};
volatile bool     leftButton {true};

int16_t joystickX {0};
int16_t joystickY {0};

int16_t normalize(int16_t difference) {
	difference %= 4096;

	if (difference > halfRevolution) {
		return difference - fullRevolution;
	} else if (difference < -halfRevolution) {
		return difference + fullRevolution;
	} else {
		return difference;
	}
}

void setLEDs(uint32_t brightnesses) {
	for (uint8_t i {0}; i < 4; ++i) {
		pwm::setBrightness(i, brightnesses);
		brightnesses >>= 8u;
	}
}

void showMode() {
	if (displayState == DisplayState::Off) {
		setLEDs(0);
	} else {
		setLEDs(0xff << static_cast<uint8_t>(gimbalMode) * 8);
	}
}

void sendOffsets(bool sendYaw = true, bool sendPitch = true, bool sendRoll = true) {
	auto yawCommand {
	  SetVariableCommand {Command::Variable::YawOffset, yawOffset}
	};
	auto pitchCommand {
	  SetVariableCommand {Command::Variable::PitchOffset, pitchOffset}
	};
	auto rollCommand {
	  SetVariableCommand {Command::Variable::RollOffset, rollOffset}
	};

	if (sendYaw) {
		uart::send(yawCommand.getBuffer(), yawCommand.getLength());
	}
	if (sendPitch) {
		uart::send(pitchCommand.getBuffer(), pitchCommand.getLength());
	}
	if (sendRoll) {
		uart::send(rollCommand.getBuffer(), rollCommand.getLength());
	}
}

bool triggerAction() {
	bool isOff {displayState == DisplayState::Off};

	if (buttonPressed) {  // buttonPressed being true means this is a long press
		if (shortPresses == 1) {
			uint8_t led = (util::getTime() - (eventTime + MAX_PRESS_WAIT_TIME)) / LONG_PRESS_STEP_TIME;
			pwm::setBrightness(led - 1, isOff ? 0xff : 0);

			if (!led) {
				setLEDs(isOff ? 0 : 0xffffffff);
			} else if (led >= 4) {
				if (isOff) {
					displayState = DisplayState::GimbalMode;
					powerMode = PowerMode::Active;
					PORT_REGS->GROUP[0].PORT_OUTSET = 0x1 << 5u;
					joystick::updateCenter();
					sendOffsets();
				} else {
					displayState = DisplayState::Off;
					powerMode = PowerMode::Sleep;
					PORT_REGS->GROUP[0].PORT_OUTCLR = 0x1 << 5u;
					yawOffset = pitchOffset = rollOffset = yawReset = rollReset = 0;
				}
				showMode();

				auto command {
				  SetVariableCommand {Command::Variable::PowerMode, static_cast<uint8_t>(powerMode)}
				};
				uart::send(command.getBuffer(), command.getLength());

				return true;
			}
		} else {
			return true;
		}
	} else if (shortPresses == 1 && (!leftButton || isOff)) {
		setLEDs(0);
		auto command {GetVariableCommand {Command::Variable::BatteryVoltage}};
		uart::send(command.getBuffer(), command.getLength());
	} else if (!isOff) {
		if (leftButton) {
			if (shortPresses == 1) {
				yawOffset = yawReset;
				pitchOffset = 0;
				rollOffset = rollReset;

				sendOffsets(true, true, true);
			} else if (shortPresses == 2) {
				rollReset += quarterRevolution;
				if (rollReset > halfRevolution) {
					rollReset = 0;
				}
				rollOffset = rollReset;

				sendOffsets(false, false, true);
			} else if (shortPresses == 3) {
				yawReset += halfRevolution + 2;  // Slightly more than half a revolution to indicate direction
				if (yawReset > halfRevolution + 2) {
					yawReset = 0;
				}
				yawOffset = yawReset;

				sendOffsets(true, false, false);
			}
		} else {
			if (shortPresses == 2) {
				if (gimbalMode == GimbalMode::Tilt) {
					gimbalMode = GimbalMode::Horizon;
				} else {
					gimbalMode = static_cast<GimbalMode>(static_cast<uint8_t>(gimbalMode) + 1);
				}
				showMode();

				Command::GimbalMode mode;
				switch (gimbalMode) {
					case GimbalMode::Horizon:
					default:
						mode = Command::GimbalMode::Horizon;
						break;
					case GimbalMode::Follow:
						mode = Command::GimbalMode::Follow;
						break;
					case GimbalMode::FPV:
						mode = Command::GimbalMode::FPV;
						break;
					case GimbalMode::Tilt:
						mode = Command::GimbalMode::Follow;
				}

				auto command {
				  SetVariableCommand {Command::Variable::GimbalMode, static_cast<uint8_t>(mode)}
				};
				uart::send(command.getBuffer(), command.getLength());
			}
		}
	}

	return !buttonPressed;
}

// CAUTION: This function is called in an interrupt, no long-running operations allowed here!
void processResponse(const uart::DefaultCallback::buffer_type& buffer) {
	displayState = DisplayState::BatteryLevel;
	stateChangeTime = util::getTime();
	voltageBars = buffer.buffer[2] / (256 / 8) + 1;
}

// CAUTION: This function is called in an interrupt, no long-running operations allowed here!
void processButtons(bool left, bool pressed) {
	if (!pressed && util::getTime() - eventTime < MAX_PRESS_WAIT_TIME) {
		++shortPresses;
	}

	eventTime = util::getTime();
	buttonPressed = pressed;
	leftButton = left;

	switch (displayState) {
		case (DisplayState::Off): {
			if (shortPresses || !buttonPressed) {
				showMode();
				break;
			} else {
				setLEDs(0xffffffff);
			}
			break;
		}
		case (DisplayState::GimbalMode): {
			if (!buttonPressed) {
				showMode();
				break;
			} else {
				setLEDs(0);
			}
			break;
		}
		default:
			break;
	}
}

#if DV_OUT
struct DVData {
	uint8_t  header {0x03};
	uint16_t dt {};
	int16_t  joystickX {};
	int16_t  joystickY {};
	uint8_t  footer {0xfc};
} __attribute__((packed));
#endif

static void sleep() {
	SCB->SCR = SCB_SCR_SEVONPEND_Msk | SCB_SCR_SLEEPDEEP_Msk;
	__WFI();
	SCB->SCR = SCB_SCR_SEVONPEND_Msk;
}

int main() {
	util::init();
	buttons::init();
	pwm::init();
	uart::init();
	adc::init();

	uart::setCallback(processResponse);
	buttons::setCallback(processButtons);

	PORT_REGS->GROUP[0].PORT_DIRSET = 0x1 << 5u;

	while (1) {
#if DV_OUT
		auto startMs = util::getTime();
		auto startUs = SysTick->VAL;
#endif

		if (displayState == DisplayState::GimbalMode) {
			joystick::update([](int16_t x, int16_t y) {
				int16_t dx = x / 25;
				int16_t dy = y / 25;

				bool sendYaw {false};
				bool sendRoll {false};

				if (gimbalMode != GimbalMode::Tilt) {
					yawOffset -= dx;
					sendYaw = true;
				} else {
					rollOffset -= dx;
					sendRoll = true;
				}
				pitchOffset += dy;

				sendOffsets(sendYaw, true, sendRoll);
			});
		}

		if (buttonPressed && util::getTime() - eventTime > MAX_LONG_PRESS_TIME) {
			shortPresses = 0;
			buttonPressed = false;
			showMode();
		}

		if (displayState == DisplayState::BatteryLevel) {
			uint8_t step = (util::getTime() - stateChangeTime) / VOLTAGE_DISPLAY_TIME;
			uint8_t leds = voltageBars / 2;

			if (step >= leds) {
				// The calculation steps + leds + 1 is just to ensure the same timing for all LEDs
				pwm::setBrightness(leds, voltageBars % 2 && (step + leds + 1) % 2 ? 255 : 0);
			} else {
				pwm::setBrightness(step, leds ? 255 : 0);
			}

			if (step > 15 || buttonPressed) {
				displayState = powerMode == PowerMode::Active ? DisplayState::GimbalMode : DisplayState::Off;
				showMode();
			}
		}

		if ((buttonPressed && util::getTime() - eventTime > MAX_SHORT_PRESS_TIME)
		    || (shortPresses && util::getTime() - eventTime > MAX_PRESS_WAIT_TIME)) {
			if (triggerAction()) {
				shortPresses = 0;
				buttonPressed = false;
			}
		}

#if DV_OUT
		DVData data {};

		data.dt = (util::getTime() - startMs) * 1000 + (startUs - SysTick->VAL) / 2;
		data.joystickX = joystickX;
		data.joystickY = joystickY;
		uart::send(reinterpret_cast<uint8_t*>(&data), sizeof(data));
#endif

		if (displayState == DisplayState::Off && !shortPresses && !buttonPressed) {
			sleep();
		} else {
			util::sleep(50);
		}
	}
}
