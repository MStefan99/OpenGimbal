//
// Created by mikha on 30.11.21.
//

#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <cstdint>
#include <new>

/*
 * If objects in the buffer are plain data objects and don't need to be constructed/destructed,
 * this option can save memory
 */
#define PLAIN_DATA true


template<class T, class size_type, size_type C>
class RingBuffer {
public:
	using value_type = T;

	using pointer = value_type*;
	using const_pointer = const value_type*;
	using reference = value_type&;
	using const_reference = const value_type&;

	RingBuffer() : _front(0), _back(0), _size(0) {}

	RingBuffer(const RingBuffer& buffer);
	#if !PLAIN_DATA
	~RingBuffer();
	#endif

	void push_front(const_reference value);
	void push_back(const_reference value);

	void pop_front(size_type amount = 1);
	void pop_back(size_type amount = 1);

	reference front();
	const_reference front() const;
	reference back();
	const_reference back() const;

	size_type size() const;
	size_type capacity() const;

	bool empty() const;
	bool full() const;

	void clear();

	pointer data();
	const_pointer data() const;
	size_type contiguous() const;

	reference operator[](size_type index);
	const_reference operator[](size_type index) const;
	RingBuffer& operator=(const RingBuffer& buffer) = default;

protected:
	T _elements[C];
	size_type _front {0};
	size_type _back {0};
	size_type _size {0};
	static constexpr size_type _capacity {C};

	#if !PLAIN_DATA
	void destroy_elements();
	#endif
};



// Copy constructor
template<class T, class size_type, size_type C>
RingBuffer<T, size_type, C>::RingBuffer(const RingBuffer& buffer) : _back(buffer._size), _size(buffer._size) {
	for (size_type i = 0; i < _size; ++i) {
		new(&_elements[(_front + i) % _capacity]) T(buffer._elements[(buffer._front + i) % _capacity]);
	}
}

#if !PLAIN_DATA
// Destructor
template<class T, class size_type, size_type C>
RingBuffer<T, size_type, C>::~RingBuffer() {
	clear();
}

// Utility cb to destroy elements
template<class T, class size_type, size_type C>
void RingBuffer<T, size_type, C>::destroy_elements() {
	while (_size > 0) {
		_elements[_front].~T();
		_front = (_front + 1) % _capacity;
		--_size;
	}
}

#endif

// Clear method
template<class T, class size_type, size_type C>
void RingBuffer<T, size_type, C>::clear() {
	#if !PLAIN_DATA
	destroy_elements();
	#endif
	_front = _back = _size = 0;
}

// push_back method
template<class T, class size_type, size_type C>
void RingBuffer<T, size_type, C>::push_back(const_reference value) {
	if (_size == _capacity) {
		pop_front();
	}

	new(&_elements[_back]) T(value); // Use placement new for construction
	_back = (_back + 1) % _capacity;
	++_size;
}

// push_front method
template<class T, class size_type, size_type C>
void RingBuffer<T, size_type, C>::push_front(const_reference value) {
	if (_size == _capacity) {
		pop_back();
	}

	_front = (_front + _capacity - 1) % _capacity;
	new(&_elements[_front]) T(value); // Use placement new for construction
	++_size;
}

// pop_front method
template<class T, class size_type, size_type C>
void RingBuffer<T, size_type, C>::pop_front(size_type amount) {
	while (amount-- > 0 && _size > 0) {
		#if !PLAIN_DATA
		_elements[_front].~T(); // Call the destructor explicitly.
		#endif
		_front = (_front + 1) % _capacity;
		--_size;
	}
}

// pop_back method
template<class T, class size_type, size_type C>
void RingBuffer<T, size_type, C>::pop_back(size_type amount) {
	while (amount-- > 0 && _size > 0) {
		_back = (_back + _capacity - 1) % _capacity;
		#if !PLAIN_DATA
		_elements[_back].~T(); // Call the destructor explicitly.
		#endif
		--_size;
	}
}

template<class T, class size_type, size_type C>
typename RingBuffer<T, size_type, C>::reference RingBuffer<T, size_type, C>::back() {
	return _elements[_back == 0 ? _capacity - 1 : _back - 1];
}


template<class T, class size_type, size_type C>
typename RingBuffer<T, size_type, C>::const_reference RingBuffer<T, size_type, C>::back() const {
	return _elements[_back == 0 ? _capacity - 1 : _back - 1];
}


template<class T, class size_type, size_type C>
typename RingBuffer<T, size_type, C>::reference RingBuffer<T, size_type, C>::front() {
	return _elements[_front == _capacity ? 0 : _front];
}


template<class T, class size_type, size_type C>
typename RingBuffer<T, size_type, C>::const_reference RingBuffer<T, size_type, C>::front() const {
	return _elements[_front == _capacity ? 0 : _front];
}


template<class T, class size_type, size_type C>
size_type RingBuffer<T, size_type, C>::size() const {
	return _size;
}


template<class T, class size_type, size_type C>
size_type RingBuffer<T, size_type, C>::capacity() const {
	return _capacity;
}


template<class T, class size_type, size_type C>
bool RingBuffer<T, size_type, C>::empty() const {
	return !_size;
}


template<class T, class size_type, size_type C>
bool RingBuffer<T, size_type, C>::full() const {
	return _size == _capacity;
}


template<class T, class size_type, size_type C>
typename RingBuffer<T, size_type, C>::pointer RingBuffer<T, size_type, C>::data() {
	return _elements + _front;
}


template<class T, class size_type, size_type C>
typename RingBuffer<T, size_type, C>::const_pointer RingBuffer<T, size_type, C>::data() const {
	return _elements;
}


template<class T, class size_type, size_type C>
size_type RingBuffer<T, size_type, C>::contiguous() const {
	size_type dist = _capacity - _front;
	return _size < dist ? _size : dist;
}

template<class T, class size_type, size_type C>
typename RingBuffer<T, size_type, C>::reference RingBuffer<T, size_type, C>::operator[](size_type index) {
	return _elements[(_front + index) % _capacity];
}

template<class T, class size_type, size_type C>
typename RingBuffer<T, size_type, C>::const_reference RingBuffer<T, size_type, C>::operator[](size_type index) const {
	return _elements[(_front + index) % _capacity];
}


#endif //RINGBUFFER_H
