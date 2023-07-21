//
// Created by MStefan99 on 19.7.21.
//

#ifndef ARDUI_ITERATOR_HPP
#define ARDUI_ITERATOR_HPP

namespace tl {
	namespace _internal {
		struct bidirectional_iterator_tag {
		};

		struct random_access_iterator_tag: public bidirectional_iterator_tag {
		};


		template <class Iterator>
		struct iterator {
			typedef typename Iterator::iterator_category iterator_category;
		};


		template <class Iterator>
		void _advance(Iterator& it, unsigned int n, bidirectional_iterator_tag t) {
			while (n--) {
				++it;
			}
		}


		template <class Iterator>
		void _advance(Iterator& it, unsigned int n, random_access_iterator_tag t) {
			it += n;
		}
	}


	template <class Iterator>
	void advance(Iterator& it, unsigned int n) {
		_internal::_advance(it, n, typename _internal::iterator<Iterator>::iterator_category());
	}
}

#endif //ARDUI_ITERATOR_HPP
