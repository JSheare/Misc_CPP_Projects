#ifndef JADT_HEAP_H
#define JADT_HEAP_H

namespace JADT
{
	template <typename T, typename U>
	class Heap
	{
	public:
		Heap(bool isMax = false, int capacity = 10);
		Heap(int capacity);
		~Heap();
		void minHeapify(int index);
		void maxHeapify(int index);
		void insert(T key, U value);
		U top();
		U extract();
		bool isEmpty();
		int size();

	private:
		T* keyArray{ nullptr };
		U* valueArray{ nullptr };
		int heapSize{ 0 };
		int capacity{};
		bool isMax{};

		int getParent(int index);
		int getLeft(int index);
		int getRight(int index);
		void swap(int index1, int index2);
	};
}
#include "Heap.hpp"
#endif