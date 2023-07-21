//
// Created by MStefan99 on 25.11.19.
//

#ifndef ARDUI_LIST_HPP
#define ARDUI_LIST_HPP

#include "allocator.hpp"
#include "iterator.hpp"


namespace tl {
	template <class T>
	struct _listNode {
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;

		_listNode(const_reference value, _listNode* prev, _listNode* next);
		_listNode(const _listNode& listElement) = default;
		~_listNode() = default;

		value_type elementValue;
		_listNode* prevElement {nullptr};
		_listNode* nextElement {nullptr};
	};


	template <class T>
	struct _listIterator {  // bidirectional _listIterator<T>
		using iterator_category = _internal::bidirectional_iterator_tag;

		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;

		_listIterator() = default;
		explicit _listIterator(_listNode<T>* elementPointer);

		_listIterator& operator++();
		const _listIterator operator++(int);

		_listIterator& operator--();
		const _listIterator operator--(int);

		bool operator==(const _listIterator& it) const;
		bool operator!=(const _listIterator& it) const;

		_listIterator& operator=(const T&);

		T* operator->() const;
		T& operator*() const;

		// TODO: make protected?
		_listNode<T>* _elementPointer {nullptr};
		_listNode<T>* _lastPointer {nullptr};
	};


	template <class T, class Alloc = tl::allocator<T>>
	struct list {
		using value_type = T;
		using allocator_type = Alloc;
		using iterator = _listIterator<value_type>;

		using pointer = typename allocator_type::pointer;
		using const_pointer = typename allocator_type::const_pointer;
		using reference = typename allocator_type::reference;
		using const_reference = typename allocator_type::const_reference;

		using difference_type = typename allocator_type::difference_type;
		using size_type = typename allocator_type::size_type;

		using _node_allocator = typename allocator_type::template rebind<_listNode<value_type>>::other;

		list() = default;
		list(const list& list);
		~list();

		void push_back(const_reference value);
		void push_front(const_reference value);

		reference front() const;
		reference back() const;

		void pop_back();
		void pop_front();

		size_type remove(const_reference value);
		size_type remove_if(bool (* predicate)(const_reference));

		_listIterator<value_type> insert(_listIterator<value_type> position, const_reference value);

		_listIterator<value_type> erase(_listIterator<value_type> position);
		_listIterator<value_type> erase(_listIterator<value_type> first, _listIterator<value_type> last);
		void clear();

		size_type size() const;
		bool empty() const;

		list& operator=(const list& list);

		_listIterator<value_type> begin() const;
		_listIterator<value_type> end() const;

	protected:
		_listNode<value_type>* _first {nullptr};
		_listNode<value_type>* _last {nullptr};

		void destroyElement(_listNode<value_type>* e);
		size_type _listSize {0};
	};


	template <class T>
	_listNode<T>::_listNode(const_reference value, _listNode<value_type>* prev, _listNode<value_type>* next):
			elementValue(value), prevElement(prev), nextElement(next) {
		// Nothing to do
	}


	template <class T, class Alloc>
	list<T, Alloc>::list(const list& l) {
		for (auto e {l._first}; e != nullptr; e = e->nextElement) {
			push_back(e->elementValue);
		}
		_listSize = l.size();
	}


	template <class T, class Alloc>
	list<T, Alloc>::~list() {
		clear();
	}


	template <class T, class Alloc>
	void list<T, Alloc>::push_back(const_reference value) {
		_listNode<value_type>* e = _node_allocator().allocate();
		_node_allocator().construct(e, value, _last, nullptr);

		if (!_first) {
			_first = e;
		}
		if (_last) {
			_last->nextElement = e;
		}
		_last = e;
		++_listSize;
	}


	template <class T, class Alloc>
	void list<T, Alloc>::push_front(const_reference value) {
		_listNode<value_type>* e = _node_allocator().allocate();
		_node_allocator().construct(e, value, nullptr, _first);

		if (!_last) {
			_last = e;
		}
		if (_first) {
			_first->prevElement = e;
		}
		_first = e;
		++_listSize;
	}


	template <class T, class Alloc>
	void list<T, Alloc>::pop_back() {
		auto e {_last};
		if (e) {
			if (e->prevElement) {
				e->prevElement->nextElement = nullptr;
			} else {
				_first = nullptr;
			}

			_last = e->prevElement;
			--_listSize;
			_node_allocator().destroy(e);
			_node_allocator().deallocate(e);
		}
	}


	template <class T, class Alloc>
	void list<T, Alloc>::pop_front() {
		auto e {_first};
		if (e) {
			if (e->nextElement) {
				e->nextElement->prevElement = nullptr;
			} else {
				_last = nullptr;
			}

			_first = e->nextElement;
			--_listSize;
			_node_allocator().destroy(e);
			_node_allocator().deallocate(e);
		}
	}


	template <class T, class Alloc>
	typename list<T, Alloc>::reference list<T, Alloc>::front() const {
		return _first->elementValue;
	}


	template <class T, class Alloc>
	typename list<T, Alloc>::reference list<T, Alloc>::back() const {
		return _last->elementValue;
	}


	template <class T, class Alloc>
	void list<T, Alloc>::destroyElement(_listNode<value_type>* e) {
		if (e->nextElement) {
			e->nextElement->prevElement = e->prevElement;
		} else {
			_last = e->prevElement;
		}
		if (e->prevElement) {
			e->prevElement->nextElement = e->nextElement;
		} else {
			_first = e->nextElement;
		}
		_node_allocator().destroy(e);
		_node_allocator().deallocate(e);
	}


	template <class T, class Alloc>
	_listIterator<typename list<T, Alloc>::value_type>
	list<T, Alloc>::insert(_listIterator<value_type> position, const_reference value) {
		auto e {position._elementPointer};
		_listNode<value_type>* n = _node_allocator().allocate();
		_node_allocator().construct(n, value, nullptr, e);

		if (e) {
			if (e->prevElement) {
				n->prevElement = e->prevElement;
				e->prevElement->nextElement = n;
			} else {
				_first = n;
			}
			e->prevElement = n;
		} else {
			if (!_first) {
				_first = n;
			}
			if (_last) {
				_last->nextElement = n;
				n->prevElement = _last;
			}
			_last = n;
		}
		++_listSize;
		return _listIterator < T > (n);
	}


	template <class T, class Alloc>
	_listIterator<typename list<T, Alloc>::value_type> list<T, Alloc>::erase(_listIterator<value_type> position) {
		auto temp = position;

		++position;
		destroyElement(temp._elementPointer);
		--_listSize;
		return position;
	}


	template <class T, class Alloc>
	_listIterator<typename list<T, Alloc>::value_type>
	list<T, Alloc>::erase(_listIterator<value_type> f, _listIterator<value_type> l) {
		while (f != l) {
			auto temp {f};
			++f;
			destroyElement(temp._elementPointer);
			--_listSize;
		}
		return l;
	}


	template <class T, class Alloc>
	typename list<T, Alloc>::size_type list<T, Alloc>::remove(const_reference value) {
		size_type i {0};
		_listNode<value_type>* e {_first};

		while (e) {
			auto temp = e;
			e = e->nextElement;
			if (temp->elementValue == value) {
				destroyElement(temp);
				++i;
				--_listSize;
			}
		}
		return i;
	}


	template <class T, class Alloc>
	typename list<T, Alloc>::size_type list<T, Alloc>::remove_if(bool (* p)(const_reference)) {
		size_type i {0};
		_listNode<value_type>* e {_first};

		while (e) {
			auto temp = e;
			e = e->nextElement;
			if (p(temp->elementValue)) {
				destroyElement(temp);
				++i;
				--_listSize;
			}
		}
		return i;
	}


	template <class T, class Alloc>
	void list<T, Alloc>::clear() {
		_listNode<value_type>* e {_first};

		while (e) {
			auto temp = e;
			e = e->nextElement;
			destroyElement(temp);
			_listSize = 0;
		}
	}


	template <class T, class Alloc>
	typename list<T, Alloc>::size_type list<T, Alloc>::size() const {
		return _listSize;
	}


	template <class T, class Alloc>
	bool list<T, Alloc>::empty() const {
		return _first == nullptr;
	}


	template <class T, class Alloc>
	_listIterator<T> list<T, Alloc>::begin() const {
		return _listIterator <value_type> {_first};
	}


	template <class T, class Alloc>
	_listIterator<T> list<T, Alloc>::end() const {
		_listIterator <value_type> it {};
		it._lastPointer = _last;
		return it;
	}


	template <class T, class Alloc>
	list<T, Alloc>& list<T, Alloc>::operator=(const list& l) {
		if (this != &l) {
			clear();
			for (auto p = l._first; p != nullptr; p = p->nextElement) {
				push_back(p->elementValue);
			}
		}
		return *this;
	}


	template <class T>
	_listIterator<T>::_listIterator(_listNode<T>* elementPointer): _elementPointer(elementPointer) {}


	template <class T>
	_listIterator<T>& _listIterator<T>::operator++() {
		if (!_elementPointer->nextElement) {
			_lastPointer = _elementPointer;
		}
		_elementPointer = _elementPointer->nextElement;
		return *this;
	}


	template <class T>
	const _listIterator<T> _listIterator<T>::operator++(int) { // NOLINT(readability-const-return-type)
		_listIterator < T > temp {*this};
		operator++();
		return temp;
	}


	template <class T>
	_listIterator<T>& _listIterator<T>::operator--() {
		if (!_elementPointer) {
			_elementPointer = _lastPointer;
		} else {
			_elementPointer = _elementPointer->prevElement;
		}
		return *this;
	}


	template <class T>
	const _listIterator<T> _listIterator<T>::operator--(int) { // NOLINT(readability-const-return-type)
		_listIterator < T > temp {*this};
		operator--();
		return temp;
	}


	template <class T>
	bool _listIterator<T>::operator==(const _listIterator<T>& it) const {
		return _elementPointer == it._elementPointer;
	}


	template <class T>
	bool _listIterator<T>::operator!=(const _listIterator<T>& it) const {
		return _elementPointer != it._elementPointer;
	}


	template <class T>
	_listIterator<T>& _listIterator<T>::operator=(const T& value) {
		_elementPointer->elementValue = value;
		return *this;
	}


	template <class T>
	T* _listIterator<T>::operator->() const {
		return &_elementPointer->elementValue;
	}


	template <class T>
	T& _listIterator<T>::operator*() const {
		return _elementPointer->elementValue;
	}
}

#endif //ARDUI_LIST_HPP
