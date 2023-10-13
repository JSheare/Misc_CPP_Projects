#ifndef LIST_H
#define LIST_H

#include <initializer_list>
#include <iostream>

#include "Link.h"
#include "Iter.h"
#include "CIter.h"

namespace JADT
{
	template <typename T>
	class List
	{
	private:
		int m_length{ 0 };
		Link<T>* m_head{ nullptr };
		Link<T>* m_tail{ nullptr };

	public:
		List() = default;

		// Initializer list constructor
		List(std::initializer_list<T> list)
		{
			for (auto element : list)
			{
				append(element);
			}
		}

		// Copy constructor
		List(List<T>& list)
		{
			for (int index{ 0 }; index < list.m_length; ++index)
			{
				append(list.getLink(index)->m_data);
			}
		}

		// Move constructor
		List(List<T>&& list) noexcept
			: m_length{ list.m_length }
			, m_head{ list.m_head }
			, m_tail{ list.m_tail }
		{
			list.m_head = nullptr;
			list.m_tail = nullptr;
			list.m_length = 0;
		}

		~List()
		{
			deallocate();
		}

		// Copy assignment
		List<T>& operator=(List<T>& list)
		{
			if (&list == this)
				return *this;

			deallocate();

			for (int index{ 0 }; index < list.m_length; ++index)
			{
				append(list.getLink(index)->m_data);
			}

			return *this;
		}

		// Move assignment
		List<T>& operator=(List<T>&& list)
		{
			deallocate();

			m_length = list.m_length;
			m_head = list.m_head;
			m_tail = list.m_tail;

			list.m_head = nullptr;
			list.m_tail = nullptr;
			list.m_length = 0;
		}


		T& operator[](int index)
		{
			return getLink(index)->m_data;
		}

		const T& operator[](int index) const
		{
			return getLink(index)->m_data;
		}

		friend bool operator==(const List<T>& list1, const List<T>& list2)
		{
			return (list1.m_length == list2.m_length
				&& list1.m_head == list2.m_head
				&& list1.m_tail == list2.m_tail);
		}

		friend bool operator!=(const List<T>& list1, const List<T>& list2)
		{
			return !(operator==(list1, list2));
		}

		friend std::ostream& operator<<(std::ostream& out, List<T>& list)
		{
			out << '[';
			Link<T>* currentLink{ list.m_head };
			int index{ 0 };
			while (currentLink)
			{
				out << currentLink->m_data;
				if (index != (list.m_length - 1))
				{
					out << ", ";
				}
				++index;
				currentLink = currentLink->m_next;
			}
			out << ']';
			return out;
		}

		// Returns the length of the list as an integer
		int length() const { return m_length; }

		bool isEmpty() const { return m_length == 0; }

		// Appends a new entry to the end of the list
		void append(T data)
		{
			insert(m_length, data);
		}

		// Inserts a new entry at the specified index
		// (entry is inserted at the end if the index is larger than the current list length)
		void insert(int index, T data)
		{
			if (index < 0)
			{
				throw std::out_of_range("Index out of range");
			}
			else if (index == 0 && m_length == 0)  // Appending to an empty list
			{
				m_head = new Link<T>(data);
				m_tail = m_head;
			}
			else if (index >= m_length) // Appending to the end of the list
			{
				Link<T>* newLink{ new Link<T>(data) };
				newLink->m_prev = m_tail;
				m_tail->m_next = newLink;
				m_tail = newLink;
			}
			else  // Appending inside of the list
			{
				Link<T>* existingLink{ getLink(index) };
				Link<T>* previousLink{ existingLink->m_prev };
				Link<T>* newLink{ new Link<T>(data) };

				if (previousLink)  // In case the insertion point is the beginning of the list
					previousLink->m_next = newLink;
				else
					m_head = newLink;

				newLink->m_next = existingLink;
				existingLink->m_prev = newLink;
			}

			++m_length;
		}

		// Returns the value at the very end of the list and then removes the corresponding entry
		T pop() { return remove(m_length - 1); }

		// Returns the value at the requested index and removes the entry from the list
		T remove(int index)
		{
			if (index <= 0 && m_length == 0)
				throw std::out_of_range("Cannot pop an empty list");

			Link<T>* reqLink(getLink(index));
			T data = reqLink->m_data;

			Link<T>* previousLink{ reqLink->m_prev };
			Link<T>* nextLink{ reqLink->m_next };

			if (!previousLink && !nextLink)  // For popping from a list with only a single link
			{
				m_head = nullptr;
				m_tail = nullptr;
			}
			else
			{
				if (previousLink)
					previousLink->m_next = nextLink;
				else  // For popping from the beginning
				{
					nextLink->m_prev = nullptr;
					m_head = nextLink;
				}

				if (nextLink)
					nextLink->m_prev = previousLink;
				else  // For popping from the end
				{
					previousLink->m_next = nullptr;
					m_tail = previousLink;
				}
			}

			delete reqLink;
			--m_length;
			return data;
		}

		// Returns a new list containing only entries from start to one before stop
		List<T> slice(int start, int stop) const
		{
			List<T> slice{};
			Link<T>* currentLink{ getLink(start) };
			int index{ start };
			while (index < stop)
			{
				slice.append(currentLink->m_data);
				++index;
				currentLink = currentLink->m_next;
			}

			return slice;
		}

		// Sorts the list with a merge sort algorithm
		void sort()  // overload for default
		{
			mergeSort(*this, ascendingComparison);
		}

		template <typename U>  // Custom comparison function
		void sort(bool(*comparisonFcn)(U, U))
		{
			mergeSort(*this, comparisonFcn);
		}

		// Finds the requested entry (if it exists) in the list and returns its index. Otherwise returns -1
		int index(T data) const
		{
			Link<T>* currentLink{ this->m_head };
			int index{ 0 };
			while (currentLink)
			{
				if (currentLink->m_data == data)
					return index;

				++index;
				currentLink = currentLink->m_next;

			}

			return -1;
		}

		// Returns true if the list contains the requested data, otherwise false
		bool contains(T data) const
		{
			return index(data) != -1;
		}

		// Returns an iterator to the beginning of the list
		Iter<T> begin()
		{
			return Iter<T>{m_head};
		}

		// Returns an iterator to one past the end of the list
		Iter<T> end()
		{
			Iter<T> temp{ m_tail };
			++temp;
			return temp;
		}

		// Returns a constant iterator to the beginning of the list
		CIter<T> cbegin() const
		{
			return CIter<T>{m_head};
		}

		// Returns a constant iterator to one past the end of the list
		CIter<T> cend() const
		{
			CIter<T> temp{ m_tail };
			++temp;
			return temp;
		}

	private:
		// Deallocates any memory that the list is using (this effectively empties the list)
		void deallocate()
		{
			if (m_length == 1)
			{
				delete m_head;
			}
			else if (m_length > 1)
			{
				Link<T>* currentLink{ m_head->m_next };
				while (true)
				{
					delete currentLink->m_prev;
					if (currentLink == m_tail)
						break;
					currentLink = currentLink->m_next;
				}

				delete m_tail;
			}

			m_head = nullptr;
			m_tail = nullptr;
			m_length = 0;
		}

		// Returns a pointer to the link corresponding to the requested index
		Link<T>* getLink(int index) const
		{
			if (index >= m_length || index < 0)
			{
				throw std::out_of_range("Index out of range");
			}

			// Starts at the head of the list if the index is before the halfway point
			if (index < (m_length / 2))
			{
				int currentIndex{ 0 };
				Link<T>* currentLink{ m_head };
				while (currentIndex < index)
				{
					currentLink = currentLink->m_next;
					++currentIndex;
				}
				return currentLink;
			}

			// Starts at the tail of the list otherwise
			else
			{
				int currentIndex{ m_length - 1 };
				Link<T>* currentLink{ m_tail };
				while (currentIndex > index)
				{
					currentLink = currentLink->m_prev;
					--currentIndex;
				}
				return currentLink;
			}
		}

		// Merge sort algorithm implementation
		template <typename U>
		static List<T>& mergeSort(List<T>& list, bool (*comparisonFcn)(U, U))
		{
			if (list.m_length > 1)
			{
				// Recursive sorts on the two halves of the list
				int midpoint{ list.m_length / 2 };

				List<T> left{ list.slice(0, midpoint) };
				mergeSort(left, comparisonFcn);

				List<T> right{ list.slice(midpoint, list.m_length) };
				mergeSort(right, comparisonFcn);

				Link<T>* currentLeft{ left.m_head };
				Link<T>* currentRight{ right.m_head };
				Link<T>* currentLink{ list.m_head };

				while (currentLeft && currentRight)
				{
					if (comparisonFcn(currentLeft->m_data, currentRight->m_data))
					{
						currentLink->m_data = currentLeft->m_data;
						currentLeft = currentLeft->m_next;
					}
					else
					{
						currentLink->m_data = currentRight->m_data;
						currentRight = currentRight->m_next;
					}

					currentLink = currentLink->m_next;
				}

				while (currentLeft)
				{
					currentLink->m_data = currentLeft->m_data;
					currentLeft = currentLeft->m_next;
					currentLink = currentLink->m_next;
				}

				while (currentRight)
				{
					currentLink->m_data = currentRight->m_data;
					currentRight = currentRight->m_next;
					currentLink = currentLink->m_next;
				}

			}

			return list;

		}

		// Default comparison function for sort(). Results in an ascending order sort
		static bool ascendingComparison(T& x, T& y)
		{
			return x <= y;
		}
	};
}

#endif