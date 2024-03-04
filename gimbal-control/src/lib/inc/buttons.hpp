/* 
 * File:   buttons.hpp
 * Author: Mikhail
 *
 * Created on January 24, 2024, 8:22 PM
 */

#ifndef BUTTONS_HPP
#define	BUTTONS_HPP

#include "device.h"

#include "util.hpp"
#include "main.hpp"


namespace buttons  {
    constexpr uint16_t MAX_SHORT_PRESS_TIME {1000};
    constexpr uint16_t MAX_PRESS_WAIT_TIME {2000};
    constexpr uint16_t LONG_PRESS_STEP_TIME {250};

    void init();
    
    void update(); // Should be called regularly to update state machine
}

#endif	/* BUTTONS_HPP */

