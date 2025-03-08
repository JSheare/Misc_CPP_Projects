#ifndef JADT_HEAP_HPP
#define JADT_HEAP_HPP

#include <cstddef>
#include <stdexcept>

namespace JADT
{
	template <typename T>
	bool heapGreater(const T& x, const T& y)
	{
		return x > y;
	}

	template <typename T, bool (*comparator)(const T&, const T&)>
	Heap<T, comparator>::Heap(std::size_t capacity) :
		capacity{capacity}, items{new T[capacity]}
	{}

	template <typename T, bool (*comparator)(const T&, const T&)>
	Heap<T, comparator>::~Heap()
	{
		delete[] items;
	}

	template <typename T, bool (*comparator)(const T&, const T&)>
	std::size_t Heap<T, comparator>::size() const
	{
		return heapSize;
	}

	template <typename T, bool (*comparator)(const T&, const T&)>
	bool Heap<T, comparator>::empty() const
	{
		return heapSize == 0;
	}

	template <typename T, bool (*comparator)(const T&, const T&)>
	void Heap<T, comparator>::insert(const T& item)
	{
		if (heapSize == capacity)
			resize();
		
		items[heapSize] = item;
		heapifyUp(heapSize++);
	}

	template <typename T, bool (*comparator)(const T&, const T&)>
	T& Heap<T, comparator>::top()
	{
		if (heapSize == 0)
			throw std::range_error("Cannot return the top of an empty heap");

		return items[0];
	}

	template <typename T, bool (*comparator)(const T&, const T&)>
	const T& Heap<T, comparator>::top() const
	{
		if (heapSize == 0)
			throw std::range_error("Cannot return the top of an empty heap");

		return items[0];
	}

	template <typename T, bool (*comparator)(const T&, const T&)>
	void Heap<T, comparator>::pop()
	{
		if (heapSize == 0)
			throw std::range_error("Cannot pop an empty heap");

		if (heapSize == 1)
			--heapSize;
		else
		{
			items[0] = items[heapSize - 1];
			--heapSize;
			heapifyDown(0);
		}
	}

	template <typename T, bool (*comparator)(const T&, const T&)>
	std::size_t Heap<T, comparator>::getParent(std::size_t index) const
	{
		return (index - 1) / 2;
	}

	template <typename T, bool (*comparator)(const T&, const T&)>
	std::size_t Heap<T, comparator>::getLeft(std::size_t index) const
	{
		return 2 * index + 1;
	}

	template <typename T, bool (*comparator)(const T&, const T&)>
	std::size_t Heap<T, comparator>::getRight(std::size_t index) const
	{
		return 2 * index + 2;
	}

	template <typename T, bool (*comparator)(const T&, const T&)>
	void Heap<T, comparator>::heapifyUp(std::size_t index)
	{
		while (index > 0)
		{
			std::size_t parentIndex{ getParent(index) };
			if (comparator(items[index], items[parentIndex]))
			{
				swap(index, parentIndex);
				index = parentIndex;
			}
			else
				break;
		}
	}

	template <typename T, bool (*comparator)(const T&, const T&)>
	void Heap<T, comparator>::heapifyDown(std::size_t index)
	{
		while (true)
		{
			std::size_t leftIndex{ getLeft(index) };
			std::size_t rightIndex{ getRight(index) };
			std::size_t smallest{ index };
			if (leftIndex < heapSize && comparator(items[leftIndex], items[index]))
				smallest = leftIndex;

			if (rightIndex < heapSize && comparator(items[rightIndex], items[smallest]))
				smallest = rightIndex;

			if (smallest != index)
			{
				swap(index, smallest);
				index = smallest;
				continue;
			}
			break;
		}
	}

	template <typename T, bool (*comparator)(const T&, const T&)>
	void Heap<T, comparator>::swap(std::size_t index1, std::size_t index2)
	{
		T temp{ items[index1] };
		items[index1] = items[index2];
		items[index2] = temp;
	}

	template <typename T, bool (*comparator)(const T&, const T&)>
	void Heap<T, comparator>::resize()
	{
		capacity *= 2;
		T* newItems{ new T[capacity] };
		for (std::size_t i{ 0 }; i < heapSize; ++i)
		{
			newItems[i] = items[i];
		}
		delete[] items;
		items = newItems;
	}
}
#endif