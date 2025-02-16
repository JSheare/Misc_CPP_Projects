#ifndef JADT_HEAP_HPP
#define JADT_HEAP_HPP

#include <algorithm>
#include <stdexcept>

namespace JADT
{
	template <typename T, typename U>
	Heap<T, U>::Heap(bool isMax, int capacity)
		: capacity{ capacity }
		, isMax{ isMax }
	{
		keyArray = new T[capacity];
		valueArray = new U[capacity];
	}

	template <typename T, typename U>
	Heap<T, U>::Heap(int capacity)
		: Heap(false, capacity)
	{}

	template <typename T, typename U>
	Heap<T, U>::~Heap()
	{
		delete keyArray;
		delete valueArray;
	}

	// Restores heap property such that the smallest key is at the top of the heap
	template <typename T, typename U>
	void Heap<T, U>::minHeapify(int index)
	{
		while (true)
		{
			int left{ getLeft(index) };
			int right{ getRight(index) };

			int smallest{ index };
			if (left < heapSize && keyArray[left] < keyArray[index])
			{
				smallest = left;
			}
			if (right < heapSize && keyArray[right] < keyArray[smallest])
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
	template <typename T, typename U>
	void Heap<T, U>::maxHeapify(int index)
	{
		while (true)
		{
			int left{ getLeft(index) };
			int right{ getRight(index) };

			int largest{ index };
			if (left < heapSize && keyArray[left] > keyArray[index])
			{
				largest = left;
			}
			if (right < heapSize && keyArray[right] > keyArray[largest])
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
	template <typename T, typename U>
	void Heap<T, U>::insert(T key, U value)
	{
		// Doubles the capacity of the heap if we try and insert while the heap is at capacity
		if (heapSize == capacity)
		{
			T* tempKeys{ new T[2 * capacity] };
			U* tempVals{ new U[2 * capacity] };
			for (int i{ 0 }; i < capacity; ++i)
			{
				tempKeys[i] = keyArray[i];
				tempVals[i] = valueArray[i];
			}

			delete keyArray;
			delete valueArray;
			keyArray = tempKeys;
			valueArray = tempVals;
			capacity = 2 * capacity;
		}

		int i = heapSize;
		keyArray[heapSize] = key;
		valueArray[heapSize++] = value;

		if (isMax)
		{
			while (i != 0 and keyArray[getParent(i)] < keyArray[i])
			{
				swap(i, getParent(i));
				i = getParent(i);
			}
		}
		else
		{
			while (i != 0 and keyArray[getParent(i)] > keyArray[i])
			{
				swap(i, getParent(i));
				i = getParent(i);
			}
		}
	}

	// Returns the top element on the heap without removing it
	template <typename T, typename U>
	U Heap<T, U>::top()
	{
		return valueArray[0];
	}

	// Returns the top element on the heap and removes it
	template <typename T, typename U>
	U Heap<T, U>::extract()
	{
		if (heapSize <= 0)
		{
			throw std::out_of_range("Cannot extract from an empty heap");
		}
		if (heapSize == 1)
		{
			--heapSize;
			return valueArray[0];
		}

		U root = valueArray[0];
		keyArray[0] = keyArray[heapSize - 1];
		valueArray[0] = valueArray[heapSize - 1];
		--heapSize;
		if (isMax)
		{
			maxHeapify(0);
		}
		else
		{
			minHeapify(0);
		}
		return root;
	}

	template <typename T, typename U>
	bool Heap<T, U>::isEmpty()
	{
		return !heapSize;
	}

	template <typename T, typename U>
	int Heap<T, U>::size()
	{
		return heapSize;
	}

	template <typename T, typename U>
	int Heap<T, U>::getParent(int index)
	{
		return (index - 1) / 2;
	}

	template <typename T, typename U>
	int Heap<T, U>::getLeft(int index)
	{
		return (2 * index) + 1;
	}

	template <typename T, typename U>
	int Heap<T, U>::getRight(int index)
	{
		return (2 * index) + 2;
	}

	template <typename T, typename U>
	void Heap<T, U>::swap(int index1, int index2)
	{
		T temp1{ keyArray[index1] };
		U temp2{ valueArray[index1] };
		keyArray[index1] = keyArray[index2];
		keyArray[index2] = temp1;

		valueArray[index1] = valueArray[index2];
		valueArray[index2] = temp2;
	}
}
#endif
