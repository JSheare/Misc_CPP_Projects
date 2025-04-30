#ifndef JML_HEAP_H
#define JML_HEAP_H

#include <cstddef>

namespace JML
{
	template <typename T>
	bool heapGreater(const T& x, const T& y);

	template <typename T, bool (*comparator)(const T&, const T&) = heapGreater>
	class Heap
	{
	public:
		Heap(std::size_t capacity = 10);
		Heap(const Heap<T, comparator>& heap);  // Copy constructor
		Heap(Heap<T, comparator>&& heap) noexcept;  // Move constructor
		~Heap();
		Heap<T, comparator>& operator=(const Heap<T, comparator>& heap);  // Copy assignment
		Heap<T, comparator>& operator=(Heap<T, comparator>&& heap);  // Move assignment
		template <typename T1, bool (*comparator)(const T1&, const T1&)> friend bool operator==(const Heap<T1, comparator>& heap1, const Heap <T1, comparator>& heap2);
		template <typename T1, bool (*comparator)(const T1&, const T1&)> friend bool operator!=(const Heap<T1, comparator>& heap1, const Heap <T1, comparator>& heap2);
		std::size_t size() const;
		bool empty() const;
		template <typename U> void insert(U&& item);
		T& top();
		const T& top() const;
		void pop();

	private:
		std::size_t capacity{};
		std::size_t heapSize{ 0 };
		T* items{};

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