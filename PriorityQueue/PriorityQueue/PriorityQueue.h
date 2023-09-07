#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <stdexcept>

#include "List.h"

namespace JADT
{
	template <typename T>
	struct Pair
	{
		int m_key{};
		T m_value{};
	};


	template <typename U>
	class PriorityQueue
	{
	private:
		List<Pair<U>> m_queue{};
		bool m_maxQueue{};

	public:
		PriorityQueue(bool maxQueue = false)
			: m_maxQueue{ maxQueue }
		{};

		void push(int key, U value)
		{
			m_queue.insert(0, Pair<U>{key, value});
			
			if (m_maxQueue)
			{
				m_queue.sort(maxQueue);
			}
			else
			{
				m_queue.sort(minQueue);
			}
		}

		U pop()
		{
			if (isEmpty())
			{
				throw std::out_of_range("Cannot pop an empty queue");
			}

			return m_queue.pop().m_value;
		}

		bool isEmpty() const
		{
			return m_queue.isEmpty();
		}

		int size() const
		{
			return m_queue.length();
		}

		void setMinQueue()
		{
			m_maxQueue = false;
		}

		void setMaxQueue()
		{
			m_maxQueue = true;
		}

	private:
		static bool minQueue(Pair<U>& x, Pair<U>& y)
		{
			return x.m_key >= y.m_key;
		}

		static bool maxQueue(Pair<U>& x, Pair<U>& y)
		{
			return x.m_key <= y.m_key;
		}
	};
}

#endif
