/* 
 * File:   uart.hpp
 * Author: mikha
 *
 * Created on November 20, 2021, 7:55 PM
 */

#ifndef UART_HPP
#define	UART_HPP

#include "device.h"

#include "lib/inc/tl/allocator.hpp"
#include "lib/inc/util.hpp"
#include "lib/inc/dma.hpp"

extern void onInput(uint16_t value);

namespace uart {
    void init();
}


#endif	/* UART_HPP */

