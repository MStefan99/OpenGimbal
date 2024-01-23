/* 
 * File:   uart.hpp
 * Author: mikha
 *
 * Created on November 20, 2021, 7:55 PM
 */

#ifndef UART_HPP
#define	UART_HPP

#include "device.h"

#include "lib/inc/RingBuffer.hpp"
#include "lib/inc/util.hpp"

namespace uart {
    template<class size_type, size_type C>
    struct Buffer {
        uint8_t buffer[C] {};
        size_type transferred {0};
        size_type remaining {0};
    };

    template<class size_type, size_type C>
    using Callback = void (*)(const Buffer<size_type, C>&);

    void init();
    
    void send(const uint8_t* buf, uint8_t len);
    void setCallback(Callback<uint8_t, 8> cb);
}


#endif	/* UART_HPP */

