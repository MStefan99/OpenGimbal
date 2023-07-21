//
// Created by MStefan99 on 4.2.20.
//

#ifndef ARDUI_STACK_HPP
#define ARDUI_STACK_HPP

namespace tl {
	template <class T, class Container = list <T>>
	class stack {
	public:
		stack() = default;
		~stack() = default;

		bool empty() const;
		unsigned int size() const;

		T& top() const;

		void pop();
		void push(const T& value);

	protected:
		Container _c {};
	};


	template <class T, class Container>
	bool stack<T, Container>::empty() const {
		return _c.empty();
	}


	template <class T, class Container>
	unsigned int stack<T, Container>::size() const {
		return _c.size();
	}


	template <class T, class Container>
	T& stack<T, Container>::top() const {
		return _c.back();
	}


	template <class T, class Container>
	void stack<T, Container>::pop() {
		_c.pop_back();
	}


	template <class T, class Container>
	void stack<T, Container>::push(const T& value) {
		_c.push_back(value);
	}
}

#endif //ARDUI_STACK_HPP
