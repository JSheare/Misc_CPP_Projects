#ifndef JML_DEQUEUE_H
#define JML_DEQUEUE_H

#include <cstddef>
#include <initializer_list>

namespace JML
{
	template <typename T>
	class Deque
	{
	public:
		Deque();
		Deque(std::initializer_list<T> list);
		Deque(const Deque<T>& deque);  // Copy constructor
		Deque(Deque<T>&& deque) noexcept;  // Move constructor
		~Deque();
		Deque<T>& operator=(const Deque<T>& deque);  // Copy assignment
		Deque<T>& operator=(Deque<T>&& deque) noexcept;  // Move assignment
		friend bool operator==(const Deque<T>& deque1, const Deque<T>& deque2);
		friend bool operator!=(const Deque<T>& deque1, const Deque<T>& deque2);
		std::size_t size() const;
		bool empty() const;
		template <typename U> void pushFront(U&& item);
		template <typename U> void pushBack(U&& item);
		T& peekFront();
		const T& peekFront() const;
		T& peekBack();
		const T& peekBack() const;
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
