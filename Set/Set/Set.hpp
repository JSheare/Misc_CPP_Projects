#ifndef JML_SET_HPP
#define JML_SET_HPP

#include <cstddef>
#include <stdexcept>

namespace JML
{
	template <typename T>
	Set<T>::Set(std::size_t reserveCount, float maxLoad) :
		maxLoad{ maxLoad }
	{
		float bucketsReq{ static_cast<float>(reserveCount) / maxLoad };
		std::size_t bucketsReqI{ static_cast<std::size_t>(bucketsReq) };
		if (bucketsReq == static_cast<float>(bucketsReqI))
			numBuckets = bucketsReqI;
		else
			numBuckets = bucketsReqI + 1;

		buckets = new BucketLink*[numBuckets];
		for (std::size_t i{ 0 }; i < numBuckets; ++i)
		{
			buckets[i] = nullptr;
		}
	}

	// Copy constructor
	template <typename T>
	Set<T>::Set(const Set<T>& set) :
		buckets{ new BucketLink*[set.numBuckets] }, numBuckets{ set.numBuckets }, numElements{ set.numElements }, maxLoad{ set.maxLoad }
	{
		for (std::size_t i{ 0 }; i < numBuckets; ++i)
		{
			if (set.buckets[i])
			{
				if (set.buckets[i]->next)
				{
					BucketLink* curr{ set.buckets[i] };
					BucketLink* copyCurr{ new BucketLink() };
					buckets[i] = copyCurr;
					copyCurr->key = curr->key;
					curr = curr->next;
					while (curr)
					{
						copyCurr->next = new BucketLink();
						copyCurr = copyCurr->next;
						copyCurr->key = curr->key;
						curr = curr->next;
					}
				}
				else
				{
					buckets[i] = new BucketLink();
					buckets[i]->key = set.buckets[i]->key;
					buckets[i]->value = set.buckets[i]->value;
				}
			}
		}
	}

	// Move constructor
	template <typename T>
	Set<T>::Set(Set<T>&& set) noexcept :
		buckets{ set.buckets }, numBuckets{ set.numBuckets }, numElements{ set.numElements }, maxLoad{ set.maxLoad }
	{
		// Allocating exactly one bucket in the old set so that it's still valid after the move
		set.numBuckets = 1;
		set.buckets = new BucketLink*[1];
		set.buckets[0] = nullptr;
		set.numElements = 0;
	}

	template <typename T>
	Set<T>::~Set()
	{
		clear();
		delete[] buckets;
	}

	// Copy assignment
	template <typename T>
	Set<T>& Set<T>::operator=(const Set<T>& set)
	{
		if (&set == this)
			return *this;

		clear();
		delete[] buckets;

		numBuckets = set.numBuckets;
		buckets = new BucketLink*[set.numBuckets];
		numElements = set.numElements;
		maxLoad = set.maxLoad;
		for (std::size_t i{ 0 }; i < numBuckets; ++i)
		{
			if (set.buckets[i])
			{
				if (set.buckets[i]->next)
				{
					BucketLink* curr{ set.buckets[i] };
					BucketLink* copyCurr{ new BucketLink() };
					buckets[i] = copyCurr;
					copyCurr->key = curr->key;
					curr = curr->next;
					while (curr)
					{
						copyCurr->next = new BucketLink();
						copyCurr = copyCurr->next;
						copyCurr->key = curr->key;
						curr = curr->next;
					}
				}
				else
				{
					buckets[i] = new BucketLink();
					buckets[i]->key = set.buckets[i]->key;
				}
			}
		}
		return *this;
	}

	// Move assignment
	template <typename T>
	Set<T>& Set<T>::operator=(Set<T>&& set) noexcept
	{
		clear();
		delete[] buckets;

		numBuckets = set.numBuckets;
		buckets = set.buckets;
		numElements = set.numElements;
		maxLoad = set.maxLoad;

		// Allocating exactly one bucket in the old set so that it's still valid after the move
		set.numBuckets = 1;
		set.buckets = new BucketLink*[1];
		set.buckets[0] = nullptr;
		set.numElements = 0;

		return *this;
	}

	template <typename T1>
	bool operator==(const Set<T1>& set1, const Set<T1>& set2)
	{
		if (set1.numElements == set2.numElements)
		{
			for (const T1& key : set1)
			{
				if (!set2.contains(key))
					return false;

			}
			return true;
		}
		return false;
	}

	template <typename T1>
	bool operator!=(const Set<T1>& table1, const Set<T1>& table2)
	{
		return !operator==(table1, table2);
	}

	// Returns true if the set is empty
	template <typename T>
	bool Set<T>::empty() const
	{
		return numElements == 0;
	}

	// Returns the size of the set
	template <typename T>
	std::size_t Set<T>::size() const
	{
		return numElements;
	}

	// Returns true if the set contains the given element
	template <typename T>
	bool Set<T>::contains(const T& element) const
	{
		std::size_t index{ bucket(element) };
		if (buckets[index])
		{
			BucketLink* curr{ buckets[index] };
			while (curr)
			{
				if (curr->element == element)
					return true;

				curr = curr->next;
			}
		}
		return false;
	}

	// Inserts the given element. Note that calling this method may invalidate iterators. Supports perfect forwarding
	template <typename T>
	template <typename V> void Set<T>::insert(V&& element)
	{
		addLink(static_cast<V&&>(element));
	}

	// Removes the element from the set (if it exists)
	template <typename T>
	void Set<T>::remove(const T& element)
	{
		std::size_t index{ bucket(element) };
		if (buckets[index])
		{
			BucketLink* prev{ nullptr };
			BucketLink* curr{ buckets[index] };
			while (curr)
			{
				if (curr->element == element)
				{
					if (prev)
						prev->next = curr->next;

					// Removing the head of the bucket
					else
						buckets[index] = curr->next;

					delete curr;
					--numElements;
					return;
				}
				prev = curr;
				curr = curr->next;
			}
		}
	}

	// Clears all elements from the set
	template <typename T>
	void Set<T>::clear()
	{
		for (std::size_t i{ 0 }; i < numBuckets; ++i)
		{
			if (buckets[i])
			{
				BucketLink* prev{ nullptr };
				BucketLink* curr{ buckets[i] };
				while (curr)
				{
					prev = curr;
					curr = curr->next;
					delete prev;
				}
				buckets[i] = nullptr;
			}
		}
		numElements = 0;
	}

	// Returns the current number of buckets in the set
	template <typename T>
	std::size_t Set<T>::bucketCount() const
	{
		return numBuckets;
	}

	// Hashes the given key and returns the corresponding bucket index
	template <typename T>
	std::size_t Set<T>::bucket(const T& key) const
	{
		std::size_t hash{ hasher(key) };
		return hash % numBuckets;
	}

	// Returns the number of elements in the bucket with the given index
	template <typename T>
	std::size_t Set<T>::bucketSize(std::size_t bucketIndex) const
	{
		if (bucketIndex < numBuckets && buckets[bucketIndex])
		{
			std::size_t count{ 0 };
			BucketLink* curr{ buckets[bucketIndex] };
			while (curr)
			{
				++count;
				curr = curr->next;
			}
			return count;
		}
		return 0;
	}

	// Returns the current load factor (the number of elements divided by the number of buckets)
	template <typename T>
	float Set<T>::loadFactor() const
	{
		return static_cast<float>(numElements) / static_cast<float>(numBuckets);
	}

	// Returns the current maximum load factor
	template <typename T>
	float Set<T>::maxLoadFactor() const
	{
		return maxLoad;
	}

	// Sets the current maximum load factor
	template <typename T>
	void Set<T>::maxLoadFactor(float newMax)
	{
		maxLoad = newMax;
	}

	// Reserves the number of buckets needed to store at least count elements (without exceeding the maximum load factor) and rehashes
	template<typename T>
	void Set<T>::reserve(std::size_t count)
	{
		// Rehashing to the ceiling of this division operation
		float bucketsReq{ static_cast<float>(count) / maxLoad };
		std::size_t bucketsReqI{ static_cast<std::size_t>(bucketsReq) };
		if (bucketsReq == static_cast<float>(bucketsReqI))
			rehash(bucketsReqI);
		else
			rehash(bucketsReqI + 1);
	}

	// Rehashes the table so that it's under the maximum load factor and has at least count buckets
	template <typename T>
	void Set<T>::rehash(std::size_t count)
	{
		std::size_t oldNumBuckets{ numBuckets };
		BucketLink** oldBuckets{ buckets };
		while (loadFactor() > maxLoadFactor() || numBuckets < count)
		{
			numBuckets *= 2;
		}
		if (numBuckets > oldNumBuckets)
		{
			buckets = new BucketLink*[numBuckets];
			for (std::size_t i{ 0 }; i < numBuckets; ++i)
			{
				buckets[i] = nullptr;
			}
			for (std::size_t i{ 0 }; i < oldNumBuckets; ++i)
			{
				if (oldBuckets[i])
				{
					BucketLink* curr{ oldBuckets[i] };
					BucketLink* next{ nullptr };
					while (curr)
					{
						next = curr->next;
						curr->next = nullptr;
						std::size_t index{ bucket(curr->element) };
						if (!buckets[index])
							buckets[index] = curr;
						else
						{
							BucketLink* curr1{ buckets[index] };
							while (curr1->next)
							{
								curr1 = curr1->next;
							}
							curr1->next = curr;
						}
						curr = next;
					}
				}
			}
			delete[] oldBuckets;
		}
	}

	// Returns an iterator to the beginning of the set
	template <typename T>
	Set<T>::Iterator Set<T>::begin() const
	{
		for (std::size_t i{ 0 }; i < numBuckets; ++i)
		{
			if (buckets[i])
				return Iterator(buckets + i, buckets[i], numBuckets - i);
		}
		return end();
	}

	// Returns an iterator to one past the end of the set
	template <typename T>
	Set<T>::Iterator Set<T>::end() const
	{
		return Iterator(buckets + numBuckets, nullptr, 0);
	}

	// Creates a link with the given element, if it doesn't already exist. Supports perfect forwarding
	template <typename T>
	template <typename U> void Set<T>::addLink(U&& element)
	{
		std::size_t index{ bucket(element) };
		BucketLink* curr{ nullptr };
		if (buckets[index])
		{
			BucketLink* prev{ nullptr };
			curr = buckets[index];
			while (curr)
			{
				// Element is already in the set
				if (curr->element == element)
					return;

				prev = curr;
				curr = curr->next;
			}
			// Adding a new element to a non-empty bucket
			curr = new BucketLink();
			prev->next = curr;
		}
		else
		{
			// Adding a new element to an empty bucket
			curr = new BucketLink();
			buckets[index] = curr;
		}

		curr->element = static_cast<U&&>(element);
		++numElements;
		if (loadFactor() >= maxLoadFactor())
			rehash();

	}

	// Bucket link implementation

	template <typename T>
	Set<T>::BucketLink::BucketLink()
	{}

	// Set forward iterator implementation

	template <typename T>
	Set<T>::Iterator::Iterator(BucketLink** bucketHead, BucketLink* currentLink, std::size_t bucketsLeft) :
		bucketHead{ bucketHead }, currentLink{ currentLink }, bucketsLeft{ bucketsLeft }
	{}

	template <typename T>
	const T& Set<T>::Iterator::operator*()
	{
		return currentLink->element;
	}

	template <typename T>
	void Set<T>::Iterator::operator++()
	{
		if (currentLink)
			currentLink = currentLink->next;

		if (!currentLink && bucketsLeft)
		{
			while (bucketsLeft > 1)
			{
				++bucketHead;
				--bucketsLeft;
				if (*bucketHead)
				{
					currentLink = *bucketHead;
					return;
				}
			}
			// We've reached the end, so ensure that the iterator points to one past
			++bucketHead;
			--bucketsLeft;
		}
	}

	template <typename T>
	void Set<T>::Iterator::operator++(int)
	{
		operator++();
	}

	template <typename T>
	bool Set<T>::Iterator::operator==(const Iterator& iterator) const
	{
		return (bucketHead == iterator.bucketHead) && (currentLink == iterator.currentLink);
	}

	template <typename T>
	bool Set<T>::Iterator::operator!=(const Iterator& iterator) const
	{
		return !operator==(iterator);
	}
}
#endif