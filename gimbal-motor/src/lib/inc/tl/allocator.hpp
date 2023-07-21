//
// Created by MStefan99 on 16.4.22.
//

#ifndef ARDUI_ALLOCATOR_HPP
#define ARDUI_ALLOCATOR_HPP

#include <new>

#include "config_tl.hpp"
#include "device.h"


#if !USE_NEW

#include <cstdlib>

#endif


namespace tl {
	template <class T>
	struct allocator {
		using value_type = T;

		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;

		using size_type = unsigned int;
		using difference_type = unsigned long;


		template <typename U>
		struct rebind {
			using other = allocator<U>;
		};


		allocator() = default;
		~allocator() = default;

		pointer allocate(size_type n = 1);
		void deallocate(pointer p, size_type n = 1);

		template <class U, class... Args>
		void construct(U* p, Args&& ... args);
		template <class U>
		void destroy(U* p);
	};


	#if USE_NEW


	template <class T>
	typename allocator<T>::pointer allocator<T>::allocate(size_type n) {
		return static_cast<pointer>(::operator new(sizeof(T) * n));
	}


	template <class T>
	void allocator<T>::deallocate(pointer p, size_type n) {
		::operator delete(p);
	}


	#else


	template <class T>
	typename allocator<T>::pointer allocator<T>::allocate(size_type n) {
		__disable_irq();
		auto p = static_cast<pointer>(std::malloc(sizeof(T) * n));
		__enable_irq();
		return p;
	}


	template <class T>
	void allocator<T>::deallocate(pointer p, unsigned int) {
		__disable_irq();
		std::free(p);
		__enable_irq();
	}


	#endif


	template <class T>
	template <class U, class... Args>
	void allocator<T>::construct(U* p, Args&& ...args) {
		::new(static_cast<void*>(p)) U(args...);
	}


	template <class T>
	template <class U>
	void allocator<T>::destroy(U* p) {
		p->~U();
	}
}

#endif //ARDUI_ALLOCATOR_HPP
