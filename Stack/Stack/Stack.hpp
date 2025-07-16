#ifndef JML_STACK_HPP
#define JML_STACK_HPP

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace JML
{
	template <typename T>
	Stack<T>::Stack() :
		mapSize{ 8 }, map{ new T*[mapSize] }, topBlock{ 0 }
	{
		for (std::size_t i{ 0 }; i < mapSize; ++i)
		{
			map[i] = nullptr;
		}
		map[topBlock] = new T[BLOCKSIZE];
	}

	template <typename T>
	Stack<T>::Stack(std::initializer_list<T> list) :
		Stack<T>::Stack()
	{
		for (auto& item : list)
		{
			push(item);
		}
	}

	// Copy constructor
	template <typename T>
	Stack<T>::Stack(const Stack<T>& stack) :
		mapSize{ stack.mapSize }, map{ new T*[stack.mapSize] }, topBlock{ stack.topBlock }, 
		numItems{ stack.numItems }, topIndex{stack.topIndex }
	{
		for (std::size_t i{ 0 }; i < mapSize; ++i)
		{
			if (stack.map[i])
			{
				map[i] = new T[BLOCKSIZE];
				for (std::size_t j{ 0 }; j < BLOCKSIZE; ++j)
				{
					map[i][j] = stack.map[i][j];
				}
			}
			else
				map[i] = nullptr;
		}
	}

	// Move constructor
	template <typename T>
	Stack<T>::Stack(Stack<T>&& stack) noexcept :
		mapSize{ stack.mapSize }, map{ stack.map }, topBlock{ stack.topBlock },
		numItems{ stack.numItems }, topIndex{ stack.topIndex }
	{
		// Allocating exactly one block so that the old stack is still valid after the move
		stack.map = new T*[1];
		stack.mapSize = 1;
		stack.topBlock = 0;
		stack.numItems = 0;
		stack.topIndex = 0;
		stack.map[0] = new T[BLOCKSIZE];
	}

	template <typename T>
	Stack<T>::~Stack()
	{
		for (std::size_t i{ 0 }; i < mapSize; ++i)
		{
			delete[] map[i];
		}
		delete[] map;
	}

	// Copy assignment
	template <typename T>
	Stack<T>& Stack<T>::operator=(const Stack<T>& stack)
	{
		if (&stack == this)
			return *this;

		for (std::size_t i{ 0 }; i < mapSize; ++i)
		{
			delete[] map[i];
		}
		delete[] map;

		map = new T*[stack.mapSize];
		mapSize = stack.mapSize;
		topBlock = stack.topBlock;
		numItems = stack.numItems;
		topIndex = stack.topIndex;
		for (std::size_t i{ 0 }; i < mapSize; ++i)
		{
			if (stack.map[i])
			{
				map[i] = new T[BLOCKSIZE];
				for (std::size_t j{ 0 }; j < BLOCKSIZE; ++j)
				{
					map[i][j] = stack.map[i][j];
				}
			}
			else
				map[i] = nullptr;
		}
		return *this;
	}

	// Move assignment
	template <typename T>
	Stack<T>& Stack<T>::operator=(Stack<T>&& stack) noexcept
	{
		for (std::size_t i{ 0 }; i < mapSize; ++i)
		{
			delete[] map[i];
		}
		delete[] map;

		map = stack.map;
		mapSize = stack.mapSize;
		topBlock = stack.topBlock;
		numItems = stack.numItems;
		topIndex = stack.topIndex;

		// Allocating exactly one block so that the old deque is still valid after the move
		stack.map = new T*[1];
		stack.mapSize = 1;
		stack.topBlock = 0;
		stack.numItems = 0;
		stack.topIndex = 0;
		stack.map[0] = new T[BLOCKSIZE];

		return *this;
	}

	template <typename T>
	bool operator==(const Stack<T>& stack1, const Stack<T>& stack2)
	{
		if (stack1.numItems == stack2.numItems)
		{
			std::size_t block{ 0 };
			std::size_t index{ 0 };
			for (std::size_t i{ 0 }; i < stack1.numItems; ++i)
			{
				if (stack1.map[block][index] != stack2.map[block][index])
					return false;

				++index;
				if (index == stack1.BLOCKSIZE)
				{
					++block;
					index = 0;
				}
			}
			return true;
		}
		return false;
	}

	template <typename T>
	bool operator!=(const Stack<T>& stack1, const Stack<T>& stack2)
	{
		return !operator==(stack1, stack2);
	}

	// Returns the number of items in the stack
	template <typename T>
	std::size_t Stack<T>::size() const
	{
		return numItems;
	}

	// Returns true if the stack has no items
	template <typename T>
	bool Stack<T>::empty() const
	{
		return numItems == 0;
	}

	// Clears all items from the stack
	template <typename T>
	void Stack<T>::clear()
	{
		while (topBlock > 0)
		{
			delete[] map[topBlock];
			map[topBlock--] = nullptr;
		}
		topIndex = numItems = 0;
	}

	// Pushes the given item to the top of the stack. Supports perfect forwarding
	template <typename T>
	template <typename U> void Stack<T>::push(U&& item)
	{
		map[topBlock][topIndex] = static_cast<U&&>(item);
		++numItems;
		++topIndex;
		if (topIndex == BLOCKSIZE)
		{
			if (topBlock == mapSize - 1)
				resize();

			if (!map[topBlock + 1])
				map[topBlock + 1] = new T[BLOCKSIZE];

			++topBlock;
			topIndex = 0;
		}
	}

	// Returns a reference to the item at the top of the stack. Throws std::out_of_range if the stack is empty
	template <typename T>
	T& Stack<T>::top()
	{
		if (numItems == 0)
			throw std::out_of_range("empty stack, no top item");

		if (topIndex == 0)
			return map[topBlock - 1][BLOCKSIZE - 1];
		else
			return map[topBlock][topIndex - 1];
	}

	template <typename T>
	const T& Stack<T>::top() const
	{
		if (numItems == 0)
			throw std::out_of_range("empty stack, no top item");

		if (topIndex == 0)
			return map[topBlock - 1][BLOCKSIZE - 1];
		else
			return map[topBlock][topIndex - 1];
	}

	// Removes the item at the top of the stack. Throws std::out_of_range if the deque is empty
	template <typename T>
	void Stack<T>::pop()
	{
		if (numItems == 0)
			throw std::out_of_range("no items to pop");

		--numItems;
		if (topIndex > 0)
			--topIndex;
		else
		{
			--topBlock;
			topIndex = BLOCKSIZE - 1;
			// Leaving a maximum of one spare block at the back
			if (topBlock + 2 < mapSize)
			{
				delete[] map[topBlock + 2];
				map[topBlock + 2] = nullptr;
			}
		}
	}

	// Shrinks the stack to the minimum map size necessary to hold all items
	template <typename T>
	void Stack<T>::shrink_to_fit()
	{
		std::size_t newMapSize{ topBlock + 1 };
		T** newMap{ new T*[newMapSize] };
		for (std::size_t i{ 0 }; i < newMapSize; ++i)
		{
			newMap[i] = map[i];
			map[i] = nullptr;
		}
		for (std::size_t i{ newMapSize }; i < mapSize; ++i)
		{
			delete[] map[i];
		}
		delete[] map;
		map = newMap;
		mapSize = newMapSize;
	}

	// Resizes the stack to 2x its current size
	template <typename T>
	void Stack<T>::resize()
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
}
#endif