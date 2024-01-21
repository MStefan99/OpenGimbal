/* 
 * File:   uart.hpp
 * Author: mikha
 *
 * Created on November 20, 2021, 7:55 PM
 */

#ifndef UART_HPP
#define	UART_HPP

#include "device.h"


#include "lib/inc/util.hpp"
#include "lib/inc/dma.hpp"

namespace uart {
    template<class size_type, size_type C>
    struct Buffer {
        uint8_t buffer[C] {};
        size_type transferrred {0};
        size_type remaining {0};
    };

    template<class size_type, size_type C>
    using Callback = void (*)(const Buffer<size_type, C>&);

    void init();
    
    void sendToMotors(uint8_t* buf, uint8_t len);
    void setMotorCallback(Callback<uint8_t, 8> cb);
}


#endif	/* UART_HPP */

