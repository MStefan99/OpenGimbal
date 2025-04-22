/*
 * File:   uart.hpp
 * Author: mikha
 *
 * Created on November 20, 2021, 7:55 PM
 */

#ifndef UART_HPP
#define UART_HPP

#include "device.h"

#include "dma.hpp"
#include "util.hpp"

namespace uart {
	template <class size_type, size_type C>
	struct Buffer {
		uint8_t   buffer[C] = {0};
		size_type transferred {0};
		size_type remaining {0};
		void      (*callback)() {nullptr};
	};

	template <class size_type, size_type C>
	struct Callback {
		using buffer_type = Buffer<size_type, C>;
		using callback_type = void (*)(const Buffer<size_type, C>&);
	};

	using DefaultCallback = Callback<uint8_t, 16>;
	using DefaultQueue = RingBuffer<uart::Buffer<uint8_t, 16>, uint8_t, 2>;

	void init();
	void enable();
	void disable();

	uint8_t print(const char* buf);
	void    send(const uint8_t* buf, const uint8_t len, void (*cb)() = nullptr);
	void    setCallback(DefaultCallback::callback_type cb);
	bool    busy();
}


#endif /* UART_HPP */
