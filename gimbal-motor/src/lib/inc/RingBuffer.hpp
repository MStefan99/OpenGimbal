//
// Created by mikha on 30.11.21.
//

#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <new>


template<class T, size_t C>
class RingBuffer {
public:
	using value_type = T;

	using pointer = value_type*;
	using const_pointer = const value_type*;
	using reference = value_type&;
	using const_reference = const value_type&;

	using size_type = size_t;

	RingBuffer() : _front(0), _back(0), _size(0) {}
	RingBuffer(const RingBuffer& buffer);
	~RingBuffer() = default;

	void push_front(const_reference value);
	void push_back(const_reference value);

	void pop_front(size_type amount = 1);
	void pop_back(size_type amount = 1);

	reference front();
	reference back();

	size_type size() const;
	size_type capacity() const;

	bool empty() const;
	bool full() const;

	void clear();

	pointer data();
	const_pointer data() const;
	size_type contiguous() const;

	reference operator[](size_type index);
	RingBuffer& operator=(const RingBuffer& buffer) = default;

protected:
	T _elements[C];
	size_type _front {0};
	size_type _back {0};
	size_type _size {0};
	static constexpr size_type _capacity {C};

	void destroy_elements();
};


// Copy constructor
template <class T, size_t C>
RingBuffer<T, C>::RingBuffer(const RingBuffer& buffer) : _back(buffer._size), _size(buffer._size) {
	for (size_type i = 0; i < _size; ++i) {
		new(&_elements[(_front + i) % _capacity]) T(buffer._elements[(buffer._front + i) % _capacity]);
	}
}

// Clear method
template <class T, size_t C>
void RingBuffer<T, C>::clear() {
	destroy_elements();
	_front = _back = _size = 0;
}

// Utility function to destroy elements
template <class T, size_t C>
void RingBuffer<T, C>::destroy_elements() {
	while (_size > 0) {
		_front = (_front + 1) % _capacity;
		--_size;
	}
}

// push_back method
template <class T, size_t C>
void RingBuffer<T, C>::push_back(const_reference value) {
	if (_size == _capacity) {
		pop_front();
	}

	_elements[_back] = value;
	_back = (_back + 1) % _capacity;
	++_size;
}

// push_front method
template <class T, size_t C>
void RingBuffer<T, C>::push_front(const_reference value) {
	if (_size == _capacity) {
		pop_back();
	}

	_front = (_front + _capacity - 1) % _capacity;
	_elements[_front] = value;
	++_size;
}

// pop_front method
template <class T, size_t C>
void RingBuffer<T, C>::pop_front(size_type amount) {
	while (amount-- > 0 && _size > 0) {
		_front = (_front + 1) % _capacity;
		--_size;
	}
}

// pop_back method
template <class T, size_t C>
void RingBuffer<T, C>::pop_back(size_type amount) {
	while (amount-- > 0 && _size > 0) {
		_back = (_back + _capacity - 1) % _capacity;
		_elements[_back].~T(); // Call the destructor explicitly.
		--_size;
	}
}

template <class T, size_t C>
typename RingBuffer<T, C>::reference RingBuffer<T, C>::back() {
	return _elements[_back == 0 ? _capacity - 1 : _back - 1];
}


template <class T, size_t C>
typename RingBuffer<T, C>::reference RingBuffer<T, C>::front() {
	return _elements[_front == _capacity? 0 : _front];
}


template <class T, size_t C>
typename RingBuffer<T, C>::size_type RingBuffer<T, C>::size() const {
	return _size;
}


template <class T, size_t C>
typename RingBuffer<T, C>::size_type RingBuffer<T, C>::capacity() const {
	return _capacity;
}


template <class T, size_t C>
bool RingBuffer<T, C>::empty() const {
	return !_size;
}


template <class T, size_t C>
bool RingBuffer<T, C>::full() const {
	return _size == _capacity;
}


template <class T, size_t C>
typename RingBuffer<T, C>::pointer RingBuffer<T, C>::data() {
	return _elements + _front;
}


template <class T, size_t C>
typename RingBuffer<T, C>::const_pointer RingBuffer<T, C>::data() const {
	return _elements;
}


template <class T, size_t C>
typename RingBuffer<T, C>::size_type RingBuffer<T, C>::contiguous() const {
	size_type dist = _capacity - _front;
	return _size < dist ? _size : dist;
}


template <class T, size_t C>
typename RingBuffer<T, C>::reference RingBuffer<T, C>::operator[](size_type index) {
	return _elements[(_front + index) % _capacity];
}


#endif //RINGBUFFER_H
