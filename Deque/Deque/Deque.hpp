#ifndef JADT_DEQUEUE_HPP
#define JADT_DEQUEUE_HPP

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace JADT
{
	template <typename T>
	Deque<T>::Deque()
	{
		mapSize = 8;
		map = new T * [mapSize];
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

	template <typename T>
	Deque<T>::~Deque()
	{
		for (std::size_t i{ 0 }; i < mapSize; ++i)
		{
			delete[] map[i];
		}
		delete[] map;
	}

	template <typename T>
	std::size_t Deque<T>::size() const
	{
		return numItems;
	}

	template <typename T>
	bool Deque<T>::empty() const
	{
		return numItems == 0;
	}

	template <typename T>
	void Deque<T>::pushFront(const T& item)
	{
		map[frontBlock][startIndex] = item;
		++numItems;
		decrementStart();
		if (numItems == 1)
			incrementEnd();
	}

	template <typename T>
	void Deque<T>::pushBack(const T& item)
	{
		map[backBlock][endIndex] = item;
		++numItems;
		incrementEnd();
		if (numItems == 1)
			decrementStart();
	}

	template <typename T>
	T& Deque<T>::peekFront() const
	{
		if (numItems == 0)
			throw std::out_of_range("no items to peek");

		if (startIndex == BLOCKSIZE - 1)
			return map[frontBlock + 1][0];
		else
			return map[frontBlock][startIndex + 1];
	}

	template <typename T>
	T& Deque<T>::peekBack() const
	{
		if (numItems == 0)
			throw std::out_of_range("no items to peek");

		if (endIndex == 0)
			return map[backBlock - 1][BLOCKSIZE - 1];
		else
			return map[backBlock][endIndex - 1];
	}

	template <typename T>
	void Deque<T>::popFront()
	{
		if (numItems == 0)
			throw std::out_of_range("no items to pop");

		--numItems;
		incrementStart();
	}

	template <typename T>
	void Deque<T>::popBack()
	{
		if (numItems == 0)
			throw std::out_of_range("no items to pop");

		--numItems;
		decrementEnd();
	}

	template <typename T>
	void Deque<T>::resize(bool resizeFront)
	{
		std::size_t newMapSize{ mapSize * 2 };
		T** newMap{ new T* [newMapSize] };
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