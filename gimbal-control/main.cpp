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

#define DV_OUT 0

DisplayState displayState {DisplayState::Off};
PowerMode    powerMode {PowerMode::Sleep};
GimbalMode   gimbalMode {GimbalMode::Follow};

uint16_t yawOffset {0};
uint16_t pitchOffset {0};
uint16_t rollOffset {0};

uint32_t stateChangeTime {0};
uint8_t  voltageBars {0};

uint8_t  shortPresses {0};
uint32_t eventTime {0};
bool     buttonPressed {false};
bool     leftButton {true};

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
				displayState = isOff ? DisplayState::GimbalMode : DisplayState::Off;
				powerMode = isOff ? PowerMode::Active : PowerMode::Sleep;
				showMode();

				auto command {
				  Command {Command::CommandType::SetVariable, 2}
				};
				auto buf {command.getBuffer()};
				buf[1] = static_cast<uint8_t>(Command::Variable::PowerMode);
				buf[2] = static_cast<uint8_t>(powerMode);

				uart::send(buf, command.getLength());
				return true;
			}
		}
	} else {
		if (leftButton) {
			if (shortPresses == 1) {
				if (isOff) {
					auto command {
					  Command {Command::CommandType::GetVariable, 1}
					};
					auto buf {command.getBuffer()};
					buf[1] = static_cast<uint8_t>(Command::Variable::BatteryVoltage);

					uart::send(buf, command.getLength());
				} else {
					if (gimbalMode == GimbalMode::FPV) {
						gimbalMode = GimbalMode::Horizon;
					} else {
						gimbalMode = static_cast<GimbalMode>(static_cast<uint8_t>(gimbalMode) + 1);
					}
					showMode();

					auto command {
					  Command {Command::CommandType::SetVariable, 2}
					};
					auto buf {command.getBuffer()};
					buf[1] = static_cast<uint8_t>(Command::Variable::GimbalMode);
					buf[2] = static_cast<uint8_t>(gimbalMode);

					uart::send(buf, command.getLength());
				}
			} else if (shortPresses == 2) {
				rollOffset += 1024;
				if (rollOffset > 2048) {
					rollOffset = 0;
				}

				auto command {
				  Command {Command::CommandType::SetVariable, 3}
				};
				auto buf {command.getBuffer()};
				buf[1] = static_cast<uint8_t>(Command::Variable::RollOffset);
				buf[2] = rollOffset >> 8u;
				buf[3] = rollOffset;

				uart::send(buf, command.getLength());
			} else if (shortPresses == 3) {
				yawOffset += 2050;  // Slightly more than half a revolution to indicate direction
				if (yawOffset > 2050) {
					yawOffset = 0;
				}

				auto command {
				  Command {Command::CommandType::SetVariable, 3}
				};
				auto buf {command.getBuffer()};
				buf[1] = static_cast<uint8_t>(Command::Variable::YawOffset);
				buf[2] = yawOffset >> 8u;
				buf[3] = yawOffset;

				uart::send(buf, command.getLength());
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
	uint16_t joystickX {};
	uint16_t joystickY {};
	uint8_t  footer {0xfc};
} __attribute__((packed));
#endif


int main() {
	util::init();
	buttons::init();
	pwm::init();
	uart::init();

	uart::setCallback(processResponse);
	buttons::setCallback(processButtons);

	PORT_REGS->GROUP[0].PORT_DIRSET = 0x1 << 5u;

	while (1) {
#if DV_OUT
		auto startMs = util::getTime();
		auto startUs = SysTick->VAL;
#endif

#if 0
    ADC_REGS->ADC_INPUTCTRL = ADC_INPUTCTRL_MUXNEG_GND // Set GND as negative input
        | ADC_INPUTCTRL_MUXPOS_PIN0; // Set temperature sensor as positive input
    ADC_REGS->ADC_SWTRIG = ADC_SWTRIG_START(1); // Start conversion
    while (!(ADC_REGS->ADC_INTFLAG & ADC_INTFLAG_RESRDY_Msk)); // Wait for ADC result
    uint16_t joystickY = 4095 - ADC_REGS->ADC_RESULT;

    ADC_REGS->ADC_INPUTCTRL = ADC_INPUTCTRL_MUXNEG_GND // Set GND as negative input
        | ADC_INPUTCTRL_MUXPOS_PIN2; // Set temperature sensor as positive input
    ADC_REGS->ADC_SWTRIG = ADC_SWTRIG_START(1); // Start conversion
    while (!(ADC_REGS->ADC_INTFLAG & ADC_INTFLAG_RESRDY_Msk)); // Wait for ADC result
    uint16_t joystickX = ADC_REGS->ADC_RESULT;
#endif

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
