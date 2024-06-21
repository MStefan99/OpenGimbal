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

PowerMode    powerMode {PowerMode::Sleep};
DisplayState displayState {DisplayState::Off};
GimbalMode   gimbalMode {GimbalMode::Follow};
uint32_t     stateChangeTime {0};
uint8_t      voltageBars {0};


uint32_t ledStates[6][2] = {
  {0,          0         },
  {0xffffffff, 0         },
  {0,          0xffffffff},
  {0,          0xffffffff},
  {0xff0000ff, 0         },
  {0,          0         }
};

void setLEDs(uint32_t brightnesses) {
	for (uint8_t i {0}; i < 4; ++i) {
		pwm::setBrightness(i, brightnesses);
		brightnesses >>= 8u;
	}
}

void setDisplayState(DisplayState state) {
	displayState = state;
	if (state != DisplayState::GimbalMode) {
		setLEDs(ledStates[static_cast<uint8_t>(displayState)][powerMode == PowerMode::Active ? 1 : 0]);
	} else {
		setLEDs(0xff << static_cast<uint8_t>(gimbalMode) * 8u);
	}
	stateChangeTime = util::getTime();
}

// CAUTION: This function is called in an interrupt, no long-running operations allowed here!
void processResponse(const uart::DefaultCallback::buffer_type& buffer) {
	setDisplayState(DisplayState::BatteryLevel);

	uint8_t voltage = buffer.buffer[2];
	voltageBars = voltage / (256 / 8) + 1;
}

void processButtons(bool left, bool pressed) {
	switch (displayState) {
		case (DisplayState::Off):
		case (DisplayState::GimbalMode):
		case (DisplayState::BatteryLevel):
			if (pressed) {
				setDisplayState(DisplayState::ShortPress);
			}
			break;
		case (DisplayState::ShortPress):
			if (!pressed) {
				setDisplayState(DisplayState::PressWait);
			}
			break;
		case (DisplayState::PressWait):
			if (pressed) {
				setDisplayState(DisplayState::LongPress);
			}
			break;
		case (DisplayState::LongPress):
			if (!pressed) {
				setDisplayState(DisplayState::Off);
			}
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

		switch (displayState) {
			case (DisplayState::ShortPress): {
				if (util::getTime() - stateChangeTime > MAX_SHORT_PRESS_TIME) {
					setDisplayState(DisplayState::Off);
				}
				break;
			}
			case (DisplayState::PressWait): {
				if (util::getTime() - stateChangeTime > MAX_PRESS_WAIT_TIME) {
					setDisplayState(DisplayState::Off);

					auto command {
					  Command {Command::CommandType::GetVariable, 1}
					};
					auto buf {command.getBuffer()};
					buf[1] = static_cast<uint8_t>(Command::Variable::BatteryVoltage);

					uart::send(buf, command.getLength());
				}
				break;
			}
			case (DisplayState::LongPress): {
				uint8_t led = (util::getTime() - stateChangeTime) / LONG_PRESS_STEP_TIME;
				bool    isOn = powerMode == PowerMode::Active;
				pwm::setBrightness(led, isOn ? 0 : 0xff);

				if (led >= 3) {
					setDisplayState(isOn ? DisplayState::Off : DisplayState::GimbalMode);
					powerMode = isOn ? PowerMode::Sleep : PowerMode::Active;

					auto command {
					  Command {Command::CommandType::SetVariable, 2}
					};
					auto buf {command.getBuffer()};
					buf[1] = static_cast<uint8_t>(Command::Variable::PowerMode);
					buf[2] = static_cast<uint8_t>(powerMode);

					uart::send(buf, command.getLength());
				}
				break;
			}
			case (DisplayState::BatteryLevel): {
				uint8_t step = (util::getTime() - stateChangeTime) / VOLTAGE_DISPLAY_TIME;
				uint8_t leds = voltageBars / 2;

				if (step >= leds) {
					// The calculation steps + leds + 1 is just to ensure the same timing for all LEDs
					pwm::setBrightness(leds, voltageBars % 2 && (step + leds + 1) % 2 ? 255 : 0);
				} else {
					pwm::setBrightness(step, leds ? 255 : 0);
				}

				if (step > 15) {
					setDisplayState(powerMode == PowerMode::Active ? DisplayState::GimbalMode : DisplayState::Off);
				}
				break;
			}
			default:
				break;
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
