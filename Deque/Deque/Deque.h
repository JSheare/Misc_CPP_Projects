#ifndef JADT_DEQUEUE_H
#define JADT_DEQUEUE_H

#include <cstddef>
#include <initializer_list>

namespace JADT
{
	template <typename T>
	class Deque
	{
	public:
		Deque();
		Deque(std::initializer_list<T> list);
		~Deque();
		std::size_t size() const;
		bool empty() const;
		void pushFront(const T& item);
		void pushBack(const T& item);
		T& peekFront() const;
		T& peekBack() const;
		void popFront();
		void popBack();

	private:
		static const std::size_t BLOCKSIZE{ 8 };
		T** map{ nullptr };
		std::size_t mapSize{ 0 };
		std::size_t frontBlock{ 0 };
		std::size_t backBlock{ 0 };

		std::size_t numItems{ 0 };
		std::size_t startIndex{ 0 };
		std::size_t endIndex{ 0 };

		void resize(bool resizeFront = true);
		void incrementStart();
		void decrementStart();
		void incrementEnd();
		void decrementEnd();

	};
}
#include "Deque.hpp"
#endif
