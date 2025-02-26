#ifndef JADT_LIST_H
#define JADT_LIST_H

#include <initializer_list>
#include <iostream>
#include <stdexcept>

#include "Link.h"
#include "Iter.h"
#include "CIter.h"

namespace JADT
{
	template <typename T>
	class List
	{
	public:
		List();
		List(std::initializer_list<T> list);
		List(List<T>& list);  // Copy constructor
		List(List<T>&& list) noexcept;  // Move constructor
		~List();
		List<T>& operator=(List<T>& list);  // Copy assignment
		List<T>& operator=(List<T>&& list);  // Move assignment
		T& operator[](int index);
		const T& operator[](int index) const;
		friend bool operator==(const List<T>& list1, const List<T>& list2);
		friend bool operator!=(const List<T>& list1, const List<T>& list2);
		template <typename T1>
		friend std::ostream& operator<<(std::ostream& out, const List<T1>& list);
		int length() const;
		bool isEmpty() const;
		void append(T data);
		void insert(int index, T data);
		T pop();
		T remove(int index);
		List<T> slice(int start, int stop) const;
		void sort();
		template <typename U> void sort(bool(*comparisonFcn)(U, U));  // Custom comparison function
		int index(T data) const;
		bool contains(T data) const;
		Iter<T> begin();
		Iter<T> end();
		CIter<T> cbegin() const;
		CIter<T> cend() const;

	private:
		int m_length{ 0 };
		Link<T>* m_head{ nullptr };
		Link<T>* m_tail{ nullptr };

		void deallocate();
		Link<T>* getLink(int index) const;
		template <typename U> static List<T>& mergeSort(List<T>& list, bool (*comparisonFcn)(U, U));
		static bool ascendingComparison(T& x, T& y);
	};
}
#include "List.hpp"
#endif