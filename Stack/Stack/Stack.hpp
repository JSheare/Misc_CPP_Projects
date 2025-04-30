#ifndef JML_STACK_HPP
#define JML_STACK_HPP

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace JML
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

	template <typename T1>
	bool operator==(const Stack<T1>& stack1, const Stack<T1>& stack2)
	{
		return stack1.deque == stack2.deque;
	}

	template <typename T1>
	bool operator!=(const Stack<T1>& stack1, const Stack<T1>& stack2)
	{
		return !operator==(stack1, stack2);
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
	template <typename U> void Stack<T>::push(U&& item)
	{
		deque.pushBack(static_cast<U&&>(item));
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