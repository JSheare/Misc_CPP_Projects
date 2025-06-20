#ifndef JML_QUEUE_HPP
#define JML_QUEUE_HPP

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace JML
{
	template <typename T>
	Queue<T>::Queue()
	{}

	template <typename T>
	Queue<T>::Queue(std::initializer_list<T> list)
	{
		for (auto& item : list)
		{
			push(item);
		}
	}

	template <typename T1>
	bool operator==(const Queue<T1>& queue1, const Queue<T1>& queue2)
	{
		return queue1.deque == queue2.deque;
	}

	template <typename T1>
	bool operator!=(const Queue<T1>& queue1, const Queue<T1>& queue2)
	{
		return !operator!=(queue1, queue2);
	}

	template <typename T>
	std::size_t Queue<T>::size() const
	{
		return deque.size();
	}

	template <typename T>
	bool Queue<T>::empty() const
	{
		return deque.empty();
	}

	template <typename T>
	template <typename U> void Queue<T>::push(U&& item)
	{
		deque.pushBack(static_cast<U&&>(item));
	}

	template <typename T>
	T& Queue<T>::front()
	{
		return deque.peekFront();
	}

	template <typename T>
	const T& Queue<T>::front() const
	{
		return deque.peekFront();
	}

	template <typename T>
	void Queue<T>::pop()
	{
		try
		{
			return deque.popFront();
		}
		catch (std::out_of_range error)
		{
			throw std::out_of_range("cannot pop an empty queue");
		}
	}
}
#endif