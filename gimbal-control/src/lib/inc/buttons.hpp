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
    using Callback = void(*)(bool left, bool pressed);

    void init();
    void setCallback(Callback cb);
}

#endif	/* BUTTONS_HPP */
