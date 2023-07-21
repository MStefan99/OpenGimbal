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


namespace uart {
    void init();
	void send(uint8_t* data, uint8_t size, void (cb)(bool) = nullptr);

	uint8_t dataReceived();
	uint8_t* getData();
}


#endif	/* UART_HPP */

