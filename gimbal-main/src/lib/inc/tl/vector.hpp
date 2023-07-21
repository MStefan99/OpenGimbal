//
// Created by MStefan99 on 21.1.2020.
//

#ifndef ARDUI_VECTOR_HPP
#define ARDUI_VECTOR_HPP

#include "allocator.hpp"
#include "iterator.hpp"


namespace tl {
	template <class T>
	struct _vectorIterator {
		using iterator_category = _internal::random_access_iterator_tag;

		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;

		using size_type = unsigned int;
		using difference_type = unsigned int;

		_vectorIterator() = default;
		explicit _vectorIterator(pointer elementPointer);

		_vectorIterator& operator++();
		const _vectorIterator operator++(int);

		_vectorIterator& operator--();
		const _vectorIterator operator--(int);

		bool operator==(const _vectorIterator& it) const;
		bool operator!=(const _vectorIterator& it) const;

		_vectorIterator& operator=(const_reference value);

		pointer operator->() const;
		reference operator*() const;

		_vectorIterator operator+(difference_type n) const;
		_vectorIterator operator-(difference_type n) const;

		_vectorIterator& operator+=(difference_type n);
		_vectorIterator& operator-=(difference_type n);

		difference_type operator-(const _vectorIterator<value_type>& it) const;

		bool operator<(const _vectorIterator& it) const;
		bool operator<=(const _vectorIterator& it) const;
		bool operator>(const _vectorIterator& it) const;
		bool operator>=(const _vectorIterator& it) const;

		reference operator[](size_type n) const;

	protected:
		pointer _elementPointer {nullptr};
	};


	template <class T, class Alloc = tl::allocator<T>>
	struct vector {
		using value_type = T;
		using allocator_type = Alloc;
		using iterator = _vectorIterator<value_type>;

		using pointer = typename allocator_type::pointer;
		using const_pointer = typename allocator_type::const_pointer;
		using reference = typename allocator_type::reference;
		using const_reference = typename allocator_type::const_reference;

		using difference_type = typename allocator_type::difference_type;
		using size_type = typename allocator_type::size_type;


		vector() = default;
		vector(const vector& v);
		~vector();

		void push_back(const_reference value);
		void pop_back();

		iterator insert(iterator pos, const_reference value);
		iterator erase(iterator position);
		iterator erase(iterator first, iterator last);
		void clear();

		reference operator[](size_type n) const;
		vector& operator=(const vector& vector);

		bool empty() const;
		size_type size() const;
		size_type capacity() const;

		pointer data();
		const_pointer data() const;

		void resize(size_type n);
		void reserve(size_type n);

		reference front();
		reference back();

		iterator begin() const;
		iterator end() const;

	protected:
		pointer _vectorArray {nullptr};
		size_type _vectorSize {0};
		size_type _vectorCapacity {0};
	};


	template <class T, class Alloc>
	vector<T, Alloc>::vector(const vector& v):
			_vectorSize(v._vectorSize), _vectorCapacity(v._vectorCapacity) {
		_vectorArray = allocator_type().allocate(_vectorSize);
		for (size_type i {0}; i < v._vectorSize; ++i) {
			allocator_type().construct(_vectorArray + i);
			_vectorArray[i] = v._vectorArray[i];
		}
	}


	template <class T, class Alloc>
	vector<T, Alloc>::~vector() {
		for (size_type i {0}; i < _vectorSize; ++i) {
			allocator_type().destroy(_vectorArray + i);
		}
		allocator_type().deallocate(_vectorArray, _vectorCapacity);
	}


	template <class T, class Alloc>
	void vector<T, Alloc>::push_back(const_reference value) {
		if (_vectorSize == _vectorCapacity) {
			reserve(_vectorCapacity * 3 / 2 + 1);
		}

		allocator_type().construct(_vectorArray + _vectorSize);
		_vectorArray[_vectorSize] = value;
		++_vectorSize;
	}


	template <class T, class Alloc>
	void vector<T, Alloc>::pop_back() {
		--_vectorSize;
	}


	template <class T, class Alloc>
	_vectorIterator<T> vector<T, Alloc>::insert(iterator position, const_reference value) {
		difference_type i {position - begin()};

		if (_vectorSize == _vectorCapacity) {
			reserve(_vectorCapacity * 3 / 2 + 1);
		}
		for (size_type j {_vectorSize}; j > i; --j) {
			_vectorArray[j] = _vectorArray[j - 1];
		}
		_vectorArray[i] = value;
		++_vectorSize;

		return position;
	}


	template <class T, class Alloc>
	_vectorIterator<T> vector<T, Alloc>::erase(_vectorIterator<value_type> position) {
		difference_type i {position - begin()};

		if (i > _vectorSize - 1) {
			return position;
		}
		while (i < _vectorSize - 1) {
			_vectorArray[i] = _vectorArray[i + 1];
			++i;
		}
		--_vectorSize;
		return position;
	}


	template <class T, class Alloc>
	_vectorIterator<T> vector<T, Alloc>::erase(iterator first, iterator last) {
		difference_type i {first - begin()};
		difference_type n {last - first};

		while (i < _vectorSize - n) {
			_vectorArray[i] = _vectorArray[i + n];
			++i;
		}

		_vectorSize -= n;
		return last - n;
	}


	template <class T, class Alloc>
	void vector<T, Alloc>::clear() {
		for (size_type i {0}; i < _vectorSize; ++i) {
			_vectorArray[i] = value_type {};
		}
		_vectorSize = 0;
	}


	template <class T, class Alloc>
	void vector<T, Alloc>::resize(size_type n) {
		if (n > _vectorSize) {  // Expanding, constructing new elements
			if (n > _vectorCapacity) {  // Not enough space, reallocating
				pointer temp = allocator_type().allocate(n);
				for (size_type i {0}; i < n; ++i) {
					allocator_type().construct(temp + i);
				}

				for (size_type i {0}; i < _vectorSize; ++i) {
					temp[i] = _vectorArray[i];
					allocator_type().destroy(_vectorArray + i);  // Destroying old elements if reallocating
				}

				allocator_type().deallocate(_vectorArray, _vectorCapacity);
				_vectorArray = temp;
				_vectorCapacity = n;
			}

			for (size_type i {_vectorSize}; i < n; ++i) {
				allocator_type().construct(_vectorArray + i);
			}

		} else if (n < _vectorSize) {  // Shrinking, destroying extra elements
			for (size_type i {n}; i < _vectorSize; ++i) {
				allocator_type().destroy(_vectorArray + i);
			}
		}
		_vectorSize = n;
	}


	template <class T, class Alloc>
	void vector<T, Alloc>::reserve(size_type n) {
		// Note: reserve() doesn't construct/destroy elements!

		if (n <= _vectorCapacity) {
			return;
		}

		pointer temp = allocator_type().allocate(n);
		for (size_type i {0}; i < n; ++i) {
			allocator_type().construct(temp + i);
		}

		for (size_type i {0}; i < _vectorSize; ++i) {
			temp[i] = _vectorArray[i];
			allocator_type().destroy(_vectorArray + i);  // Destroying old elements if reallocating
		}

		allocator_type().deallocate(_vectorArray, _vectorCapacity);
		_vectorArray = temp;
		_vectorCapacity = n;
	}


	template <class T, class Alloc>
	typename vector<T, Alloc>::reference vector<T, Alloc>::operator[](size_type n) const {
		return _vectorArray[n];
	}


	template <class T, class Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator=(const vector <value_type, allocator_type>& vector) {
		if (this != &vector) {
			for (size_type i {0}; i < _vectorSize; ++i) {
				allocator_type().destroy(_vectorArray + i);
			}

			allocator_type().deallocate(_vectorArray, _vectorCapacity);

			_vectorArray = allocator_type().allocate(vector.size());
			for (size_type i {0}; i < vector.size(); ++i) {
				allocator_type().construct(_vectorArray + i);
				_vectorArray[i] = vector._vectorArray[i];
			}

			_vectorSize = vector.size();
			_vectorCapacity = vector.capacity();
		}
		return *this;
	}


	template <class T, class Alloc>
	bool vector<T, Alloc>::empty() const {
		return _vectorSize == 0;
	}


	template <class T, class Alloc>
	typename vector<T, Alloc>::size_type vector<T, Alloc>::size() const {
		return _vectorSize;
	}


	template <class T, class Alloc>
	typename vector<T, Alloc>::size_type vector<T, Alloc>::capacity() const {
		return _vectorCapacity;
	}


	template <class T, class Alloc>
	typename vector<T, Alloc>::pointer vector<T, Alloc>::data() {
		return _vectorArray;
	}


	template <class T, class Alloc>
	typename vector<T, Alloc>::const_pointer vector<T, Alloc>::data() const {
		return _vectorArray;
	}


	template <class T, class Alloc>
	typename vector<T, Alloc>::reference vector<T, Alloc>::front() {
		return _vectorArray[0];
	}


	template <class T, class Alloc>
	typename vector<T, Alloc>::reference vector<T, Alloc>::back() {
		return _vectorArray[_vectorSize - 1];
	}


	template <class T, class Alloc>
	_vectorIterator<T> vector<T, Alloc>::begin() const {
		return iterator(_vectorArray);
	}


	template <class T, class Alloc>
	_vectorIterator<T> vector<T, Alloc>::end() const {
		return iterator(_vectorArray + _vectorSize);
	}


	template <class T>
	_vectorIterator<T>::_vectorIterator(pointer elementPointer): _elementPointer(elementPointer) {
	}


	template <class T>
	_vectorIterator<T>& _vectorIterator<T>::operator++() {
		++_elementPointer;
		return *this;
	}


	template <class T>
	const _vectorIterator<T> _vectorIterator<T>::operator++(int) { // NOLINT(readability-const-return-type)
		_vectorIterator<value_type> temp {*this};
		++_elementPointer;
		return temp;
	}


	template <class T>
	_vectorIterator<T>& _vectorIterator<T>::operator--() {
		--_elementPointer;
		return *this;
	}


	template <class T>
	const _vectorIterator<T> _vectorIterator<T>::operator--(int) { // NOLINT(readability-const-return-type)
		_vectorIterator<value_type> temp {*this};
		--_elementPointer;
		return temp;
	}


	template <class T>
	_vectorIterator<T> _vectorIterator<T>::operator+(size_type n) const {
		_vectorIterator<value_type> temp {*this};
		temp._elementPointer += n;
		return temp;
	}


	template <class T>
	_vectorIterator<T> _vectorIterator<T>::operator-(size_type n) const {
		_vectorIterator<value_type> temp {*this};
		temp._elementPointer -= n;
		return temp;
	}


	template <class T>
	_vectorIterator<T>& _vectorIterator<T>::operator+=(size_type n) {
		_elementPointer += n;
		return *this;
	}


	template <class T>
	_vectorIterator<T>& _vectorIterator<T>::operator-=(size_type n) {
		_elementPointer -= n;
		return *this;
	}


	template <class T>
	typename _vectorIterator<T>::difference_type
	_vectorIterator<T>::operator-(const _vectorIterator<value_type>& it) const {
		return _elementPointer - it._elementPointer;
	}


	template <class T>
	_vectorIterator<T>& _vectorIterator<T>::operator=(const_reference value) {
		*_elementPointer = value;
		return *this;
	}


	template <class T>
	typename _vectorIterator<T>::pointer _vectorIterator<T>::operator->() const {
		return _elementPointer;
	}


	template <class T>
	typename _vectorIterator<T>::reference _vectorIterator<T>::operator*() const {
		return *_elementPointer;
	}


	template <class T>
	typename _vectorIterator<T>::reference _vectorIterator<T>::operator[](size_type n) const {
		return _elementPointer[n];
	}


	template <class T>
	bool _vectorIterator<T>::operator==(const _vectorIterator<T>& it) const {
		return _elementPointer == it._elementPointer;
	}


	template <class T>
	bool _vectorIterator<T>::operator!=(const _vectorIterator<T>& it) const {
		return _elementPointer != it._elementPointer;
	}


	template <class T>
	bool _vectorIterator<T>::operator<(const _vectorIterator<T>& it) const {
		return _elementPointer < it._elementPointer;
	}


	template <class T>
	bool _vectorIterator<T>::operator<=(const _vectorIterator<T>& it) const {
		return _elementPointer <= it._elementPointer;
	}


	template <class T>
	bool _vectorIterator<T>::operator>(const _vectorIterator<T>& it) const {
		return _elementPointer > it._elementPointer;
	}


	template <class T>
	bool _vectorIterator<T>::operator>=(const _vectorIterator<T>& it) const {
		return _elementPointer >= it._elementPointer;
	}
}

#endif //ARDUI_VECTOR_HPP
