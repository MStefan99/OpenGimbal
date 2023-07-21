//
// Created by MStefan99 on 29.1.20.
//

#ifndef ARDUI_MAP_HPP
#define ARDUI_MAP_HPP

#include "pair.hpp"
#include "less.hpp"
#include "iterator.hpp"
#include "allocator.hpp"


namespace tl {
	template <class pNode>
	static pNode Leftmost(pNode pointer) {
		if (pointer) {
			while (pointer->left) {
				pointer = pointer->left;
			}
		}
		return pointer;
	}


	template <class pNode>
	static pNode Rightmost(pNode pointer) {
		if (pointer) {
			while (pointer->right) {
				pointer = pointer->right;
			}
		}
		return pointer;
	}


	template <class T>
	struct _mapNode {
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;

		explicit _mapNode(const_reference value, _mapNode* parent);
		explicit _mapNode(const typename value_type::first_type& k, _mapNode<value_type>* parent);

		value_type value;
		_mapNode* parent {nullptr};
		_mapNode* left {nullptr};
		_mapNode* right {nullptr};
	};


	template <class T>
	struct _mapIterator {
		typedef _internal::bidirectional_iterator_tag iterator_category;

		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;

		_mapIterator() = default;
		explicit _mapIterator(_mapNode<value_type>* currentElement, _mapNode<value_type>* lastElement = nullptr);

		_mapIterator& operator++();
		const _mapIterator operator++(int);

		_mapIterator& operator--();
		const _mapIterator operator--(int);

		bool operator==(const _mapIterator& it) const;
		bool operator!=(const _mapIterator& it) const;

		_mapIterator& operator=(const_reference it);

		pointer operator->() const;
		reference operator*() const;

		// TODO: make protected?
		_mapNode<value_type>* _currentElement {nullptr};
		_mapNode<value_type>* _lastElement {nullptr};
	};


	template <class Key, class T, class Comp = less<Key>, class Alloc = tl::allocator<pair<const Key, T>>>
	struct map {
		using key_type = Key;
		using mapped_type = T;
		using value_type = tl::pair<const key_type, mapped_type>;
		using key_compare = Comp;

		using allocator_type = Alloc;
		using pointer = typename allocator_type::pointer;
		using const_pointer = typename allocator_type::const_pointer;
		using reference = typename allocator_type::reference;
		using const_reference = typename allocator_type::const_reference;
		using iterator = _mapIterator<value_type>;

		using difference_type = typename allocator_type::difference_type;
		using size_type = typename allocator_type::size_type;
		
		using _node_allocator = typename allocator_type::template rebind<_mapNode<value_type>>::other;


		map() = default;
		map(const map& m);
		~map();

		mapped_type& operator[](const key_type& k);

		bool empty() const;
		size_type size() const;

		iterator find(const key_type& k) const;

		pair<iterator, bool> insert(const value_type& value);

		iterator erase(iterator position);
		size_type erase(const key_type& k);
		iterator erase(iterator first, iterator last);
		void clear();

		iterator begin() const;
		iterator end() const;

	protected:

		void replaceElement(_mapNode<value_type>* o, _mapNode<value_type>* n);
		void destroyElement(_mapNode<value_type>* e);
		_mapNode<value_type>* findElement(const key_type& k) const;

		_mapNode<value_type>* _mapRoot {nullptr};
		key_compare _less {};
		size_type _mapSize {0};
		bool _balancingLeft {true};
	};


	template <class T>
	_mapIterator<T>::_mapIterator(_mapNode<value_type>* currentElement, _mapNode<value_type>* lastElement):
			_currentElement(currentElement),
			_lastElement(lastElement) {
	}


	template <class T>
	_mapIterator<typename _mapIterator<T>::value_type>& _mapIterator<T>::operator++() {
		if (_currentElement->right) {
			_currentElement = Leftmost(_currentElement->right);
			_lastElement = _currentElement;
		} else if (_currentElement->parent) {
			while (_currentElement->parent && _currentElement->parent->right == _currentElement) {
				_currentElement = _currentElement->parent;
			}
			if (_currentElement->parent) {
				_currentElement = _currentElement->parent;
			} else {
				_currentElement = nullptr;
			}
		} else {
			_lastElement = _currentElement;
			_currentElement = nullptr;
		}
		return *this;
	}


	template <class T>
	const _mapIterator<typename _mapIterator<T>::value_type>  // NOLINT(readability-const-return-type)
	_mapIterator<T>::operator++(int) {
		auto temp {*this};
		operator++();
		return temp;
	}


	template <class T>
	_mapIterator<typename _mapIterator<T>::value_type>& _mapIterator<T>::operator--() {
		if (!_currentElement) {
			_currentElement = _lastElement;
		} else if (_currentElement->left) {
			_currentElement = Rightmost(_currentElement->left);
		} else if (_currentElement->parent) {
			while (_currentElement->parent && _currentElement->parent->left == _currentElement) {
				_currentElement = _currentElement->parent;
			}
			if (_currentElement->parent) {
				_currentElement = _currentElement->parent;
			} else {
				_currentElement = nullptr;
			}
		} else {
			_lastElement = _currentElement;
			_currentElement = nullptr;
		}
		return *this;
	}


	template <class T>
	const _mapIterator<typename _mapIterator<T>::value_type>  // NOLINT(readability-const-return-type)
	_mapIterator<T>::operator--(int) {
		auto temp {*this};
		operator--();
		return temp;
	}


	template <class T>
	bool _mapIterator<T>::operator==(const _mapIterator<value_type>& it) const {
		return _currentElement == it._currentElement;
	}


	template <class T>
	bool _mapIterator<T>::operator!=(const _mapIterator<value_type>& it) const {
		return _currentElement != it._currentElement;
	}


	template <class T>
	_mapIterator<typename _mapIterator<T>::value_type>& _mapIterator<T>::operator=(const_reference it) {
		_currentElement->value.second = it;
		return *this;
	}


	template <class T>
	typename _mapIterator<T>::pointer _mapIterator<T>::operator->() const {
		return &_currentElement->value;
	}


	template <class T>
	typename _mapIterator<T>::reference _mapIterator<T>::operator*() const {
		return _currentElement->value;
	}


	template <class T>
	_mapNode<T>::_mapNode(const_reference v, _mapNode<value_type>* p):
			value(v),
			parent(p) {
		// Nothing to do
	}


	template <class T>
	_mapNode<T>::_mapNode(const typename value_type::first_type& k, _mapNode<value_type>* p):
			value(k, typename T::second_type {}),
			parent(p) {
	}


	template <class Key, class T, class Comp, class Alloc>
	bool map<Key, T, Comp, Alloc>::empty() const {
		return !_mapRoot;
	}


	template <class Key, class T, class Comp, class Alloc>
	typename map<Key, T, Comp, Alloc>::size_type map<Key, T, Comp, Alloc>::size() const {
		return _mapSize;
	}


	template <class Key, class T, class Comp, class Alloc>
	typename map<Key, T, Comp, Alloc>::iterator map<Key, T, Comp, Alloc>::find(const key_type& k) const {
		return iterator(findElement(k));
	}


	template <class Key, class T, class Comp, class Alloc>
	tl::pair<typename map<Key, T, Comp, Alloc>::iterator, bool>
	map<Key, T, Comp, Alloc>::insert(const value_type& value) {
		_mapNode<value_type>** node {&_mapRoot};
		_mapNode<value_type>* parent {nullptr};

		while (*node) {
			key_type k = (*node)->value.first;
			if (k == value.first) {
				return {iterator(*node), false};
			} else if (_less(value.first, k)) {
				parent = *node;
				node = &(*node)->left;
			} else {
				parent = *node;
				node = &(*node)->right;
			}
		}
		++_mapSize;
		*node = _node_allocator().allocate();
		_node_allocator().construct(*node, _mapNode<value_type>{value, parent});
		return {iterator(*node), true};
	}


	template <class Key, class T, class Comp, class Alloc>
	typename map<Key, T, Comp, Alloc>::iterator map<Key, T, Comp, Alloc>::erase(iterator position) {
		auto temp {position};
		++position;
		destroyElement(temp._currentElement);
		--_mapSize;

		return position;
	}


	template <class Key, class T, class Comp, class Alloc>
	typename map<Key, T, Comp, Alloc>::size_type map<Key, T, Comp, Alloc>::erase(const key_type& k) {
		_mapNode<value_type>* e {findElement(k)};

		if (e) {
			destroyElement(e);
			--_mapSize;
			return 1;
		} else {
			return 0;
		}
	}


	template <class Key, class T, class Comp, class Alloc>
	typename map<Key, T, Comp, Alloc>::iterator map<Key, T, Comp, Alloc>::erase(iterator first, iterator last) {
		_mapNode<value_type>* node {first._currentElement};
		_mapNode<value_type>* temp;
		bool deleteRoot {false};

		while (node != last._currentElement) {
			if (node == _mapRoot) {
				deleteRoot = true;
			}
			if (node->right) {
				node = Leftmost(node->right);
			} else if (node->parent) {
				while (node->parent && node->parent->right == node) {
					temp = node;
					node = node->parent;
					destroyElement(temp);
					--_mapSize;
				}
				temp = node;
				node = node->parent;
				destroyElement(temp);
				--_mapSize;
			}
		}
		if (deleteRoot && _mapRoot) {
			temp = _mapRoot;
			destroyElement(temp);
			--_mapSize;
		}
		return last;
	}


	template <class Key, class T, class Comp, class Alloc>
	void map<Key, T, Comp, Alloc>::clear() {
		_mapNode<value_type>* node {Leftmost(_mapRoot)};
		_mapNode<value_type>* temp;

		while (node) {
			if (node->right) {
				node = Leftmost(node->right);
			} else if (node->parent) {
				while (node->parent && node->parent->right == node) {
					temp = node;
					node = node->parent;
					destroyElement(temp);
				}
				temp = node;
				node = node->parent;
				if (!temp->right) {
					destroyElement(temp);
				}
			} else if (node == _mapRoot && !node->right && !node->left) {
				destroyElement(node);
				node = nullptr;
			}
		}
		_mapSize = 0;
	}


	template <class Key, class T, class Comp, class Alloc>
	map<Key, T, Comp, Alloc>::map(const map& m) {
		if (m._mapRoot) {
			_mapRoot = _node_allocator().allocate();
			_node_allocator().construct(_mapRoot, _mapNode<value_type>{{m._mapRoot->value}, nullptr});
			_mapSize = 1;

			auto e {m._mapRoot};
			while (e->left) {
				e = e->left;
				insert({e->value});
			}
			while (e != nullptr) {
				if (e->right) {
					e = e->right;
					insert({e->value});
					while (e->left) {
						e = e->left;
						insert({e->value});
					}
				} else if (e->parent) {
					while (e->parent && e->parent->right == e) {
						e = e->parent;
					}
					if (e->parent) {
						e = e->parent;
					} else {
						break;
					}
				} else {
					break;
				}
			}
		}
	}


	template <class Key, class T, class Comp, class Alloc>
	map<Key, T, Comp, Alloc>::~map() {
		clear();
	}


	template <class Key, class T, class Comp, class Alloc>
	typename map<Key, T, Comp, Alloc>::mapped_type& map<Key, T, Comp, Alloc>::operator[](const key_type& k) {
		_mapNode<value_type>** node {&_mapRoot};
		_mapNode<value_type>* parent {_mapRoot};

		while (*node) {
			key_type cKey = (*node)->value.first;
			if (cKey == k) {
				return (*node)->value.second;
			} else if (_less(k, cKey)) {
				parent = *node;
				node = &(*node)->left;
			} else {
				parent = *node;
				node = &(*node)->right;
			}
		}
		++_mapSize;
		*node = _node_allocator().allocate();
		_node_allocator().construct(*node, _mapNode<value_type>{k, parent});
		return (*node)->value.second;
	}


	template <class Key, class T, class Comp, class Alloc>
	typename map<Key, T, Comp, Alloc>::iterator map<Key, T, Comp, Alloc>::begin() const {
		return map::iterator(Leftmost(_mapRoot));
	}


	template <class Key, class T, class Comp, class Alloc>
	typename map<Key, T, Comp, Alloc>::iterator map<Key, T, Comp, Alloc>::end() const {
		return map::iterator(nullptr, Rightmost(_mapRoot));
	}


	template <class Key, class T, class Comp, class Alloc>
	void map<Key, T, Comp, Alloc>::replaceElement(_mapNode<value_type>* o, _mapNode<value_type>* n) {
		if (o->parent) {
			if (o == o->parent->left) {
				o->parent->left = n;
			} else {
				o->parent->right = n;
			}
		}
		if (o->right) {
			o->right->parent = n;
		}
		if (o->left) {
			o->left->parent = n;
		}
		n->parent = o->parent;
	}


	template <class Key, class T, class Comp, class Alloc>
	void map<Key, T, Comp, Alloc>::destroyElement(_mapNode<value_type>* e) {
		_mapNode<value_type>* replacement {};

		if (e->left && e->right) {
			if (_balancingLeft) {
				replacement = Rightmost(e->left);
				replacement->right = e->right;
			} else {
				replacement = Leftmost(e->right);
				replacement->left = e->left;
			}
			_balancingLeft = !_balancingLeft;
			replaceElement(e, replacement);
		} else if (e->left) {
			replaceElement(e, e->left);
		} else if (e->right) {
			replaceElement(e, e->right);
		} else {
			if (e->parent) {  // check if tree root is being unlinked
				if (e->parent->left == e) {
					e->parent->left = nullptr;
				} else {
					e->parent->right = nullptr;
				}
			}
		}
		if (e == _mapRoot) {
			_mapRoot = replacement;
		}
		_node_allocator().destroy(e);
		_node_allocator().deallocate(e);
	}


	template <class Key, class T, class Comp, class Alloc>
	_mapNode<typename map<Key, T, Comp, Alloc>::value_type>*
	map<Key, T, Comp, Alloc>::findElement(const key_type& k) const {
		_mapNode<value_type>* node {_mapRoot};

		while (node) {
			if (_less(k, node->value.first)) {
				node = node->left;
			} else if (_less(node->value.first, k)) {
				node = node->right;
			} else {
				return node;
			}
		}
		return nullptr;
	}
}

#endif //ARDUI_MAP_HPP
