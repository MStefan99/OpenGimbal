/* 
 * File:   system.h
 * Author: mikha
 *
 * Created on November 20, 2021, 1:15 PM
 */


#ifndef SYSTEM_H
#define	SYSTEM_H

#include "device.h"


constexpr float TWO_PI {6.283185307179586476925286766559};
constexpr float PI {3.1415926535897932384626433832795};
constexpr float HALF_PI {1.5707963267948966192313216916398};
constexpr float QUARTER_PI {0.78539816339744830961566084581988};
constexpr float DEG_TO_RAD {0.017453292519943295769236907684886};
constexpr float RAD_TO_DEG {57.295779513082320876798154814105};
constexpr float EULER {2.718281828459045235360287471352};


constexpr uint8_t  MIN_INT8   {0x80};
constexpr uint8_t  MAX_INT8   {0x7f};
constexpr uint8_t  MIN_UINT8  {0x00};
constexpr uint8_t  MAX_UINT8  {0xff};
constexpr uint16_t MIN_INT16  {0x8000};
constexpr uint16_t MAX_INT16  {0x7fff};
constexpr uint16_t MIN_UINT16 {0x0000};
constexpr uint16_t MAX_UINT16 {0xffff};
constexpr uint32_t MIN_INT32  {0x80000000};
constexpr uint32_t MAX_INT32  {0x7fffffff};
constexpr uint32_t MIN_UINT32 {0x00000000};
constexpr uint32_t MAX_UINT32 {0xffffffff};


#define AVG(a, b) ((a + b) / 2)
#define ABS(a) ((a > 0)? (a) : -(a))
#define MIN(a, b) ((a < b)? (a) : (b))
#define MAX(a, b) ((a > b)? (a) : (b))
#define CLAMP(min, val, max) ((val < min)? (min) : ((val > max)? max : val))
#define MAP(minSrc, maxSrc, minTgt, maxTgt, val) ((val - minSrc) * (maxTgt - minTgt) / (maxSrc - minSrc) + minTgt)
#define SWAP(a, b) {\
  auto t {a};  \
  a = b;  \
  b = t;\
}
#define timeout(condition, time, body) {uint32_t _tme {system::getTickCount() + time}; \
    while (condition && _tme < system::getTickCount()) {\
        body\
		}\
}


namespace util {
	void init();

	uint32_t getTickCount();
	void sleep(uint32_t ms);

    
	template <class T>
	void copy(T* dest, const T* src, size_t len = 1) {
		for (size_t i{0}; i < len; ++i) {
			dest[i] = src[i];
		}
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
	T switchEndianness(T val) {
		if (sizeof (T) == 2) {
			return (val << 8u)
							| (val >> 8u & 0xff);
		} else if (sizeof (T) == 4) {
			return (val << 24u)
							| (val << 8u  & 0x00ff0000)
							| (val >> 8u  & 0x0000ff00)
							| (val >> 24u & 0x000000ff);
		} else {
			return val;
		}
	}
}

#endif	/* SYSTEM_H */

