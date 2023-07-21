//
// Created by MStefan99 on 3.4.20.
//

#ifndef ARDUI_QUEUE_HPP
#define ARDUI_QUEUE_HPP

namespace tl {
	template <class T, class Container = list <T>>
	class queue {
	public:
		queue() = default;
		~queue() = default;

		bool empty() const;
		unsigned int size() const;

		T& front() const;
		T& back() const;

		void pop();
		void push(const T& value);

	protected:
		Container _c {};
	};


	template <class T, class Container>
	bool queue<T, Container>::empty() const {
		return _c.empty();
	}


	template <class T, class Container>
	unsigned int queue<T, Container>::size() const {
		return _c.size();
	}


	template <class T, class Container>
	T& queue<T, Container>::front() const {
		return _c.front();
	}


	template <class T, class Container>
	T& queue<T, Container>::back() const {
		return _c.back();
	}


	template <class T, class Container>
	void queue<T, Container>::pop() {
		_c.pop_front();
	}


	template <class T, class Container>
	void queue<T, Container>::push(const T& value) {
		_c.push_back(value);
	}
}

#endif //ARDUI_QUEUE_HPP
