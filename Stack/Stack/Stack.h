#ifndef JADT_STACK_H
#define JADT_STACK_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

#include "Deque.h"

namespace JADT
{
	template <typename T>
	class Stack
	{
	public:
		Stack();
		Stack(std::initializer_list<T> list);
		std::size_t size() const;
		bool empty() const;
		void push(const T& item);
		T& top() const;
		void pop();

	private:
		Deque<T> deque;
	};
}
#include "Stack.hpp"
#endif