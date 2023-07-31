/* 
 * File:   input.hpp
 * Author: Mikhail
 *
 * Created on July 29, 2023, 8:37 PM
 */

#ifndef INPUT_HPP
#define	INPUT_HPP

#include "device.h"

#include "util.hpp"

namespace input {
    void init();
    
    extern void onInput(uint16_t value);
}

#endif	/* INPUT_HPP */

