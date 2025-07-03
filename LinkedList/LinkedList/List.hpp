#ifndef JML_LIST_HPP
#define JML_LIST_HPP

#include <initializer_list>
#include <iostream>
#include <stdexcept>

namespace JML
{
	template <typename T>
	List<T>::List()
	{}

	// Initializer list constructor
	template <typename T>
	List<T>::List(std::initializer_list<T> list)
	{
		for (auto& item : list)
		{
			pushBack(item);
		}
	}

	// Copy constructor
	template <typename T>
	List<T>::List(const List<T>& list)
	{
		length = list.length;
		ListLink* curr{ list.head };
		ListLink* copyPrev{ nullptr };
		ListLink* copyCurr{ new ListLink(curr->data) };
		head = copyCurr;
		curr = curr->next;
		copyPrev = copyCurr;
		while (curr)
		{
			copyCurr = new ListLink(curr->data);
			copyCurr->prev = copyPrev;
			copyPrev->next = copyCurr;
			curr = curr->next;
			copyPrev = copyCurr;
		}
		tail = copyPrev;
	}

	// Move constructor
	template <typename T>
	List<T>::List(List<T>&& list) noexcept : 
		length{ list.length }, 
		head{ list.head }, 
		tail{ list.tail }
	{
		list.length = 0;
		list.head = nullptr;
		list.tail = nullptr;
	}

	template <typename T>
	List<T>::~List()
	{
		clear();
	}

	// Copy assignment
	template <typename T>
	List<T>& List<T>::operator=(const List<T>& list)
	{
		if (&list == this)
			return *this;

		clear();

		length = list.length;
		ListLink* curr{ list.head };
		ListLink* copyPrev{ nullptr };
		ListLink* copyCurr{ new ListLink(curr->data) };
		head = copyCurr;
		curr = curr->next;
		copyPrev = copyCurr;
		while (curr)
		{
			copyCurr = new ListLink(curr->data);
			copyCurr->prev = copyPrev;
			copyPrev->next = copyCurr;
			curr = curr->next;
			copyPrev = copyCurr;
		}
		tail = copyPrev;
		return *this;
	}

	// Move assignment
	template <typename T>
	List<T>& List<T>::operator=(List<T>&& list) noexcept
	{
		clear();

		length = list.length;
		head = list.head;
		tail = list.tail;

		list.length = 0;
		list.head = nullptr;
		list.tail = nullptr;

		return *this;
	}

	template <typename T>
	T& List<T>::operator[](std::size_t index)
	{
		return get(index);
	}

	template <typename T>
	const T& List<T>::operator[](std::size_t index) const
	{
		return get(index);
	}

	template <typename T1>
	bool operator==(const List<T1>& list1, const List<T1>& list2)
	{
		if (list1.length == list2.length)
		{
			typename List<T1>::ListLink* oneCurr{ list1.head };
			typename List<T1>::ListLink* twoCurr{ list2.head };
			while (oneCurr)
			{
				if (oneCurr->data != twoCurr->data)
					return false;

				oneCurr = oneCurr->next;
				twoCurr = twoCurr->next;
			}
			return true;
		}
		return false;
	}

	template <typename T1>
	bool operator!=(const List<T1>& list1, const List<T1>& list2)
	{
		return !(operator==(list1, list2));
	}

	template <typename T1> 
	std::ostream& operator<<(std::ostream& out, const List<T1>& list)
	{
		out << '[';
		typename List<T1>::ListLink* currentLink{ list.head };
		std::size_t index{ 0 };
		while (currentLink)
		{
			out << currentLink->data;
			if (index != (list.length - 1))
			{
				out << ", ";
			}
			++index;
			currentLink = currentLink->next;
		}
		out << ']';
		return out;
	}

	// Pushes the item to the front of the list. Supports perfect forwarding
	template <typename T>
	template <typename U> void List<T>::pushFront(U&& item)
	{
		if (length == 0)
			head = tail = new ListLink(static_cast<U&&>(item));
		else
		{
			ListLink* newLink{ new ListLink(static_cast<U&&>(item)) };
			newLink->next = head;
			head->prev = newLink;
			head = newLink;
		}
		++length;
	}

	// Pushes the item to the back of the list. Supports perfect forwarding
	template <typename T>
	template <typename U> void List<T>::pushBack(U&& item)
	{
		if (length == 0)
			head = tail = new ListLink(static_cast<U&&>(item));
		else
		{
			ListLink* newLink{ new ListLink(static_cast<U&&>(item)) };
			newLink->prev = tail;
			tail->next = newLink;
			tail = newLink;
		}
		++length;
	}

	// Inserts the item at the specified index. Supports perfect forwarding.
	// Appends to the end of the list if index is greater than or equal to the current length of the list
	template <typename T>
	template <typename U> void List<T>::insert(U&& item, std::size_t index)
	{
		if (length == 0)
			head = tail = new ListLink(static_cast<U&&>(item));
		else if (index >= length)
		{
			ListLink* newLink{ new ListLink(static_cast<U&&>(item)) };
			newLink->prev = tail;
			tail->next = newLink;
			tail = newLink;
		}
		// Inserting the item at the given index and moving everything else forward 
		else
		{
			ListLink* newLink{ new ListLink(static_cast<U&&>(item)) };
			ListLink* oldLink{ getLink(index) };
			oldLink->prev->next = newLink;
			newLink->prev = oldLink->prev;
			newLink->next = oldLink;
			oldLink->prev = newLink;
		}
		++length;
	}

	// Returns the item at the front of the list
	template <typename T>
	T& List<T>::front()
	{
		return head->data;
	}

	template <typename T>
	const T& List<T>::front() const
	{
		return head->data;
	}

	// Returns the item at the back of the list
	template <typename T>
	T& List<T>::back()
	{
		return tail->data;
	}

	template <typename T>
	const T& List<T>::back() const
	{
		return tail->data;
	}

	// Returns the item at the specified index
	template <typename T>
	T& List<T>::get(std::size_t index)
	{
		return getLink(index)->data;
	}

	template <typename T>
	const T& List<T>::get(std::size_t index) const
	{
		return getLink(index)->data;
	}

	// Pops the item at the front of the list
	template <typename T>
	void List<T>::popFront()
	{
		if (length > 0)
		{
			if (length == 1)
			{
				delete head;
				head = tail = nullptr;
			}
			else
			{
				ListLink* newHead = head->next;
				newHead->prev = nullptr;
				delete head;
				head = newHead;
			}
			--length;
		}
	}

	// Pops the item at the back of the list
	template <typename T>
	void List<T>::popBack()
	{
		if (length > 0)
		{
			if (length == 1)
			{
				delete tail;
				tail = head = nullptr;
			}
			else
			{
				ListLink* newTail = tail->prev;
				newTail->next = nullptr;
				delete tail;
				tail = newTail;
			}
			--length;
		}
	}

	// Pops the item at the specified index
	template <typename T>
	void List<T>::remove(std::size_t index)
	{
		if (length > 0)
		{
			if (length == 1)
			{
				delete head;
				head = tail = nullptr;
			}
			else
			{
				ListLink* link{ getLink(index) };
				if (link->prev)
					link->prev->next = link->next;

				if (link->next)
					link->next->prev = link->prev;

				delete link;
			}
			--length;
		}
	}

	// Returns true if the list is empty and false otherwise
	template <typename T>
	bool List<T>::empty() const
	{
		return length == 0;
	}

	// Returns the length of the list
	template <typename T>
	std::size_t List<T>::size() const 
	{ 
		return length; 
	}

	// Clears the list, deallocating all the memory that it holds
	template <typename T>
	void List<T>::clear()
	{
		if (length > 1)
		{
			ListLink* prevLink{ nullptr };
			ListLink* currentLink{ head };
			while (currentLink)
			{
				delete prevLink;
				prevLink = currentLink;
				currentLink = currentLink->next;
			}
			delete tail;
		}
		else
			delete head;

		head = nullptr;
		tail = nullptr;
		length = 0;
	}

	// Finds the requested item in the list (if it exists) and returns its index.
	template <typename T>
	std::size_t List<T>::index(const T& item) const
	{
		ListLink* currentLink{ head };
		std::size_t index{ 0 };
		while (currentLink)
		{
			if (currentLink->data == item)
				return index;

			++index;
			currentLink = currentLink->next;
		}
		throw std::invalid_argument("Item is not in the list");
	}

	// Returns true if the list contains the requested item, otherwise false
	template <typename T>
	bool List<T>::contains(const T& item) const
	{
		ListLink* currentLink{ head };
		while (currentLink)
		{
			if (currentLink->data == item)
				return true;
			
			currentLink = currentLink->next;
		}
		return false;
	}

	// Returns a slice of the list containing only entries from start to one before stop
	template <typename T>
	List<T> List<T>::slice(std::size_t start, std::size_t stop) const
	{
		if (start >= length)
			throw std::out_of_range("Start index out of range");

		if (stop > length)
			throw std::out_of_range("End index out of range");

		List<T> slice;
		ListLink* currentLink{ getLink(start) };
		std::size_t index{ start };
		while (index < stop)
		{
			slice.pushBack(currentLink->data);
			++index;
			currentLink = currentLink->next;
		}
		return slice;
	}

	// Sorts the list with a merge sort algorithm. Ascending sort by default.
	template <typename T>
	void List<T>::sort()  // overload for default
	{
		sort(ascendingComparison);
	}

	// Sorts the list with a merge sort algorithm
	template <typename T>  // Custom comparison function
	template <typename U> void List<T>::sort(bool(*comparisonFcn)(U, U))
	{
		mergeSort(head, length, comparisonFcn);
	}

	// Returns an iterator to the beginning of the list
	template <typename T>
	List<T>::Iterator List<T>::begin()
	{
		return Iterator{head};
	}

	// Returns an iterator to one past the end of the list
	template <typename T>
	List<T>::Iterator List<T>::end()
	{
		Iterator temp{ tail };
		++temp;
		return temp;
	}

	// Returns a constant iterator to the beginning of the list
	template <typename T>
	List<T>::ConstIterator List<T>::begin() const
	{
		return ConstIterator{head};
	}

	// Returns a constant iterator to one past the end of the list
	template <typename T>
	List<T>::ConstIterator List<T>::end() const
	{
		ConstIterator temp{ tail };
		++temp;
		return temp;
	}

	// Returns a pointer to the link corresponding to the requested index
	template <typename T>
	List<T>::ListLink* List<T>::getLink(std::size_t index) const
	{
		if (index >= length)
			throw std::out_of_range("Index out of range");

		// Starts at the head of the list if the index is before the halfway point
		if (index < (length / 2))
		{
			std::size_t currentIndex{ 0 };
			ListLink* currentLink{ head };
			while (currentIndex < index)
			{
				currentLink = currentLink->next;
				++currentIndex;
			}
			return currentLink;
		}
		// Starts at the tail of the list otherwise
		else
		{
			std::size_t currentIndex{ length - 1 };
			ListLink* currentLink{ tail };
			while (currentIndex > index)
			{
				currentLink = currentLink->prev;
				--currentIndex;
			}
			return currentLink;
		}
	}

	// Merge sort algorithm implementation
	template <typename T>
	template <typename U> List<T>::ListLink* List<T>::mergeSort(List<T>::ListLink* subHead, std::size_t subLength, bool (*comparisonFcn)(U, U))
	{
		if (subLength > 1)
		{
			// Splitting the list in half
			std::size_t midpoint{ subLength / 2 };
			ListLink* right{ subHead };
			for (std::size_t i{ 0 }; i < midpoint; ++i)
			{
				right = right->next;
			}
			if (right->prev)
				right->prev->next = nullptr;

			right->prev = nullptr;

			// Recursive sorts on the two halves of the list
			ListLink* left{ mergeSort(subHead, midpoint, comparisonFcn) };
			right = mergeSort(right, subLength - midpoint, comparisonFcn);

			// Merging the two sorted halves
			ListLink dummy{};
			ListLink* curr{ &dummy };
			while (left && right)
			{
				if (comparisonFcn(left->data, right->data))
				{
					curr->next = left;
					left->prev = curr;
					left = left->next;
				}
				else
				{
					curr->next = right;
					right->prev = curr;
					right = right->next;
				}
				curr = curr->next;
			}
			while (left)
			{
				curr->next = left;
				left->prev = curr;
				left = left->next;
				curr = curr->next;
			}
			while (right)
			{
				curr->next = right;
				right->prev = curr;
				right = right->next;
				curr = curr->next;
			}
			// Saving the final sorted list
			if (subLength == length)
			{
				head = dummy.next;
				tail = curr;
			}
			return dummy.next;
		}
		return subHead;
	}

	// Default comparison function for sort(). Results in an ascending order sort
	template <typename T>
	bool List<T>::ascendingComparison(T& x, T& y)
	{
		return x <= y;
	}

	// List link implementation

	template <typename T>
	List<T>::ListLink::ListLink(const T& data) :
		data{ data }
	{}

	template <typename T>
	List<T>::ListLink::ListLink(T&& data) :
		data{ data }
	{}

	// List bidirectional iterator implementation

	template <typename T>
	List<T>::Iterator::Iterator(List<T>::ListLink* nodePtr) :
		nodePtr{nodePtr}
	{}

	template <typename T>
	T& List<T>::Iterator::operator*()
	{
		return nodePtr->data;
	}

	template <typename T>
	void List<T>::Iterator::operator++()
	{
		if (nodePtr)
		{
			if (nodePtr->next)
				nodePtr = nodePtr->next;
			else
				++nodePtr;
		}
	}

	template <typename T>
	void List<T>::Iterator::operator++(int)
	{
		operator++();
	}

	template <typename T>
	void List<T>::Iterator::operator--()
	{
		if (nodePtr)
		{
			if (nodePtr->next)
				nodePtr = nodePtr->next;
			else
				--nodePtr;
		}
	}

	template <typename T>
	void List<T>::Iterator::operator--(int)
	{
		operator--();
	}

	template <typename T>
	bool List<T>::Iterator::operator==(const Iterator& iterator) const
	{
		return nodePtr == iterator.nodePtr;
	}

	template <typename T>
	bool List<T>::Iterator::operator!=(const Iterator& iterator) const
	{
		return !operator==(iterator);
	}

	// Constant list iterator implementation

	template <typename T>
	List<T>::ConstIterator::ConstIterator(List<T>::ListLink* nodePtr) :
		Iterator::nodePtr{nodePtr}
	{}

	template <typename T>
	const T& List<T>::ConstIterator::operator*()
	{
		return Iterator::nodePtr->data;
	}
}
#endif