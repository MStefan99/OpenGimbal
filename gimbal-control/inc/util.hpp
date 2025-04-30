/*
 * File:   system.h
 * Author: mikha
 *
 * Created on November 20, 2021, 1:15 PM
 */


#ifndef UTIL_HPP
#define UTIL_HPP

#include "device.h"

constexpr float F_2_PI {6.283185307179586476925286766559};
constexpr float F_PI {3.1415926535897932384626433832795};
constexpr float F_PI_2 {1.5707963267948966192313216916398};
constexpr float F_PI_3 {1.0471975511965977461542144610932};
constexpr float F_PI_4 {0.78539816339744830961566084581988};
constexpr float F_PI_8 {0.39269908169872415480783042290994};
constexpr float F_DEG_TO_RAD {0.017453292519943295769236907684886};
constexpr float F_RAD_TO_DEG {57.295779513082320876798154814105};
constexpr float F_E {2.718281828459045235360287471352};

constexpr uint8_t  MIN_INT8 {0x80};
constexpr uint8_t  MAX_INT8 {0x7f};
constexpr uint8_t  MIN_UINT8 {0x00};
constexpr uint8_t  MAX_UINT8 {0xff};
constexpr uint16_t MIN_INT16 {0x8000};
constexpr uint16_t MAX_INT16 {0x7fff};
constexpr uint16_t MIN_UINT16 {0x0000};
constexpr uint16_t MAX_UINT16 {0xffff};
constexpr uint32_t MIN_INT32 {0x80000000};
constexpr uint32_t MAX_INT32 {0x7fffffff};
constexpr uint32_t MIN_UINT32 {0x00000000};
constexpr uint32_t MAX_UINT32 {0xffffffff};

namespace util {
	void init();

	uint32_t getTime();
	void     sleep(uint32_t ms);

	template <class T, class S>
	void copy(T* dest, const T* src, S len = 1) {
		for (S i {0}; i < len; ++i) {
			dest[i] = src[i];
		}
	}

	template <class T>
	void swap(T* a, T* b) {
		T t {*a};
		*a = *b;
		*b = t;
	}

	template <class T>
	void copy(T* dest, const T& src) {
		*dest = src;
	}

	template <class T>
	void copy(T* dest, T&& src) {
		*dest = src;
	}

	template <class T>
	T mod(T a, T b) {
		return ((a % b) + b) % b;
	}

	template <class T>
	T avg(T a, T b) {
		return (a + b) / 2;
	}

	template <class T>
	T abs(T a) {
		return a > 0 ? a : -a;
	}

	template <class T>
	int8_t sign(T a) {
		return a > 0 ? 1 : a < 0 ? -1 : 0;
	}

	template <class T>
	T min(T a, T b) {
		return (a < b) ? a : b;
	}

	template <class T>
	T max(T a, T b) {
		return (a > b) ? a : b;
	}

	template <class T>
	T clamp(T val, T min, T max) {
		return (val < min) ? min : (val > max) ? max : val;
	}

	template <class T>
	T scale(T val, T minSrc, T maxSrc, T minTgt, T maxTgt) {
		return (val - minSrc) * (maxTgt - minTgt) / (maxSrc - minSrc) + minTgt;
	}

	float invSqrt(float a);
}  // namespace util

#endif /* UTIL_HPP */
