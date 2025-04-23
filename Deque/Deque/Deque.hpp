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
		mapSize = 8;
		map = new T*[mapSize];
		for (std::size_t i{ 0 }; i < mapSize; ++i)
		{
			map[i] = nullptr;
		}
		//  Starting towards the middle to prevent early resizings
		frontBlock = backBlock = 3;
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
		map = new T*[deque.mapSize];
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
		mapSize{ deque.mapSize }, frontBlock{ deque.frontBlock }, backBlock{ deque.backBlock },
		numItems{ deque.numItems }, startIndex{ deque.numItems }, endIndex{ deque.endIndex }, 
		map{deque.map}
	{
		// Allocating exactly one block so that the old deque is still valid after the move
		deque.mapSize = 1;
		deque.frontBlock = 0;
		deque.backBlock = 0;
		deque.numItems = 0;
		deque.startIndex = 0;
		deque.endIndex = 0;
		deque.map = new T*[1];
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

		mapSize = deque.mapSize;
		frontBlock = deque.frontBlock;
		backBlock = deque.backBlock;
		numItems = deque.numItems;
		startIndex = deque.numItems;
		endIndex = deque.endIndex;
		map = new T*[deque.mapSize];
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
		
		mapSize = deque.mapSize;
		frontBlock = deque.frontBlock;
		backBlock = deque.backBlock;
		numItems = deque.numItems;
		startIndex = deque.numItems;
		endIndex = deque.endIndex;
		map = deque.map;

		// Allocating exactly one block so that the old deque is still valid after the move
		deque.mapSize = 1;
		deque.frontBlock = 0;
		deque.backBlock = 0;
		deque.numItems = 0;
		deque.startIndex = 0;
		deque.endIndex = 0;
		deque.map = new T*[1];
		deque.map[0] = new T[BLOCKSIZE];

		return *this;
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
			incrementEnd();
	}

	// Pushes the given item to the back of the deque. Supports perfect forwarding
	template <typename T>
	template <typename U> void Deque<T>::pushBack(U&& item)
	{
		map[backBlock][endIndex] = static_cast<U&&>(item);
		++numItems;
		incrementEnd();
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
	}

	// Removes hte item at the back of the deque. Throws std::out_of_range if the deque is empty
	template <typename T>
	void Deque<T>::popBack()
	{
		if (numItems == 0)
			throw std::out_of_range("no items to pop");

		--numItems;
		decrementEnd();
	}

	// Resizes the queue to 2x its current size, with a bias toward the front if the given argument is true, and the back otherwise
	template <typename T>
	void Deque<T>::resize(bool resizeFront)
	{
		std::size_t newMapSize{ mapSize * 2 };
		T** newMap{ new T*[newMapSize] };
		if (resizeFront)
		{
			std::size_t divider{ newMapSize - mapSize };
			for (std::size_t i{ 0 }; i < divider; ++i)
			{
				newMap[i] = nullptr;
			}
			for (std::size_t i{ divider }; i < newMapSize; ++i)
			{
				newMap[i] = map[i - divider];
			}
			frontBlock += divider;
			backBlock += divider;
		}
		else
		{
			for (std::size_t i{ 0 }; i < mapSize; ++i)
			{
				newMap[i] = map[i];
			}
			for (std::size_t i{ mapSize }; i < newMapSize; ++i)
			{
				newMap[i] = nullptr;
			}
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
			if (frontBlock == 0)
				resize(true);

			if (!map[frontBlock - 1])
				map[frontBlock - 1] = new T[BLOCKSIZE];

			--frontBlock;
			startIndex = BLOCKSIZE - 1;
		}
	}

	// Increments the pointer to the end of the deque by one
	template <typename T>
	void Deque<T>::incrementEnd()
	{
		++endIndex;
		if (endIndex == BLOCKSIZE)
		{
			if (backBlock == mapSize - 1)
				resize(false);

			if (!map[backBlock + 1])
				map[backBlock + 1] = new T[BLOCKSIZE];

			++backBlock;
			endIndex = 0;
		}
	}

	// Decrements the pointer to the end of the deque by one
	template <typename T>
	void Deque<T>::decrementEnd()
	{
		if (endIndex == 0)
		{
			--backBlock;
			endIndex = BLOCKSIZE - 1;
		}
		else
			--endIndex;
	}
}
#endif