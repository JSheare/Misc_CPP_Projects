#ifndef HEAP_H
#define HEAP_H

#include <exception>

namespace JADT
{
	template <typename T, typename U>
	class Heap
	{
	private:
		T* m_keyArray{nullptr};
		U* m_valueArray{ nullptr };
		int m_heapSize{ 0 };
		int m_capacity{};
		bool m_isMax{};

	public:
		Heap(bool isMax=false, int capacity=10)
			: m_capacity{ capacity }
			, m_isMax{ isMax }
		{
			m_keyArray = new T[m_capacity];
			m_valueArray = new U[m_capacity];
		}

		Heap(int capacity)
			: Heap(false, capacity)
		{}

		~Heap()
		{
			delete m_keyArray;
			delete m_valueArray;
		}

		// Restores heap property such that the smallest key is at the top of the heap
		void minHeapify(int index)
		{
			while (true)
			{
				int left{ getLeft(index) };
				int right{ getRight(index) };

				int smallest{ index };
				if (left < m_heapSize && m_keyArray[left] < m_keyArray[index])
				{
					smallest = left;
				}
				if (right < m_heapSize && m_keyArray[right] < m_keyArray[smallest])
				{
					smallest = right;
				}
				if (smallest != index)
				{
					swap(index, smallest);
					index = smallest;
					continue;
				}

				break;
			}
		}

		// Restores heap property such that the largest key is at the top of the heap
		void maxHeapify(int index)
		{
			while (true)
			{
				int left{ getLeft(index) };
				int right{ getRight(index) };

				int largest{ index };
				if (left < m_heapSize && m_keyArray[left] > m_keyArray[index])
				{
					largest = left;
				}
				if (right < m_heapSize && m_keyArray[right] > m_keyArray[largest])
				{
					largest = right;
				}
				if (largest != index)
				{
					swap(index, largest);
					index = largest;
					continue;
				}

				break;
			}
		}

		// Inserts a new key/value pair onto the heap
		void insert(T key, U value)
		{
			// Doubles the capacity of the heap if we try and insert while the heap is at capacity
			if (m_heapSize == m_capacity)
			{
				T* tempKeys{ new T[2 * m_capacity] };
				U* tempVals{ new U[2 * m_capacity] };
				for (int i{ 0 }; i < m_capacity; ++i)
				{
					tempKeys[i] = m_keyArray[i];
					tempVals[i] = m_valueArray[i];
				}

				delete m_keyArray;
				delete m_valueArray;
				m_keyArray = tempKeys;
				m_valueArray = tempVals;
				m_capacity = 2 * m_capacity;
			}

			int i = m_heapSize;
			m_keyArray[m_heapSize] = key;
			m_valueArray[m_heapSize++] = value;

			if (m_isMax)
			{
				while (i != 0 and m_keyArray[getParent(i)] < m_keyArray[i])
				{
					swap(i, getParent(i));
					i = getParent(i);
				}
			}
			else
			{
				while (i != 0 and m_keyArray[getParent(i)] > m_keyArray[i])
				{
					swap(i, getParent(i));
					i = getParent(i);
				}
			}
		}

		// Returns the top element on the heap without removing it
		U top()
		{
			return m_valueArray[0];
		}

		// Returns the top element on the heap and removes it
		U extract()
		{
			if (m_heapSize <= 0)
			{
				throw std::out_of_range("Cannot extract from an empty heap");
			}
			if (m_heapSize == 1)
			{
				--m_heapSize;
				return m_valueArray[0];
			}

			U root = m_valueArray[0];
			m_keyArray[0] = m_keyArray[m_heapSize - 1];
			m_valueArray[0] = m_valueArray[m_heapSize - 1];
			--m_heapSize;
			if (m_isMax)
			{
				maxHeapify(0);
			}
			else
			{
				minHeapify(0);
			}
			return root;
		}

		bool isEmpty()
		{
			return !m_heapSize;
		}

		int size()
		{
			return m_heapSize;
		}

	private:
		int getParent(int index)
		{
			return (index - 1) / 2;
		}

		int getLeft(int index)
		{
			return (2 * index) + 1;
		}

		int getRight(int index)
		{
			return (2 * index) + 2;
		}

		void swap(int index1, int index2)
		{
			T temp1{ m_keyArray[index1] };
			U temp2{ m_valueArray[index1] };
			m_keyArray[index1] = m_keyArray[index2];
			m_keyArray[index2] = temp1;

			m_valueArray[index1] = m_valueArray[index2];
			m_valueArray[index2] = temp2;
		}

	};
}

#endif
