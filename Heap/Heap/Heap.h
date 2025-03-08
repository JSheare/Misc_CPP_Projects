#ifndef JADT_HEAP_H
#define JADT_HEAP_H

#include <cstddef>

namespace JADT
{
	template <typename T>
	bool heapGreater(const T& x, const T& y);

	template <typename T, bool (*comparator)(const T&, const T&) = heapGreater>
	class Heap
	{
	public:
		Heap(std::size_t capacity = 10);
		~Heap();
		std::size_t size() const;
		bool empty() const;
		void insert(const T& item);
		T& top();
		const T& top() const;
		void pop();

	private:
		std::size_t heapSize{ 0 };
		std::size_t capacity;
		T* items;

		std::size_t getParent(std::size_t index) const;
		std::size_t getLeft(std::size_t index) const;
		std::size_t getRight(std::size_t index) const;
		void heapifyUp(std::size_t index);
		void heapifyDown(std::size_t index);
		void swap(std::size_t index1, std::size_t index2);
		void resize();
	};
}
#include "Heap.hpp"
#endif