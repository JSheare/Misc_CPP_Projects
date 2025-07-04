#ifndef JML_DEQUEUE_HPP
#define JML_DEQUEUE_HPP

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace JML
{
	template <typename T>
	Deque<T>::Deque() :
		mapSize{8}, map{new T*[mapSize]}, frontBlock{3},
		backBlock{frontBlock}
	{
		for (std::size_t i{ 0 }; i < mapSize; ++i)
		{
			map[i] = nullptr;
		}
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
		mapSize {deque.mapSize}, map{new T*[deque.mapSize]}, frontBlock{deque.frontBlock},
		backBlock{deque.backBlock}, numItems{deque.numItems}, startIndex{deque.startIndex}, 
		endIndex{deque.endIndex}
	{
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
		mapSize{deque.mapSize}, map{deque.map}, frontBlock{deque.frontBlock},
		backBlock{deque.backBlock}, numItems{deque.numItems}, startIndex{deque.startIndex}, 
		endIndex{deque.endIndex}
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
		startIndex = deque.startIndex;
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
		startIndex = deque.startIndex;
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
	T& Deque<T>::operator[](std::size_t index)
	{
		return peek(index);
	}

	template <typename T>
	const T& Deque<T>::operator[](std::size_t index) const
	{
		return peek(index);
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

	// Clears all items from the queue
	template <typename T>
	void Deque<T>::clear()
	{
		std::size_t middle{ mapSize/2 };
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

	// Inserts the given item at the given index. Supports perfect forwarding
	template <typename T>
	template <typename U> void Deque<T>::insert(std::size_t index, U&& item)
	{
		if (index >= numItems)
			pushBack(item);
		else if (index <= 0)
			pushFront(item);
		else
		{
			std::size_t copyBlock{ backBlock };
			std::size_t copyIndex{ endIndex };
			std::size_t newBlock{ getBlock(index + 1) };
			std::size_t newIndex{ getIndex(index + 1) };
			incrementBack();
			while (true)
			{
				if (copyBlock == newBlock && copyIndex == newIndex)
					break;

				map[copyBlock][copyIndex] = static_cast<T&&>((copyIndex != 0 ? map[copyBlock][copyIndex - 1] : map[copyBlock - 1][BLOCKSIZE - 1]));
				--copyIndex;
				if (copyIndex == 0)
				{
					--copyBlock;
					copyIndex = BLOCKSIZE - 1;
				}
			}
			map[newBlock][newIndex] = static_cast<U&&>(item);
			++numItems;
		}
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

	// Returns a reference to the item at the specified index. Throws std::out_of_range if the deque is empty
	template <typename T>
	T& Deque<T>::peek(std::size_t index)
	{
		if (index >= numItems || index < 0)
			throw std::out_of_range("index out of range");

		// +1 to account for the reserved empty slot at startIndex
		return map[getBlock(index + 1)][getIndex(index + 1)];
	}

	template <typename T>
	const T& Deque<T>::peek(std::size_t index) const
	{
		if (index >= numItems || index < 0)
			throw std::out_of_range("index out of range");

		// +1 to account for the reserved empty slot at startIndex
		return map[getBlock(index + 1)][getIndex(index + 1)];
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

	// Removes the item at the back of the deque. Throws std::out_of_range if the deque is empty
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

	// Removes the item at the given index. Throws std::out_of_range if the deque is empty
	template <typename T>
	void Deque<T>::remove(std::size_t index)
	{
		if (index >= numItems || index < 0)
			return;
		else if (numItems == 0)
			throw std::out_of_range("no items to pop");
		else
		{
			std::size_t copyBlock{ getBlock(index + 1) };
			std::size_t copyIndex{ getIndex(index + 1) };
			while (true)
			{
				if (copyBlock == backBlock && copyIndex == endIndex)
					break;

				map[copyBlock][copyIndex] = static_cast<T&&>(copyIndex == BLOCKSIZE - 1 ? map[copyBlock + 1][0] : map[copyBlock][copyIndex + 1]);
				++copyIndex;
				if (copyIndex == BLOCKSIZE)
				{
					++copyBlock;
					copyIndex = 0;
				}
			}
			decrementBack();
			--numItems;
			if (numItems == 0)
				incrementStart();
		}
	}

	// Shrinks the deque to the minimum map size and number of blocks necessary to hold all current items
	template <typename T>
	void Deque<T>::shrink_to_fit()
	{
		if (numItems > 0)
		{
			// 2 for the empty entries at start and end index
			std::size_t newMapSize{ (numItems + 2) / BLOCKSIZE + ((numItems + 2) % BLOCKSIZE > 0 ? 1 : 0) };
			T** newMap{ new T * [newMapSize] };
			std::size_t oldBlock{ frontBlock };
			std::size_t oldIndex{ startIndex + 1 };
			if (oldIndex == BLOCKSIZE)
			{
				oldBlock = frontBlock + 1;
				oldIndex = 0;
			}
			std::size_t newBlock{ 0 };
			std::size_t newIndex{ 1 };  // Leaving one empty for start index
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

	// Returns an iterator to the beginning of the deque
	template <typename T>
	Deque<T>::Iterator Deque<T>::begin()
	{
		if (numItems == 0)
			return end();
		else if (startIndex != BLOCKSIZE - 1)
			return Iterator(map + frontBlock, map[frontBlock] + startIndex + 1);
		else
			return Iterator(map + frontBlock + 1, map[frontBlock + 1]);
	}

	// Returns an iterator to the end of the deque
	template <typename T>
	Deque<T>::Iterator Deque<T>::end()
	{
		return Iterator(map + backBlock, map[backBlock] + endIndex);
	}

	// Returns a constant iterator to the beginning of the deque
	template <typename T>
	Deque<T>::ConstIterator Deque<T>::begin() const
	{
		if (numItems == 0)
			return end();
		else if (startIndex != BLOCKSIZE - 1)
			return ConstIterator(map + frontBlock,*map[frontBlock] + startIndex + 1);
		else
			return ConstIterator(map + frontBlock + 1,*map[frontBlock + 1]);
	}

	// Returns a constant iterator to the end of the deque
	template <typename T>
	Deque<T>::ConstIterator Deque<T>::end() const
	{
		return ConstIterator(map + backBlock, map[backBlock] + endIndex);
	}

	// Returns the block of the item at the specified offset from startIndex
	template <typename T>
	std::size_t Deque<T>::getBlock(std::size_t offset)
	{
		return frontBlock + (startIndex + offset) / BLOCKSIZE;
	}

	// Returns the index of the item at the specified offset from startIndex
	template <typename T>
	std::size_t Deque<T>::getIndex(std::size_t offset)
	{
		std::size_t remainder{ BLOCKSIZE - startIndex };
		return remainder > offset ? startIndex + offset : (offset - remainder) % BLOCKSIZE;
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
				++index;
				do
				{
					--index;
					map[index + 1] = map[index];
					if (index == 0)
					{
						map[index] = nullptr;
						break;
					}
				} while (map[index]);

				++frontBlock;
				++backBlock;
				return true;
			}
			return false;
		}
	}

	// Deque bidirectional iterator implementation

	template <typename T>
	Deque<T>::Iterator::Iterator(T** currentBlock, T* current) :
		currentBlock{currentBlock}, current{current}
	{}

	template <typename T>
	T& Deque<T>::Iterator::operator*()
	{
		return *current;
	}

	template <typename T>
	void Deque<T>::Iterator::operator++()
	{
		++current;
		if (current - *currentBlock == BLOCKSIZE)
		{
			++currentBlock;
			current = *currentBlock;
		}
	}

	template <typename T>
	void Deque<T>::Iterator::operator++(int)
	{
		operator++();
	}

	template <typename T>
	void Deque<T>::Iterator::operator--()
	{
		if (current == 0)
		{
			--currentBlock;
			current = *currentBlock[BLOCKSIZE - 1];
		}
		else
			--current;
	}

	template <typename T>
	void Deque<T>::Iterator::operator--(int)
	{
		operator--();
	}

	template <typename T>
	bool Deque<T>::Iterator::operator==(const Iterator& iterator) const
	{
		return currentBlock == iterator.currentBlock && current == iterator.current;
	}

	template <typename T>
	bool Deque<T>::Iterator::operator!=(const Iterator& iterator) const
	{
		return !operator==(iterator);
	}

	// Constant deque iterator implementation

	template <typename T>
	Deque<T>::ConstIterator::ConstIterator(T** currentBlock, T* current) :
		Iterator::currentBlock{currentBlock}, Iterator::current{current}
	{}

	template <typename T>
	const T& Deque<T>::ConstIterator::operator*()
	{
		return Iterator::operator*();
	}
}
#endif