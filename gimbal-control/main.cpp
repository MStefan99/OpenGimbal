#include "main.hpp"

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


DisplayState displayState {DisplayState::Off};
PowerMode    powerMode {PowerMode::Sleep};
GimbalMode   gimbalMode {GimbalMode::Follow};

int16_t yawOffset {0};
int16_t pitchOffset {0};
int16_t rollOffset {0};

uint32_t stateChangeTime {0};
uint8_t  voltageBars {0};

uint8_t  shortPresses {0};
uint32_t eventTime {0};
bool     buttonPressed {false};
bool     leftButton {true};

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
				} else {
					displayState = DisplayState::Off;
					powerMode = PowerMode::Sleep;
					PORT_REGS->GROUP[0].PORT_OUTCLR = 0x1 << 5u;
					yawOffset = pitchOffset = rollOffset = 0;
				}
				showMode();

				auto command {
				  SetVariableCommand {Command::Variable::PowerMode, static_cast<uint8_t>(powerMode)}
				};
				uart::send(command.getBuffer(), command.getLength());

				return true;
			}
		}
	} else {
		if (leftButton) {
			if (shortPresses == 1) {
				yawOffset = pitchOffset = rollOffset = 0;
			} else if (shortPresses == 2) {
				rollOffset += quarterRevolution;
				if (rollOffset > halfRevolution) {
					rollOffset = 0;
				}

				auto command {
				  SetVariableCommand {Command::Variable::RollOffset, rollOffset}
				};
				uart::send(command.getBuffer(), command.getLength());
			} else if (shortPresses == 3) {
				yawOffset += halfRevolution + 2;  // Slightly more than half a revolution to indicate direction
				if (yawOffset > halfRevolution + 2) {
					yawOffset = 0;
				}

				auto command {
				  SetVariableCommand {Command::Variable::YawOffset, yawOffset}
				};
				uart::send(command.getBuffer(), command.getLength());
			}
		} else {
			if (shortPresses == 1) {
				auto command {GetVariableCommand {Command::Variable::BatteryVoltage}};
				uart::send(command.getBuffer(), command.getLength());
			} else if (shortPresses == 2) {
				if (gimbalMode == GimbalMode::FPV) {
					gimbalMode = GimbalMode::Horizon;
				} else {
					gimbalMode = static_cast<GimbalMode>(static_cast<uint8_t>(gimbalMode) + 1);
				}
				showMode();

				auto command {
				  SetVariableCommand {Command::Variable::GimbalMode, static_cast<uint8_t>(gimbalMode)}
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
			if (shortPresses || !buttonPressed) {
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
	PM_REGS->PM_SLEEP = PM_SLEEP_IDLE_APB;
	while (PM_REGS->PM_SLEEP != PM_SLEEP_IDLE_APB);
	do {
		__WFI();
	} while (displayState == DisplayState::Off);
	SCB->SCR = SCB_SCR_SEVONPEND_Msk;
	PM_REGS->PM_SLEEP = PM_SLEEP_IDLE_CPU;
	while (PM_REGS->PM_SLEEP != PM_SLEEP_IDLE_CPU);
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
			adc::getX([](uint16_t x) {
				yawOffset = normalize(yawOffset + (static_cast<int16_t>(x) - 1650) / 100);

				auto command {
				  SetVariableCommand {Command::Variable::YawOffset, yawOffset}
				};
				uart::send(command.getBuffer(), command.getLength());

				adc::getY([](uint16_t y) {
					pitchOffset = normalize(pitchOffset + (1650 - static_cast<int16_t>(y)) / 100);

					auto command {
					  SetVariableCommand {Command::Variable::PitchOffset, pitchOffset}
					};
					uart::send(command.getBuffer(), command.getLength());
				});
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

		util::sleep(50);
	}
}
