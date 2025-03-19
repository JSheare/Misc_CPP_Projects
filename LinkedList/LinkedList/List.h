#ifndef JADT_LIST_H
#define JADT_LIST_H

#include <cstddef>
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
		List(const List<T>& list);  // Copy constructor
		List(List<T>&& list) noexcept;  // Move constructor
		~List();
		List<T>& operator=(List<T>& list);  // Copy assignment
		List<T>& operator=(List<T>&& list);  // Move assignment
		T& operator[](std::size_t index);
		const T& operator[](std::size_t index) const;
		friend bool operator==(const List<T>& list1, const List<T>& list2);
		friend bool operator!=(const List<T>& list1, const List<T>& list2);
		template <typename T1> friend std::ostream& operator<<(std::ostream& out, const List<T1>& list);
		bool empty() const;
		std::size_t size() const;
		void clear();
		void pushFront(const T& item);
		void pushBack(const T& item);
		void insert(const T& item, std::size_t index);
		T& front(); 
		const T& front() const;
		T& back();
		const T& back() const;
		T& get(std::size_t index);
		const T& get(std::size_t index) const;
		void popFront();
		void popBack();
		void remove(std::size_t index);
		std::size_t index(const T& item) const;
		bool contains(const T& item) const;
		List<T> slice(std::size_t start, std::size_t end) const;
		void sort();
		template <typename U> void sort(bool(*comparisonFcn)(U, U)=ascendingComparison);  // Custom comparison function
		ListIter begin();
		ListIter end();
		ConstListIter cbegin() const;
		ConstListIter cend() const;

	private:
		std::size_t length{ 0 };
		ListLink* head{ nullptr };
		ListLink* tail{ nullptr };

		ListLink* getLink(std::size_t index) const;
		template <typename U> static List<T>& mergeSort(List<T>& list, bool (*comparisonFcn)(U, U));
		static bool ascendingComparison(T& x, T& y);

		class ListLink
		{
		public:
			T data;
			ListLink* next{ nullptr };
			ListLink* prev{ nullptr };

			ListLink(const T& data);
		};

		class ListIter
		{
		public:
			ListIter(ListLink* nodePtr);
			T& operator*();
			void operator++();
			bool operator!=(const ListIter& iterator) const;
		protected:
			ListLink* nodePtr;
		};

		class ConstListIter : ListIter
		{
		public:
			ConstListIter(ListLink* nodePtr);
			const T& operator*() const;
		};
	};
}
#include "List.hpp"
#endif