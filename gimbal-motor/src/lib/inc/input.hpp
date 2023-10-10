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


extern void onInput(uint16_t value);
    
namespace input {
    void init();
}

#endif	/* INPUT_HPP */

