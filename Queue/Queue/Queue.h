#ifndef JML_QUEUE_H
#define JML_QUEUE_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

#include "Deque.h"

namespace JML
{
	template <typename T>
	class Queue
	{
	public:
		Queue();
		Queue(std::initializer_list<T> list);
		Queue(const Queue<T>& queue);  // Copy constructor
		Queue(Queue<T>&& queue) noexcept;  // Move constructor
		~Queue();
		Queue<T>& operator=(const Queue<T>& queue);  // Copy assignment
		Queue<T>& operator=(Queue<T>&& queue) noexcept;  // Move assignment
		friend bool operator==(const Queue<T>& queue1, const Queue<T>& queue2);
		friend bool operator!=(const Queue<T>& queue1, const Queue<T>& queue2);
		std::size_t size() const;
		bool empty() const;
		void clear();
		template <typename U> void push(U&& item);
		T& front();
		const T& front() const;
		void pop();
		void shrink_to_fit();

	private:
		static constexpr std::size_t BLOCKSIZE{ 512 / sizeof(T) ? 512 / sizeof(T) : 1 };
		std::size_t mapSize{ 0 };
		T** map{ nullptr };
		std::size_t frontBlock{ 0 };
		std::size_t backBlock{ 0 };

		std::size_t numItems{ 0 };
		std::size_t startIndex{ 0 };  // Points to the front item
		std::size_t endIndex{ 0 };  // Points to one past the back item
	};
}
#include "Queue.hpp"
#endif
