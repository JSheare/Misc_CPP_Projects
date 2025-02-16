#ifndef JADT_STACK_HPP
#define JADT_STACK_HPP

#include <initializer_list>
#include <stdexcept>

namespace JADT
{
	template <typename T>
	Stack<T>::Stack()
	{}

	template <typename T>
	Stack<T>::Stack(std::initializer_list<T> list)
	{
		for (auto& item : list)
		{
			push(item);
		}
	}

	template <typename T>
	void Stack<T>::push(T data)
	{
		stack.append(data);
	}

	template <typename T>
	T Stack<T>::pop()
	{
		try
		{
			return stack.pop();
		}
		catch (std::out_of_range error)
		{
			throw std::out_of_range("Cannot pop from an empty stack");
		}
	}

	template <typename T>
	const T& Stack<T>::peek() const
	{
		if (isEmpty())
		{
			throw std::out_of_range("Cannot peek at an empty stack");
		}

		return stack[stack.length() - 1];
	}

	template <typename T>
	int Stack<T>::size() const
	{
		return stack.length();
	}

	template <typename T>
	bool Stack<T>::isEmpty() const
	{
		return stack.isEmpty();
	}
}
#endif