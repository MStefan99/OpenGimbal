/*
 * File:   host.hpp
 * Author: mstefan99
 *
 * Created on December 2, 2025, 11:58 AM
 */

#ifndef HOST_HPP
#define HOST_HPP

#include "device.h"

#include "RingBuffer.hpp"
#include "util.hpp"

namespace host {
	template <class size_type, size_type C>
	struct Buffer {
		uint8_t   buffer[C] {};
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
	using DefaultQueue = RingBuffer<Buffer<uint8_t, 16>, uint8_t, 4>;

	void init();
	void enable();
	void disable();

	void send(const uint8_t* buf, const uint8_t len, void (*cb)() = nullptr);
	void setCallback(DefaultCallback::callback_type cb);
	bool busy();
}

#endif /* HOST_HPP */
