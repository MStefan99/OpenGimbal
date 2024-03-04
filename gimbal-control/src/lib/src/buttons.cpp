#include "lib/inc/buttons.hpp"
#include "lib/inc/pwm.hpp"

enum class State : uint8_t {
    Idle,
    Short,
    Waiting,
    Long
};

static State currentState {State::Idle};
static uint32_t switchTime {0};
static bool power {false};

static void leds(uint8_t brightness) {
    if (power) {
        brightness = 255 - brightness;
    }
    for (uint8_t i {0}; i < 4; ++i) {
        pwm::setBrightness(i, brightness);
    }
}


static void onPress() {
    switch (currentState) {
        case (State::Idle): {
            currentState = State::Short;
            switchTime = util::getTime();
            leds(255);
            break;
        }
        case (State::Waiting): {
            currentState = State::Long;
            switchTime = util::getTime();
            break;
        }
    }
}


static void onRelease() {
    switch (currentState) {
        case (State::Short): {
            currentState = State::Waiting;
            switchTime = util::getTime();
            leds(0);
            break;
        }
        case (State::Long): {
            currentState = State::Idle;
            leds(0);
            break;
        }
    }
}


extern "C" {
    void EIC_Handler() {
        if (PORT_REGS->GROUP[0].PORT_IN & (0x1 << 24u)) {
            onRelease();
        } else {
            onPress();
        }
        EIC_REGS->EIC_INTFLAG = EIC_INTFLAG_Msk;
    }
}


void buttons::init() {    
    GCLK_REGS->GCLK_CLKCTRL = GCLK_CLKCTRL_ID_EIC // Enable EIC clock
            | GCLK_CLKCTRL_CLKEN(1) // Enable clock
			| GCLK_CLKCTRL_GEN_GCLK1; //Set GCLK1 as a clock source
    
    // Setting up pins 24 and 25
    PORT_REGS->GROUP[0].PORT_WRCONFIG = PORT_WRCONFIG_PINMASK(0x1 << 8u | 0x1 << 9u)
            | PORT_WRCONFIG_PMUXEN(1)
            | PORT_WRCONFIG_INEN(1)
            | PORT_WRCONFIG_PULLEN(1)
            | PORT_WRCONFIG_PMUX(MUX_PA24A_EIC_EXTINT4)
            | PORT_WRCONFIG_WRPMUX(1)
            | PORT_WRCONFIG_WRPINCFG(1)
            | PORT_WRCONFIG_HWSEL(1);
    
    // Enabling pull-ups on button inputs
    PORT_REGS->GROUP[0].PORT_OUTSET = (0x1 << 24u | 0x1 << 25u);
    
    EIC_REGS->EIC_INTENSET = EIC_INTENSET_EXTINT4(1)
            | EIC_INTENSET_EXTINT5(1);
    EIC_REGS->EIC_WAKEUP = EIC_WAKEUP_WAKEUPEN4(1)
            | EIC_WAKEUP_WAKEUPEN5(1);
    EIC_REGS->EIC_CONFIG[0] = EIC_CONFIG_SENSE4_BOTH
            | EIC_CONFIG_SENSE5_BOTH;
    EIC_REGS->EIC_CTRL = EIC_CTRL_ENABLE(1);
    
    NVIC_EnableIRQ(EIC_IRQn);
}


void buttons::update() {
    switch (currentState) {
        case (State::Short): {
            if (util::getTime() - switchTime > MAX_SHORT_PRESS_TIME) {
                currentState = State::Idle;
                leds(0);
            }
            break;
        }
        case (State::Waiting): {
            if (util::getTime() - switchTime > MAX_PRESS_WAIT_TIME) {
                currentState = State::Idle;
                leds(0);
            }
            break;
        }
        case (State::Long): {
            uint32_t duration = util::getTime() - switchTime;
            pwm::setBrightness(duration / LONG_PRESS_STEP_TIME - 1, power? 0 : 255);
            if (duration >= 4 * LONG_PRESS_STEP_TIME) {
                currentState = State::Idle;
                setPower(power = !power);
            }
            break;
        }
    }
}
