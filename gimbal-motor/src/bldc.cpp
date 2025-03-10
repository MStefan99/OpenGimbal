#include "bldc.hpp"

struct TrigTable {
	uint8_t table[1024];

	constexpr TrigTable():
	  table() {
		for (auto i = 0; i < 1024; ++i) {
			table[i] = std::round(std::sin(i * M_PI / 2048) * 127.5f + 127.5f);
		}
	}

	constexpr uint8_t sin(uint16_t i) const {
		if (i > 4096) {
			i %= 4096;
		}

		if (i < 2048) {
			return halfSin(i);
		} else {
			return -halfSin(i - 2048);
		}
	}

	constexpr uint8_t cos(uint16_t i) const {
		return sin(i + 1024);
	}

protected:
	// i from 0 to 2047
	inline constexpr uint8_t halfSin(uint16_t i) const {
		if (i < 1024) {
			return table[i];
		} else {
			return table[2047 - i];
		}
	}
};

constexpr static uint16_t silentPeriod {1999};  // 2000 possible values from 0 through 1999
constexpr static auto     trigTable = TrigTable();
constexpr static uint16_t SQRT3 = sqrtf(3) * 128;
constexpr static uint16_t MAX_VAL = 255 * 255;

void bldc::init() {
	// Clock setup
	GCLK_REGS->GCLK_PCHCTRL[TCC0_GCLK_ID] = GCLK_PCHCTRL_CHEN(1)     // Enable TCC[0:1] clock
	                                      | GCLK_PCHCTRL_GEN_GCLK1;  // Set GCLK1 as a clock source

	uint16_t period {silentPeriod + 1};

	// TCC setup
	TCC0_REGS->TCC_DBGCTRL = TCC_DBGCTRL_DBGRUN(1);                                     // Run while debugging
	TCC0_REGS->TCC_WAVE = TCC_WAVE_WAVEGEN_NPWM | TCC_WAVE_POL1(1) | TCC_WAVE_POL3(1);  // PWM generation
	TCC0_REGS->TCC_PER = silentPeriod;
	TCC0_REGS->TCC_CC[0] = 0;
	TCC0_REGS->TCC_CC[1] = period;
	TCC0_REGS->TCC_CC[2] = 0;
	TCC0_REGS->TCC_CC[3] = period;
	TCC0_REGS->TCC_CTRLA = TCC_CTRLA_ENABLE(1);  // Enable timer

	TCC1_REGS->TCC_DBGCTRL = TCC_DBGCTRL_DBGRUN(1);                  // Run while debugging
	TCC1_REGS->TCC_WAVE = TCC_WAVE_WAVEGEN_NPWM | TCC_WAVE_POL1(1);  // PWM generation
	TCC1_REGS->TCC_PER = silentPeriod;
	TCC1_REGS->TCC_CC[0] = 0;
	TCC1_REGS->TCC_CC[1] = period;
	TCC1_REGS->TCC_CTRLA = TCC_CTRLA_ENABLE(1);  // Enable timer
}

void bldc::enable() {
	// Pin setup
	PORT_REGS->GROUP[0].PORT_WRCONFIG = PORT_WRCONFIG_PINMASK(0x3 << 8u)        // Select pins 8 and 9
	                                  | PORT_WRCONFIG_PMUXEN(1)                 // Enable pin mux
	                                  | PORT_WRCONFIG_PMUX(MUX_PA08F_TCC1_WO2)  // Mux to TCC1
	                                  | PORT_WRCONFIG_WRPMUX(1)                 // Write pin mux
	                                  | PORT_WRCONFIG_WRPINCFG(1);              // Write pin config

	PORT_REGS->GROUP[0].PORT_WRCONFIG = PORT_WRCONFIG_PINMASK(0x3 << 10u)       // Select pins 10 and 11
	                                  | PORT_WRCONFIG_PMUXEN(1)                 // Enable pin mux
	                                  | PORT_WRCONFIG_PMUX(MUX_PA10F_TCC0_WO2)  // Mux to TCC1
	                                  | PORT_WRCONFIG_WRPMUX(1)                 // Write pin mux
	                                  | PORT_WRCONFIG_WRPINCFG(1);              // Write pin config

	PORT_REGS->GROUP[0].PORT_WRCONFIG = PORT_WRCONFIG_PINMASK(0x3 << 14u)       // Select pins 14 and 15
	                                  | PORT_WRCONFIG_PMUXEN(1)                 // Enable pin mux
	                                  | PORT_WRCONFIG_PMUX(MUX_PA14F_TCC0_WO4)  // Mux to TCC1
	                                  | PORT_WRCONFIG_WRPMUX(1)                 // Write pin mux
	                                  | PORT_WRCONFIG_WRPINCFG(1);              // Write pin config
}

void bldc::disable() {
	// Pin setup
	PORT_REGS->GROUP[0].PORT_WRCONFIG = PORT_WRCONFIG_PINMASK(0xf << 8u)  // Select pins 8-11
	                                  | PORT_WRCONFIG_PMUXEN(0)           // Disable pin mux
	                                  | PORT_WRCONFIG_WRPINCFG(1);        // Write pin config

	PORT_REGS->GROUP[0].PORT_WRCONFIG = PORT_WRCONFIG_PINMASK(0x3 << 14u)  // Select pins 14 and 15
	                                  | PORT_WRCONFIG_PMUXEN(0)            // Disable pin mux
	                                  | PORT_WRCONFIG_WRPINCFG(1)          // Write pin config
	                                  | PORT_WRCONFIG_HWSEL(1);            // Select top pins
}

void bldc::applyTorque(uint16_t angle, uint8_t power) {
	uint16_t period = TCC0_REGS->TCC_PER + 1;
	uint8_t  factor = MAX_VAL / period + 1;
	uint16_t inv = period - (static_cast<uint32_t>(period) * power / 255);

	TCC0_REGS->TCC_CCBUF[0] = power * trigTable.sin(angle) / factor;
	TCC0_REGS->TCC_CCBUF[1] = TCC0_REGS->TCC_CCBUF[0] + inv;

	TCC0_REGS->TCC_CCBUF[2] = power * trigTable.sin(angle + fullRevolution / 3) / factor;
	TCC0_REGS->TCC_CCBUF[3] = TCC0_REGS->TCC_CCBUF[2] + inv;

	TCC1_REGS->TCC_CCBUF[0] = power * trigTable.sin(angle - fullRevolution / 3) / factor;
	TCC1_REGS->TCC_CCBUF[1] = TCC1_REGS->TCC_CCBUF[0] + inv;
}

void bldc::removeTorque() {
	TCC0_REGS->TCC_CC[0] = TCC0_REGS->TCC_CC[2] = TCC1_REGS->TCC_CC[0] = 0;
	TCC0_REGS->TCC_CC[1] = TCC0_REGS->TCC_CC[3] = TCC1_REGS->TCC_CC[1] = silentPeriod + 1;

	TCC0_REGS->TCC_CTRLBSET = TCC1_REGS->TCC_CTRLBSET = TCC_CTRLBSET_CMD_RETRIGGER;
}

void bldc::tone(uint16_t frequency) {
	uint32_t period = 48000000 / frequency - 1;
	if (period < silentPeriod) {
		silent();
	} else {
		TCC0_REGS->TCC_PERBUF = TCC1_REGS->TCC_PERBUF = period;
	}
}

void bldc::silent() {
	TCC0_REGS->TCC_PERBUF = TCC1_REGS->TCC_PERBUF = silentPeriod;
}
