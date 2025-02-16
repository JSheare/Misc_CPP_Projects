#ifndef JADT_QUEUE_H
#define JADT_QUEUE_H

#include <initializer_list>
#include <stdexcept>

#include "List.h"

namespace JADT
{
	template <typename T>
	class Queue
	{
	public:
		Queue();
		Queue(std::initializer_list<T> list);
		void push(T data);
		T pop();
		bool isEmpty() const;
		int size() const;

	private:
		List<T> queue;
	};
}
#include "Queue.hpp"
#endif
