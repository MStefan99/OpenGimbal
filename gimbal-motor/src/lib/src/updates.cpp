#include "lib/inc/updates.hpp"

static int16_t angle {0};
static int16_t da {1};
static int16_t dv {1};

void updates::init() {
    bldc::init();
    
    bldc::setAngle(0);
    util::sleep(100);
    as5600::update();
    util::sleep(10);
    uint16_t angle0 {as5600::getAngle()};
    
    bldc::setAngle(1000);
    util::sleep(100);
    as5600::update();
    util::sleep(10);
    uint16_t angle120 {as5600::getAngle()};
    
    uint16_t diff = angle120 - angle0;
    uint8_t poles = (2 << 12u) / diff;
}

void updates::ms() {
}

void updates::fast() {
}

void updates::slow() {
    // Nothing to do here yet
}
