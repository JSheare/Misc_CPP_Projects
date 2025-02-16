#ifndef JADT_DEQUEUE_HPP
#define JADT_DEQUEUE_HPP

#include <initializer_list>
#include <stdexcept>

namespace JADT
{
	template <typename T>
	Dequeue<T>::Dequeue()
	{}

	template <typename T>
	Dequeue<T>::Dequeue(std::initializer_list<T> list)
	{
		for (auto& item : list)
		{
			dequeue.append(item);
		}
	}

	template <typename T>
	void Dequeue<T>::addFront(T data)
	{
		dequeue.insert(0, data);
	}

	template <typename T>
	void Dequeue<T>::addRear(T data)
	{
		dequeue.append(data);
	}

	template <typename T>
	T Dequeue<T>::removeFront()
	{
		if (isEmpty())
		{
			throw std::out_of_range("Cannot dequeue from an empty queue");
		}

		return dequeue.remove(0);
	}

	template <typename T>
	T Dequeue<T>::removeRear()
	{
		if (isEmpty())
		{
			throw std::out_of_range("Cannot dequeue from an empty queue");
		}

		return dequeue.pop();
	}

	template <typename T>
	int Dequeue<T>::size() const
	{
		return dequeue.length();
	}

	template <typename T>
	bool Dequeue<T>::isEmpty() const
	{
		return dequeue.isEmpty();
	}
}
#endif