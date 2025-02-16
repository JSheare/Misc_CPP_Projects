#ifndef JADT_STACK_H
#define JADT_STACK_H

#include <initializer_list>
#include <stdexcept>

#include "List.h"

namespace JADT
{
	template <typename T>
	class Stack
	{
	private:
		List<T> m_stack{};

	public:
		Stack();
		Stack(std::initializer_list<T> list);
		void push(T data);
		T pop();
		const T& peek() const;
		int size() const;
		bool isEmpty() const;

	private:
		List<T> stack;
	};
}
#include "Stack.hpp"
#endif