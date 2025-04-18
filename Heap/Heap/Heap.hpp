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

	// Copy constructor
	template <typename T, bool (*comparator)(const T&, const T&)>
	Heap<T, comparator>::Heap(const Heap<T, comparator>& heap) :
		capacity{heap.capacity}, heapSize{heap.heapSize}, items{new T[capacity]}
	{
		for (std::size_t i{ 0 }; i < capacity; ++i)
		{
			items[i] = heap.items[i];
		}
	}

	// Move constructor
	template <typename T, bool (*comparator)(const T&, const T&)>
	Heap<T, comparator>::Heap(Heap<T, comparator>&& heap) noexcept :
		capacity{heap.capacity}, heapSize{heap.heapSize}, items{heap.items}
	{
		// Allocating a capacity of exactly one so that the old heap is still valid after the move
		heap.capacity = 1;
		heap.heapSize = 0;
		heap.items = new T[1];
	}

	template <typename T, bool (*comparator)(const T&, const T&)>
	Heap<T, comparator>::~Heap()
	{
		delete[] items;
	}

	// Copy assignment
	template <typename T, bool (*comparator)(const T&, const T&)>
	Heap<T, comparator>& Heap<T, comparator>::operator=(const Heap<T, comparator>& heap)
	{
		if (&heap == this)
			return *this;
		
		delete[] items;
		capacity = heap.capacity;
		heapSize = heap.heapSize;
		items = new T[heap.capacity];
		for (std::size_t i{ 0 }; i < capacity; ++i)
		{
			items[i] = heap.items[i];
		}
		return *this;
	}

	// Move assignment
	template <typename T, bool (*comparator)(const T&, const T&)>
	Heap<T, comparator>& Heap<T, comparator>::operator=(Heap<T, comparator>&& heap)
	{
		delete[] items;
		capacity = heap.capacity;
		heapSize = heap.heapSize;
		items = heap.items;

		// Allocating a capacity of exactly one so that the old heap is still valid after the move
		heap.capacity = 1;
		heap.heapSize = 0;
		heap.items = new T[1];

		return *this;
	}

	// Returns the number of items in the heap
	template <typename T, bool (*comparator)(const T&, const T&)>
	std::size_t Heap<T, comparator>::size() const
	{
		return heapSize;
	}

	// Returns true if the heap has no items
	template <typename T, bool (*comparator)(const T&, const T&)>
	bool Heap<T, comparator>::empty() const
	{
		return heapSize == 0;
	}

	// Inserts the given item into the heap. Supports perfect forwarding
	template <typename T, bool (*comparator)(const T&, const T&)>
	template <typename U> void Heap<T, comparator>::insert(U&& item)
	{
		if (heapSize == capacity)
			resize();
		
		items[heapSize] = static_cast<U&&>(item);
		heapifyUp(heapSize++);
	}

	// Returns a reference to the item at the top of the heap. Raises std::range_error if the heap is empty
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

	// Removes the item at the top of the heap
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

	// Returns the parent index of the given index
	template <typename T, bool (*comparator)(const T&, const T&)>
	std::size_t Heap<T, comparator>::getParent(std::size_t index) const
	{
		return (index - 1) / 2;
	}

	// Returns the left child index of the given index
	template <typename T, bool (*comparator)(const T&, const T&)>
	std::size_t Heap<T, comparator>::getLeft(std::size_t index) const
	{
		return 2 * index + 1;
	}

	// Returns the right child index of the given index
	template <typename T, bool (*comparator)(const T&, const T&)>
	std::size_t Heap<T, comparator>::getRight(std::size_t index) const
	{
		return 2 * index + 2;
	}

	// Restores the heap property upwards starting at the given index
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

	// Restores the heap property down starting at the given index
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

	// Swaps the items at the given indices
	template <typename T, bool (*comparator)(const T&, const T&)>
	void Heap<T, comparator>::swap(std::size_t index1, std::size_t index2)
	{
		T temp{ items[index1] };
		items[index1] = items[index2];
		items[index2] = temp;
	}

	// Resizes the heap to 2x its current size
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