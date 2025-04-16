#ifndef JADT_QUEUE_H
#define JADT_QUEUE_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

#include "Deque.h"

namespace JADT
{
	template <typename T>
	class Queue
	{
	public:
		Queue();
		Queue(std::initializer_list<T> list);
		std::size_t size() const;
		bool empty() const;
		void push(const T& data);
		T& front();
		const T& front() const;
		void pop();

	private:
		Deque<T> deque{};
	};
}
#include "Queue.hpp"
#endif
