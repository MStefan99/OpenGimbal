/* 
 * File:   uart.hpp
 * Author: mikha
 *
 * Created on November 20, 2021, 7:55 PM
 */

#ifndef UART_HPP
#define	UART_HPP

#include "device.h"

#include "main.hpp"
#include "lib/inc/util.hpp"
#include "lib/inc/dma.hpp"

namespace uart {
    void init();
    
    enum Command : uint8_t {
        Ping = 0,
        Position = 1
    };
}


#endif	/* UART_HPP */

