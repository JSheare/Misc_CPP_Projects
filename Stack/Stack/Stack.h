#ifndef JML_STACK_H
#define JML_STACK_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

#include "Deque.h"

namespace JML
{
	template <typename T>
	class Stack
	{
	public:
		Stack();
		Stack(std::initializer_list<T> list);
		std::size_t size() const;
		bool empty() const;
		template <typename U> void push(U&& item);
		T& top();
		const T& top() const;
		void pop();

	private:
		Deque<T> deque{};
	};
}
#include "Stack.hpp"
#endif