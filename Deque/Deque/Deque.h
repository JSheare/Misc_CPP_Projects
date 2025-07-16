#ifndef JML_DEQUEUE_H
#define JML_DEQUEUE_H

#include <cstddef>
#include <initializer_list>

namespace JML
{
	template <typename T>
	class Deque
	{
	private:
		class Iterator;
		class ConstIterator;

	public:
		Deque();
		Deque(std::initializer_list<T> list);
		Deque(const Deque<T>& deque);  // Copy constructor
		Deque(Deque<T>&& deque) noexcept;  // Move constructor
		~Deque();
		Deque<T>& operator=(const Deque<T>& deque);  // Copy assignment
		Deque<T>& operator=(Deque<T>&& deque) noexcept;  // Move assignment
		T& operator[](std::size_t index);
		const T& operator[](std::size_t index) const;
		friend bool operator==(const Deque<T>& deque1, const Deque<T>& deque2);
		friend bool operator!=(const Deque<T>& deque1, const Deque<T>& deque2);
		std::size_t size() const;
		bool empty() const;
		void clear();
		template <typename U> void pushFront(U&& item);
		template <typename U> void pushBack(U&& item);
		template <typename U> void insert(std::size_t index, U&& item);
		T& peekFront();
		const T& peekFront() const;
		T& peekBack();
		const T& peekBack() const;
		T& peek(std::size_t index);
		const T& peek(std::size_t index) const;
		void popFront();
		void popBack();
		void remove(std::size_t index);
		void shrink_to_fit();
		Iterator begin();
		Iterator end();
		ConstIterator begin() const;
		ConstIterator end() const;

	private:
		static constexpr std::size_t BLOCKSIZE{512/sizeof(T) ? 512/sizeof(T) : 1};
		std::size_t mapSize{ 0 };
		T** map{ nullptr };
		std::size_t frontBlock{ 0 };
		std::size_t backBlock{ 0 };

		std::size_t numItems{ 0 };
		std::size_t startIndex{ 0 };  // Points to one slot before the first element
		std::size_t endIndex{ 0 };  // Points to one slot after the last element

		std::size_t getBlock(std::size_t offset);
		std::size_t getIndex(std::size_t offset);
		void resize();
		void incrementStart();
		void decrementStart();
		void incrementBack();
		void decrementBack();
		bool slide(bool left);

		class Iterator
		{
		public:
			Iterator(T** currentBlock, T* current);
			T& operator*();
			void operator++();
			void operator++(int);
			void operator--();
			void operator--(int);
			bool operator==(const Iterator& iterator) const;
			bool operator!=(const Iterator& iterator) const;

		protected:
			T** currentBlock{ nullptr };
			T* current{ nullptr };
		};

		class ConstIterator : Iterator
		{
		public:
			ConstIterator(T** currentBlock, T* current);
			const T& operator*();
		};
	};
}
#include "Deque.hpp"
#endif
