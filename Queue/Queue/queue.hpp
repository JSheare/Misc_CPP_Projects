#ifndef JML_QUEUE_HPP
#define JML_QUEUE_HPP

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace JML
{
	template <typename T>
	Queue<T>::Queue() :
		mapSize{ 8 }, map{ new T*[mapSize] }, frontBlock{ 0 },
		backBlock{ frontBlock }
	{
		for (std::size_t i{ 0 }; i < mapSize; ++i)
		{
			map[i] = nullptr;
		}
		map[frontBlock] = new T[BLOCKSIZE];
	}

	template <typename T>
	Queue<T>::Queue(std::initializer_list<T> list) :
		Queue<T>::Queue()
	{
		for (auto& item : list)
		{
			push(item);
		}
	}

	// Copy constructor
	template <typename T>
	Queue<T>::Queue(const Queue<T>& queue) :
		mapSize{ queue.mapSize }, map{ new T*[queue.mapSize] }, frontBlock{ queue.frontBlock },
		backBlock{ queue.backBlock }, numItems{ queue.numItems }, startIndex{ queue.startIndex },
		endIndex{ queue.endIndex }
	{
		for (std::size_t i{ 0 }; i < mapSize; ++i)
		{
			if (queue.map[i])
			{
				map[i] = new T[BLOCKSIZE];
				for (std::size_t j{ 0 }; j < BLOCKSIZE; ++j)
				{
					map[i][j] = queue.map[i][j];
				}
			}
			else
				map[i] = nullptr;
		}
	}

	// Move constructor
	template <typename T>
	Queue<T>::Queue(Queue<T>&& queue) noexcept :
		mapSize{ queue.mapSize }, map{ queue.map }, frontBlock{ queue.frontBlock },
		backBlock{ queue.backBlock }, numItems{ queue.numItems }, startIndex{ queue.startIndex },
		endIndex{ queue.endIndex }
	{
		// Allocating exactly one block so that the old queue is still valid after the move
		queue.map = new T*[1];
		queue.mapSize = 1;
		queue.frontBlock = 0;
		queue.backBlock = 0;
		queue.numItems = 0;
		queue.startIndex = 0;
		queue.endIndex = 0;
		queue.map[0] = new T[BLOCKSIZE];
	}

	template <typename T>
	Queue<T>::~Queue()
	{
		for (std::size_t i{ 0 }; i < mapSize; ++i)
		{
			delete[] map[i];
		}
		delete[] map;
	}

	// Copy assignment
	template <typename T>
	Queue<T>& Queue<T>::operator=(const Queue<T>& queue)
	{
		if (&queue == this)
			return *this;

		for (std::size_t i{ 0 }; i < mapSize; ++i)
		{
			delete[] map[i];
		}
		delete[] map;

		map = new T*[queue.mapSize];
		mapSize = queue.mapSize;
		frontBlock = queue.frontBlock;
		backBlock = queue.backBlock;
		numItems = queue.numItems;
		startIndex = queue.startIndex;
		endIndex = queue.endIndex;
		for (std::size_t i{ 0 }; i < mapSize; ++i)
		{
			if (queue.map[i])
			{
				map[i] = new T[BLOCKSIZE];
				for (std::size_t j{ 0 }; j < BLOCKSIZE; ++j)
				{
					map[i][j] = queue.map[i][j];
				}
			}
			else
				map[i] = nullptr;
		}
		return *this;
	}

	// Move assignment
	template <typename T>
	Queue<T>& Queue<T>::operator=(Queue<T>&& queue) noexcept
	{
		for (std::size_t i{ 0 }; i < mapSize; ++i)
		{
			delete[] map[i];
		}
		delete[] map;

		map = queue.map;
		mapSize = queue.mapSize;
		frontBlock = queue.frontBlock;
		backBlock = queue.backBlock;
		numItems = queue.numItems;
		startIndex = queue.startIndex;
		endIndex = queue.endIndex;

		// Allocating exactly one block so that the old queue is still valid after the move
		queue.map = new T * [1];
		queue.mapSize = 1;
		queue.frontBlock = 0;
		queue.numItems = 0;
		queue.startIndex = 0;
		queue.endIndex = 0;
		queue.map[0] = new T[BLOCKSIZE];

		return *this;
	}

	template <typename T>
	bool operator==(const Queue<T>& queue1, const Queue<T>& queue2)
	{
		if (queue1.numItems == queue2.numItems)
		{
			std::size_t itemsLeft{ queue1.numItems };
			std::size_t block1{ queue1.frontBlock };
			std::size_t block2{ queue2.frontBlock };
			std::size_t index1{ queue1.startIndex };
			std::size_t index2{ queue2.startIndex };
			while (itemsLeft)
			{
				if (queue1.map[block1][index1] != queue2.map[block2][index2])
					return false;

				++index1;
				++index2;
				if (index1 == queue1.BLOCKSIZE)
				{
					++block1;
					index1 = 0;
				}
				if (index2 == queue2.BLOCKSIZE)
				{
					++block2;
					index2 = 0;
				}
				--itemsLeft;
			}
			return true;
		}
		return false;
	}

	template <typename T>
	bool operator!=(const Queue<T>& queue1, const Queue<T>& queue2)
	{
		return !operator==(queue1, queue2);
	}

	// Returns the number of items in the deque
	template <typename T>
	std::size_t Queue<T>::size() const
	{
		return numItems;
	}

	// Returns true if the deque has no items
	template <typename T>
	bool Queue<T>::empty() const
	{
		return numItems == 0;
	}

	// Clears all items from the queue
	template <typename T>
	void Queue<T>::clear()
	{
		std::size_t middle{ mapSize / 2 };
		if (middle > 0)
		{
			while (frontBlock < middle - 1)
			{
				delete[] map[frontBlock];
				map[frontBlock++] = nullptr;
			}
			while (backBlock > middle + 1)
			{
				delete[] map[backBlock];
				map[backBlock--] = nullptr;
			}
		}
		startIndex = endIndex = numItems = 0;
	}

	// Pushes the given item to the back of the queue. Supports perfect forwarding
	template <typename T>
	template <typename U> void Queue<T>::push(U&& item)
	{
		map[backBlock][endIndex] = static_cast<U&&>(item);
		++numItems;
		++endIndex;
		if (endIndex == BLOCKSIZE)
		{
			// If we're at the end of the map, attempting to slide everything to the left. Resizing if that isn't possible
			if (backBlock == mapSize - 1)
			{
				std::size_t index{ frontBlock };
				// Accounting for the spare block
				if (index > 0 && map[index - 1])
					--index;

				if (index > 0)
				{
					--index;
					do
					{
						++index;
						map[index - 1] = map[index];
						if (index == mapSize - 1)
						{
							map[index] = nullptr;
							break;
						}
					} while (map[index]);
					--frontBlock;
					--backBlock;
				}
				else
				{
					// Increase the size of the map by 2x its current size
					std::size_t newMapSize{ mapSize * 2 };
					T** newMap{ new T * [newMapSize] };
					for (std::size_t i{ 0 }; i < mapSize; ++i)
					{
						newMap[i] = map[i];
					}
					for (std::size_t i{ mapSize }; i < newMapSize; ++i)
					{
						newMap[i] = nullptr;
					}
					delete[] map;
					map = newMap;
					mapSize = newMapSize;
				}
			}
			// Using the spare from the front if it exists
			if (frontBlock >= 1 && map[frontBlock - 1])
			{
				map[backBlock + 1] = map[frontBlock - 1];
				map[frontBlock - 1] = nullptr;
			}
			// Allocating a new block if it doesn't
			else
				map[backBlock + 1] = new T[BLOCKSIZE];

			++backBlock;
			endIndex = 0;
		}
	}

	// Returns a reference to the item at the front of the queue. Throws std::out_of_range if the queue is empty
	template <typename T>
	T& Queue<T>::front()
	{
		if (numItems == 0)
			throw std::out_of_range("no items to peek");

		return map[frontBlock][startIndex];
	}

	template <typename T>
	const T& Queue<T>::front() const
	{
		if (numItems == 0)
			throw std::out_of_range("no items to peek");

		return map[frontBlock][startIndex];
	}

	// Removes the item at the front of the queue. Throws std::out_of_range if the queue is empty
	template <typename T>
	void Queue<T>::pop()
	{
		if (numItems == 0)
			throw std::out_of_range("no items to pop");

		--numItems;
		++startIndex;
		if (startIndex == BLOCKSIZE)
		{
			++frontBlock;
			startIndex = 0;
			// Leaving a maximum of one spare block at the front
			if (frontBlock >= 2)
			{
				delete[] map[frontBlock - 2];
				map[frontBlock - 2] = nullptr;
			}
		}
	}

	// Shrinks the queue to the minimum map size and number of blocks necessary to hold all current items
	template <typename T>
	void Queue<T>::shrink_to_fit()
	{
		if (numItems > 0)
		{
			// 1 for the empty entry at the end index
			std::size_t newMapSize{ (numItems + 1) / BLOCKSIZE + ((numItems + 1) % BLOCKSIZE > 0 ? 1 : 0) };
			T** newMap{ new T * [newMapSize] };
			// Reducing the number of partially full blocks by copying into the new blocks from beginning to end
			std::size_t oldBlock{ frontBlock };
			std::size_t oldIndex{ startIndex };
			std::size_t newBlock{ 0 };
			std::size_t newIndex{ 0 };
			std::size_t count{ numItems };
			while (newBlock < newMapSize)
			{
				newMap[newBlock] = new T[BLOCKSIZE];
				while (newIndex < BLOCKSIZE)
				{
					// Leaving one empty for end index
					if (count == 0)
					{
						backBlock = newBlock;
						endIndex = newIndex;
						break;
					}
					newMap[newBlock][newIndex] = map[oldBlock][oldIndex];
					--count;
					++oldIndex;
					++newIndex;
					if (oldIndex == BLOCKSIZE)
					{
						oldIndex = 0;
						++oldBlock;
					}
				}
				newIndex = 0;
				++newBlock;
			}
			frontBlock = startIndex = 0;
			for (std::size_t i{ 0 }; i < mapSize; ++i)
			{
				delete[] map[i];
			}
			delete[] map;
			map = newMap;
			mapSize = newMapSize;
		}
		else
		{
			T** newMap{ new T*[1] };
			newMap[0] = map[frontBlock];
			map[frontBlock] = nullptr;
			frontBlock = backBlock = startIndex = endIndex = 0;
			for (std::size_t i{ 0 }; i < mapSize; ++i)
			{
				delete[] map[i];
			}
			delete[] map;
			map = newMap;
			mapSize = 1;
		}
	}
}
#endif