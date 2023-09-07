#ifndef DEQUEUE_H
#define DEQUEUE_H

#include <initializer_list>
#include <stdexcept>

#include "List.h"

namespace JADT
{
	template <typename T>
	class Dequeue
	{
	private:
		List<T> m_dequeue{};

	public:
		Dequeue() = default;

		Dequeue(std::initializer_list<T> list)
		{
			for (T element : list)
			{
				m_dequeue.append(element);
			}
		}

		void addFront(T data)
		{
			m_dequeue.insert(0, data);
		}

		void addRear(T data)
		{
			m_dequeue.append(data);
		}

		T removeFront()
		{
			if (isEmpty())
			{
				throw std::out_of_range("Cannot dequeue from an empty queue");
			}
			
			return m_dequeue.remove(0);
		}

		T removeRear()
		{
			if (isEmpty())
			{
				throw std::out_of_range("Cannot dequeue from an empty queue");
			}

			return m_dequeue.pop();
		}

		int size() const
		{
			return m_dequeue.length();
		}

		bool isEmpty() const
		{
			return m_dequeue.isEmpty();
		}

	};
}

#endif
