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
