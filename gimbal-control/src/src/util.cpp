#include "inc/util.hpp"


static uint32_t ticks {0};
static TaskScheduler<uint8_t, 20> scheduler {};


extern "C" {
    void SysTick_Handler() {
        ++ticks;
    }
}

void util::init() {
    // SysTick config
    SysTick->CTRL = 0;
    SysTick->LOAD = 1000 - 1;
    SysTick->CTRL = SysTick_CTRL_TICKINT_Msk
            | SysTick_CTRL_CLKSOURCE_Msk
            | SysTick_CTRL_ENABLE_Msk;

    // NVIC config
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
