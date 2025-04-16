#ifndef JADT_STACK_HPP
#define JADT_STACK_HPP

#include <cstddef>
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
	std::size_t Stack<T>::size() const
	{
		return deque.size();
	}

	template <typename T>
	bool Stack<T>::empty() const
	{
		return deque.empty();
	}

	template <typename T>
	void Stack<T>::push(const T& item)
	{
		deque.pushBack(item);
	}

	template <typename T>
	T& Stack<T>::top()
	{
		return deque.peekBack();
	}

	template <typename T>
	const T& Stack<T>::top() const
	{
		return deque.peekBack();
	}

	template <typename T>
	void Stack<T>::pop()
	{
		try
		{
			return deque.popBack();
		}
		catch (std::out_of_range error)
		{
			throw std::out_of_range("cannot pop from an empty stack");
		}
	}
}
#endif