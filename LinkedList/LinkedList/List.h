#ifndef JADT_LIST_H
#define JADT_LIST_H

#include <initializer_list>
#include <iostream>
#include <stdexcept>

namespace JADT
{
	template <typename T>
	class List
	{
	private:
		class ListLink;
		class ListIter;
		class ConstListIter;

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
		ListIter begin();
		ListIter end();
		ConstListIter cbegin() const;
		ConstListIter cend() const;

	private:
		int m_length{ 0 };
		ListLink* m_head{ nullptr };
		ListLink* m_tail{ nullptr };

		void deallocate();
		ListLink* getLink(int index) const;
		template <typename U> static List<T>& mergeSort(List<T>& list, bool (*comparisonFcn)(U, U));
		static bool ascendingComparison(T& x, T& y);

		class ListLink
		{
		public:
			T m_data{};
			ListLink* m_next{ nullptr };
			ListLink* m_prev{ nullptr };

			ListLink();
			ListLink(T& data);
		};

		class ListIter
		{
		public:
			ListIter(ListLink* nodePtr);
			T& operator*();
			void operator++();
			bool operator!=(const ListIter& iterator) const;
		protected:
			ListLink* m_nodePtr;
		};

		class ConstListIter : public ListIter
		{
			ConstListIter(ListLink* nodePtr);
			const T& operator*();
		};
	};
}
#include "List.hpp"
#endif