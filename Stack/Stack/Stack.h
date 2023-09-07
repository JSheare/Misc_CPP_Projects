#ifndef STACK_H
#define STACK_H

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
		Stack() = default;

		Stack(std::initializer_list<T> list)
		{
			for (T element : list)
			{
				push(element);
			}
		}

		void push(T data)
		{
			m_stack.append(data);
		}

		T pop()
		{
			try
			{
				return m_stack.pop();
			}
			catch (std::out_of_range error)
			{
				throw std::out_of_range("Cannot pop from an empty stack");
			}
		}

		const T& peek()
		{
			if (isEmpty())
			{
				throw std::out_of_range("Cannot peek at an empty stack");
			}

			return m_stack[m_stack.length() - 1];
		}

		int size() const
		{
			return m_stack.length();
		}

		bool isEmpty() const
		{
			return m_stack.isEmpty();
		}
	};
}

#endif