#include "lib/inc/util.hpp"


static uint32_t ticks {0};
static TaskScheduler<uint8_t, 20> scheduler {};


extern "C" {
    void SysTick_Handler() {
        ++ticks;
    }
}

void util::init() {
    // Oscillator setup
    SYSCTRL_REGS->SYSCTRL_OSC8M = SYSCTRL_REGS->SYSCTRL_OSC8M & ~SYSCTRL_OSC8M_PRESC_Msk | SYSCTRL_OSC8M_PRESC_2;
    
    // GCLK setup
    GCLK_REGS->GCLK_GENCTRL = GCLK_GENCTRL_ID(1)
            | GCLK_GENCTRL_SRC_OSCULP32K
            | GCLK_GENCTRL_GENEN(1);
    GCLK_REGS->GCLK_GENDIV = GCLK_GENDIV_ID(1)
            | GCLK_GENDIV_DIV(32);
    
    // APB clock setup
    PM_REGS->PM_APBCMASK = PM_APBCMASK_SERCOM1(1) // Enable APB SERCOM1 clock
            | PM_APBCMASK_TC1(1) // Enable APB TC1 clock
            | PM_APBCMASK_TC2(1); // Enable APB TC2 clock

    // SysTick setup
    SysTick_Config(2000);

    // NVIC setup
    __DMB();
    __enable_irq();
    NVIC_EnableIRQ(SysTick_IRQn);
}

uint32_t util::getTime() {
    return ticks;
}

void util::sleep(uint32_t ms) {
    uint32_t t{ticks + ms};

    while (ticks < t) {
        __WFI();
    }
}

uint8_t util::setTimeout(uint32_t timeout, void (*cb)()) {
    return scheduler.setTimeout(ticks, timeout, cb);
}

uint8_t util::setInterval(uint32_t interval, void (*cb)()) {
    return scheduler.setInterval(ticks, interval, cb);
}

void util::clearTimeout(uint8_t id) {
    scheduler.clearTimeout(id);
}

void util::clearInterval(uint8_t id) {
    scheduler.clearInterval(id);
}

void util::runTasks() {
    scheduler.execute(ticks);
}
