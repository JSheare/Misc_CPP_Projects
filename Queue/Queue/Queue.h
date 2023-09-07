#ifndef QUEUE_H
#define QUEUE_H

#include <initializer_list>
#include <stdexcept>

#include "List.h"

namespace JADT
{
	template <typename T>
	class Queue
	{
	private:
		List<T> m_queue{};
	public:
		Queue() = default;

		Queue(std::initializer_list<T> list)
		{
			for (T element : list)
			{
				push(element);
			}
		}

		void push(T data)
		{
			m_queue.insert(0, data);
		}

		T pop()
		{
			try
			{
				return m_queue.pop();
			}
			catch (std::out_of_range error)
			{
				throw std::out_of_range("Cannot pop an empty queue");
			}
		}

		bool isEmpty() const
		{
			return m_queue.isEmpty();
		}

		int size() const
		{
			return m_queue.length();
		}

	};
}

#endif
