#ifndef JML_LIST_H
#define JML_LIST_H

#include <initializer_list>
#include <iostream>
#include <stdexcept>

namespace JML
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
		List<T>& operator=(const List<T>& list);  // Copy assignment
		List<T>& operator=(List<T>&& list) noexcept;  // Move assignment
		T& operator[](std::size_t index);
		const T& operator[](std::size_t index) const;
		template <typename T1> friend bool operator==(const List<T1>& list1, const List<T1>& list2);
		template <typename T1> friend bool operator!=(const List<T1>& list1, const List<T1>& list2);
		template <typename T1> friend std::ostream& operator<<(std::ostream& out, const List<T1>& list);
		bool empty() const;
		std::size_t size() const;
		void clear();
		template <typename U> void pushFront(U&& item);
		template <typename U> void pushBack(U&& item);
		template <typename U> void insert(U&& item, std::size_t index);
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
		template <typename U> void sort(bool(*comparisonFcn)(U, U));  // Custom comparison function
		ListIter begin();
		ListIter end();
		ConstListIter begin() const;
		ConstListIter end() const;

	private:
		std::size_t length{ 0 };
		ListLink* head{ nullptr };
		ListLink* tail{ nullptr };

		ListLink* getLink(std::size_t index) const;
		template <typename U> ListLink* mergeSort(ListLink* head, std::size_t length, bool (*comparisonFcn)(U, U));
		static bool ascendingComparison(T& x, T& y);

		class ListLink
		{
		public:
			T data{};
			ListLink* next{ nullptr };
			ListLink* prev{ nullptr };

			ListLink() = default;
			ListLink(const T& data);
			ListLink(T&& data);
		};

		class ListIter
		{
		public:
			ListIter(ListLink* nodePtr);
			T& operator*();
			void operator++();
			bool operator==(const ListIter& iterator) const;
			bool operator!=(const ListIter& iterator) const;
		protected:
			ListLink* nodePtr;
		};

		class ConstListIter : public ListIter
		{
		public:
			ConstListIter(ListLink* nodePtr);
			const T& operator*();
		};
	};
}
#include "List.hpp"
#endif