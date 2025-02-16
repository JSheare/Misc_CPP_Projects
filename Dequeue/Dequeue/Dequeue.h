#ifndef JADT_DEQUEUE_H
#define JADT_DEQUEUE_H

#include <initializer_list>

#include "List.h"

namespace JADT
{
	template <typename T>
	class Dequeue
	{
	public:
		Dequeue();
		Dequeue(std::initializer_list<T> list);
		void addFront(T data);
		void addRear(T data);
		T removeFront();
		T removeRear();
		int size() const;
		bool isEmpty() const;

	private:
		List<T> dequeue;
	};
}
#include "Dequeue.hpp"
#endif
