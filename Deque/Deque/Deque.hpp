#ifndef JML_DEQUEUE_HPP
#define JML_DEQUEUE_HPP

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace JML
{
	template <typename T>
	Deque<T>::Deque()
	{
		mapSize = 3;
		map = new T*[mapSize];
		for (std::size_t i{ 0 }; i < mapSize; ++i)
		{
			map[i] = nullptr;
		}
		//  Starting at the middle to prevent early resizings
		frontBlock = backBlock = mapSize / 2;
		map[frontBlock] = new T[BLOCKSIZE];
	}

	template <typename T>
	Deque<T>::Deque(std::initializer_list<T> list) : 
		Deque<T>::Deque()
	{
		for (auto& item : list)
		{
			pushBack(item);
		}
	}

	// Copy constructor
	template <typename T>
	Deque<T>::Deque(const Deque<T>& deque) :
		mapSize{deque.mapSize}, frontBlock{deque.frontBlock}, backBlock{deque.backBlock}, 
		numItems{deque.numItems}, startIndex{deque.numItems}, endIndex{deque.endIndex}
	{
		map = new T*[mapSize];
		for (std::size_t i{ 0 }; i < mapSize; ++i)
		{
			if (deque.map[i])
			{
				map[i] = new T[BLOCKSIZE];
				for (std::size_t j{ 0 }; j < BLOCKSIZE; ++j)
				{
					map[i][j] = deque.map[i][j];
				}
			}
			else
				map[i] = nullptr;
		}
	}

	// Move constructor
	template <typename T>
	Deque<T>::Deque(Deque<T>&& deque) noexcept :
		map{deque.map}, mapSize{deque.mapSize}, frontBlock{deque.frontBlock}, 
		backBlock {deque.backBlock}, numItems{deque.numItems}, 
		startIndex{deque.startIndex}, endIndex{deque.endIndex}
	{
		// Allocating exactly one block so that the old deque is still valid after the move
		deque.map = new T*[1];
		deque.mapSize = 1;
		deque.frontBlock = 0;
		deque.backBlock = 0;
		deque.numItems = 0;
		deque.startIndex = 0;
		deque.endIndex = 0;
		deque.map[0] = new T[BLOCKSIZE];
	}

	template <typename T>
	Deque<T>::~Deque()
	{
		for (std::size_t i{ 0 }; i < mapSize; ++i)
		{
			delete[] map[i];
		}
		delete[] map;
	}

	// Copy assignment
	template <typename T>
	Deque<T>& Deque<T>::operator=(const Deque<T>& deque)
	{
		if (&deque == this)
			return *this;

		for (std::size_t i{ 0 }; i < mapSize; ++i)
		{
			delete[] map[i];
		}
		delete[] map;

		map = new T*[deque.mapSize];
		mapSize = deque.mapSize;
		frontBlock = deque.frontBlock;
		backBlock = deque.backBlock;
		numItems = deque.numItems;
		startIndex = deque.numItems;
		endIndex = deque.endIndex;
		for (std::size_t i{ 0 }; i < mapSize; ++i)
		{
			if (deque.map[i])
			{
				map[i] = new T[BLOCKSIZE];
				for (std::size_t j{ 0 }; j < BLOCKSIZE; ++j)
				{
					map[i][j] = deque.map[i][j];
				}
			}
			else
				map[i] = nullptr;
		}
		return *this;
	}

	// Move assignment
	template <typename T>
	Deque<T>& Deque<T>::operator=(Deque<T>&& deque) noexcept
	{
		for (std::size_t i{ 0 }; i < mapSize; ++i)
		{
			delete[] map[i];
		}
		delete[] map;
		
		map = deque.map;
		mapSize = deque.mapSize;
		frontBlock = deque.frontBlock;
		backBlock = deque.backBlock;
		numItems = deque.numItems;
		startIndex = deque.numItems;
		endIndex = deque.endIndex;

		// Allocating exactly one block so that the old deque is still valid after the move
		deque.map = new T*[1];
		deque.mapSize = 1;
		deque.frontBlock = 0;
		deque.numItems = 0;
		deque.startIndex = 0;
		deque.endIndex = 0;
		deque.map[0] = new T[BLOCKSIZE];

		return *this;
	}

	template <typename T>
	bool operator==(const Deque<T>& deque1, const Deque<T>& deque2)
	{
		if (deque1.numItems == deque2.numItems)
		{
			std::size_t itemsLeft{ deque1.numItems };
			std::size_t block1{ deque1.frontBlock };
			std::size_t block2{ deque2.frontBlock };
			std::size_t index1{ deque1.startIndex };
			std::size_t index2{ deque2.startIndex };
			while (itemsLeft)
			{
				if (deque1.map[block1][index1] != deque2.map[block2][index2])
					return false;

				++index1;
				++index2;
				if (index1 == deque1.BLOCKSIZE)
				{
					++block1;
					index1 = 0;
				}
				if (index2 == deque2.BLOCKSIZE)
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
	bool operator!=(const Deque<T>& deque1, const Deque<T>& deque2)
	{
		return !operator==(deque1, deque2);
	}

	// Returns the number of items in the deque
	template <typename T>
	std::size_t Deque<T>::size() const
	{
		return numItems;
	}

	// Returns true if the deque has no items
	template <typename T>
	bool Deque<T>::empty() const
	{
		return numItems == 0;
	}

	// Pushes the given item to the front of the deque. Supports perfect forwarding
	template <typename T>
	template <typename U> void Deque<T>::pushFront(U&& item)
	{
		map[frontBlock][startIndex] = static_cast<U&&>(item);
		++numItems;
		decrementStart();
		if (numItems == 1)
			incrementBack();
	}

	// Pushes the given item to the back of the deque. Supports perfect forwarding
	template <typename T>
	template <typename U> void Deque<T>::pushBack(U&& item)
	{
		map[backBlock][endIndex] = static_cast<U&&>(item);
		++numItems;
		incrementBack();
		if (numItems == 1)
			decrementStart();
	}

	// Returns a reference to the item at the front of the deque. Throws std::out_of_range if the deque is empty
	template <typename T>
	T& Deque<T>::peekFront()
	{
		if (numItems == 0)
			throw std::out_of_range("no items to peek");

		if (startIndex == BLOCKSIZE - 1)
			return map[frontBlock + 1][0];
		else
			return map[frontBlock][startIndex + 1];
	}

	template <typename T>
	const T& Deque<T>::peekFront() const
	{
		if (numItems == 0)
			throw std::out_of_range("no items to peek");

		if (startIndex == BLOCKSIZE - 1)
			return map[frontBlock + 1][0];
		else
			return map[frontBlock][startIndex + 1];
	}

	// Returns a reference to the item at the back of the deque. Throws std::out_of_range if the deque is empty
	template <typename T>
	T& Deque<T>::peekBack()
	{
		if (numItems == 0)
			throw std::out_of_range("no items to peek");

		if (endIndex == 0)
			return map[backBlock - 1][BLOCKSIZE - 1];
		else
			return map[backBlock][endIndex - 1];
	}

	template <typename T>
	const T& Deque<T>::peekBack() const
	{
		if (numItems == 0)
			throw std::out_of_range("no items to peek");

		if (endIndex == 0)
			return map[backBlock - 1][BLOCKSIZE - 1];
		else
			return map[backBlock][endIndex - 1];
	}

	// Removes the item at the front of the deque. Throws std::out_of_range if the deque is empty
	template <typename T>
	void Deque<T>::popFront()
	{
		if (numItems == 0)
			throw std::out_of_range("no items to pop");

		--numItems;
		incrementStart();
		if (numItems == 0)
			decrementBack();
	}

	// Removes hte item at the back of the deque. Throws std::out_of_range if the deque is empty
	template <typename T>
	void Deque<T>::popBack()
	{
		if (numItems == 0)
			throw std::out_of_range("no items to pop");

		--numItems;
		decrementBack();
		if (numItems == 0)
			incrementStart();
	}

	// Resizes the deque to 2x its current size
	template <typename T>
	void Deque<T>::resize()
	{
		std::size_t newMapSize{ mapSize * 2 };
		T** newMap{ new T*[newMapSize] };
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

	// Increments the pointer to the front of the deque by one
	template <typename T>
	void Deque<T>::incrementStart()
	{
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

	// Decrements the pointer to the front of the deque by one
	template <typename T>
	void Deque<T>::decrementStart()
	{
		if (startIndex > 0)
			--startIndex;
		else
		{
			// Attempting to slide everything to the right. Resizing if that isn't possible
			if (frontBlock == 0)
			{
				bool success{ slide(false) };
				if (!success)
				{
					resize();
					slide(false);  // Doing a slide to make a one block space before the front
				}
			}
			if (!map[frontBlock - 1])
			{
				// Using the spare from the back if it exists
				if (backBlock + 1 < mapSize && map[backBlock + 1])
				{
					map[frontBlock - 1] = map[backBlock + 1];
					map[backBlock + 1] = nullptr;
				}
				// Allocating a new block if it doesn't
				else
					map[frontBlock - 1] = new T[BLOCKSIZE];
			}
			--frontBlock;
			startIndex = BLOCKSIZE - 1;
		}
	}

	// Increments the pointer to the end of the deque by one
	template <typename T>
	void Deque<T>::incrementBack()
	{
		++endIndex;
		if (endIndex == BLOCKSIZE)
		{
			// Attempting to slide everything to the left. Resizing if that isn't possible
			if (backBlock == mapSize - 1)
			{
				bool success{ slide(true) };
				if (!success)
					resize();
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

	// Decrements the pointer to the end of the deque by one
	template <typename T>
	void Deque<T>::decrementBack()
	{
		if (endIndex > 0)
			--endIndex;
		else
		{
			--backBlock;
			endIndex = BLOCKSIZE - 1;
			// Leaving a maximum of one spare block at the back
			if (backBlock + 2 < mapSize)
			{
				delete[] map[backBlock + 2];
				map[backBlock + 2] = nullptr;
			}
		}
	}

	// Attempts to slide all blocks to either the left or right depending on the given parameter. Returns true if successful and false otherwise
	template <typename T>
	bool Deque<T>::slide(bool left)
	{
		if (left)
		{
			std::size_t index{ frontBlock };
			// Accounting for the spare block
			if (index > 0 && map[index - 1])
				--index;

			if (index > 0)
			{
				while (map[index])
				{
					map[index - 1] = map[index];
					++index;
				}
				map[index] = nullptr;
				--frontBlock;
				--backBlock;
				return true;
			}
			return false;
		}
		else
		{
			std::size_t index{ backBlock };
			// Accounting for the spare block
			if (index < mapSize - 1 && map[index + 1])
				++index;

			if (index < mapSize - 1)
			{
				while (map[index])
				{
					map[index + 1] = map[index];
					if (index == 0)
						break;
					
					--index;
				}
				map[index] = nullptr;
				++frontBlock;
				++backBlock;
				return true;
			}
			return false;
		}
	}
}
#endif