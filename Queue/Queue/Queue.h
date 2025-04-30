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
		template <typename T1> friend bool operator==(const Queue<T1>& queue1, const Queue<T1>& queue2);
		template <typename T1> friend bool operator!=(const Queue<T1>& queue1, const Queue<T1>& queue2);
		std::size_t size() const;
		bool empty() const;
		template <typename U> void push(U&& data);
		T& front();
		const T& front() const;
		void pop();

	private:
		Deque<T> deque{};
	};
}
#include "Queue.hpp"
#endif
