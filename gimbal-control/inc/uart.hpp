/*
 * File:   uart.hpp
 * Author: mikha
 *
 * Created on November 20, 2021, 7:55 PM
 */

#ifndef UART_HPP
#define UART_HPP

#include "device.h"

#include "RingBuffer.hpp"
#include "uart.hpp"
#include "util.hpp"

#define UART_HALF_DUPLEX 1

namespace uart {
	template <class size_type, size_type C>
	struct Buffer {
		uint8_t   buffer[C] {};
		size_type transferred {0};
		size_type remaining {0};

		constexpr static size_type capacity {C};
	};

	template <class size_type, size_type C>
	struct Callback {
		using buffer_type = Buffer<size_type, C>;
		using callback_type = void (*)(const Buffer<size_type, C>&);
	};

	using DefaultCallback = Callback<uint8_t, 16>;
	using DefaultQueue = RingBuffer<uart::Buffer<uint8_t, 16>, uint8_t, 4>;

	void    init();
	uint8_t print(const char* buf);

	uint8_t sendToMotors(const uint8_t* buf, uint8_t len);
	void    setMotorCallback(DefaultCallback::callback_type cb);
	void    clear();
}


#endif /* UART_HPP */
