//
// Created by MStefan99 on 29.1.20.
//

#ifndef ARDUI_PAIR_HPP
#define ARDUI_PAIR_HPP

namespace tl {
	template <class T1, class T2>
	struct pair {
		using first_type = T1;
		using second_type = T2;

		pair() = default;
		pair(const T1& x, const T2& y);
		pair(const pair& p) = default;
		pair& operator=(const pair& p);

		T1 first;
		T2 second;
	};


	template <class T1, class T2>
	pair<T1, T2>::pair(const T1& x, const T2& y): first(x), second(y) {
		// Nothing to do
	}


	template <class T1, class T2>
	pair<T1, T2>& pair<T1, T2>::operator=(const pair& p) {
		if (this == &p) {
			return *this;
		} else {
			first = p.first;
			second = p.second;
		}
		return *this;
	}
}

#endif //ARDUI_PAIR_HPP
