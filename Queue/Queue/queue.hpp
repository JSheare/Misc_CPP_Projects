#ifndef JADT_QUEUE_HPP
#define JADT_QUEUE_HPP

#include <initializer_list>
#include <stdexcept>

namespace JADT
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

	template <typename T>
	void Queue<T>::push(T item)
	{
		queue.insert(0, item);
	}

	template <typename T>
	T Queue<T>::pop()
	{
		try
		{
			return queue.pop();
		}
		catch (std::out_of_range error)
		{
			throw std::out_of_range("Cannot pop an empty queue");
		}
	}

	template <typename T>
	bool Queue<T>::isEmpty() const
	{
		return queue.isEmpty();
	}

	template <typename T>
	int Queue<T>::size() const
	{
		return queue.length();
	}
}

#endif