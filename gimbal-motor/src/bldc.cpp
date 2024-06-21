#include "bldc.hpp"

static tcc_registers_t* getTimer(uint8_t channel) {
	if (channel < 4) {
		return TCC0_REGS;
	} else {
		return TCC1_REGS;
	}
}

static uint8_t getTimerChannel(uint8_t channel) {
	if (channel < 4) {
		return channel;
	} else {
		return channel & 0x01;
	}
}

static uint8_t getPin(uint8_t channel) {
	if (channel < 2) {
		return channel + 4;
	} else if (channel < 4) {
		return channel + 8;
	} else {
		return channel + 2;
	}
}

struct TrigTable {
	int8_t table[1024];

	constexpr TrigTable():
	  table() {
		for (auto i = 0; i < 1024; ++i) {
			table[i] = std::round(std::sin(i * M_PI / 2048) * 127);
		}
	}

	constexpr int8_t sin(uint16_t i) const {
		if (i > 4096) {
			i %= 4096;
		}

		if (i < 2048) {
			return halfSin(i);
		} else {
			return -halfSin(i - 2048);
		}
	}

	constexpr int8_t cos(uint16_t i) const {
		return sin(i + 1024);
	}

protected:
	// i from 0 to 2047
	inline constexpr int8_t halfSin(uint16_t i) const {
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
	GCLK_REGS->GCLK_PCHCTRL[TCC2_GCLK_ID] = GCLK_PCHCTRL_CHEN(1)     // Enable TCC2 clock
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
	TCC0_REGS->TCC_CTRLA |= TCC_CTRLA_ENABLE(1);  // Enable timer

	TCC1_REGS->TCC_DBGCTRL = TCC_DBGCTRL_DBGRUN(1);                  // Run while debugging
	TCC1_REGS->TCC_WAVE = TCC_WAVE_WAVEGEN_NPWM | TCC_WAVE_POL1(1);  // PWM generation
	TCC1_REGS->TCC_PER = silentPeriod;
	TCC1_REGS->TCC_CC[0] = 0;
	TCC1_REGS->TCC_CC[1] = period;
	TCC1_REGS->TCC_CTRLA |= TCC_CTRLA_ENABLE(1);  // Enable timer
}

void bldc::enable() {
	// Pin setup
	for (uint8_t i {0}; i < 6; ++i) {
		uint8_t pin {getPin(i)};
		PORT_REGS->GROUP[0].PORT_PINCFG[pin] = PORT_PINCFG_PMUXEN(1);  // Enable mux on pin
		if (pin & 0x1) {                                               // Odd pin
			PORT_REGS->GROUP[0].PORT_PMUX[pin / 2] =
			    (PORT_REGS->GROUP[0].PORT_PMUX[pin / 2] & 0xf) | (i == 3 ? PORT_PMUX_PMUXO_F : PORT_PMUX_PMUXO_E);
		} else {  // Even pin
			PORT_REGS->GROUP[0].PORT_PMUX[pin / 2] =
			    (i == 2 ? PORT_PMUX_PMUXE_F : PORT_PMUX_PMUXE_E) | (PORT_REGS->GROUP[0].PORT_PMUX[pin / 2] & 0xf0);
		}
	}
}

void bldc::disable() {
	// Pin setup
	for (uint8_t i {0}; i < 6; ++i) {
		uint8_t pin {getPin(i)};
		PORT_REGS->GROUP[0].PORT_PINCFG[pin] = PORT_PINCFG_PMUXEN(0);  // Enable mux on pin
	}
}

void bldc::applyTorque(uint16_t angle, uint8_t power) {
	int8_t va = trigTable.sin(angle);
	int8_t vb = trigTable.cos(angle);

	uint16_t period = TCC0_REGS->TCC_PER + 1;
	uint8_t  factor = MAX_VAL / period + 1;
	uint16_t inv = period - (static_cast<uint32_t>(period) * power / 255);

	TCC0_REGS->TCC_CCBUF[0] = power * static_cast<uint16_t>(va + 127) / factor;
	TCC0_REGS->TCC_CCBUF[1] = TCC0_REGS->TCC_CCBUF[0] + inv;

	TCC0_REGS->TCC_CCBUF[2] = power * (((-va + ((SQRT3 * vb) >> 7u)) / 2) + 127) / factor;
	TCC0_REGS->TCC_CCBUF[3] = TCC0_REGS->TCC_CCBUF[2] + inv;

	TCC1_REGS->TCC_CCBUF[0] = power * (((-va - ((SQRT3 * vb) >> 7u)) / 2) + 127) / factor;
	TCC1_REGS->TCC_CCBUF[1] = TCC1_REGS->TCC_CCBUF[0] + inv;
}

void bldc::removeTorque() {
	TCC0_REGS->TCC_CC[0] = TCC0_REGS->TCC_CC[2] = TCC1_REGS->TCC_CC[0] = 0;

	TCC0_REGS->TCC_CC[1] = TCC0_REGS->TCC_CC[3] = TCC1_REGS->TCC_CC[1] = silentPeriod + 1;
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
