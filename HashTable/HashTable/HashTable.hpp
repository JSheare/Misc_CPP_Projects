#ifndef JML_HASH_TABLE_HPP
#define JML_HASH_TABLE_HPP

#include <cstddef>
#include <functional>
#include <stdexcept>

namespace JML
{
	template <typename T, typename U>
	HashTable<T, U>::HashTable(std::size_t numBuckets) :
		buckets{ new BucketLink*[numBuckets] }, numBuckets{ numBuckets }
	{
		for (std::size_t i{ 0 }; i < numBuckets; ++i)
		{
			buckets[i] = nullptr;
		}
	}

	// Copy constructor
	template <typename T, typename U>
	HashTable<T, U>::HashTable(const HashTable<T, U>& table) :
		buckets{ new BucketLink*[table.numBuckets] }, numBuckets{table.numBuckets}, numPairs{table.numPairs}, maxLoad{table.maxLoad}
	{
		for (std::size_t i{ 0 }; i < numBuckets; ++i)
		{
			if (table.buckets[i])
			{
				if (table.buckets[i]->next)
				{
					BucketLink* curr{ table.buckets[i] };
					BucketLink* copyCurr{ new BucketLink() };
					buckets[i] = copyCurr;
					copyCurr->key = curr->key;
					copyCurr->value = curr->value;
					curr = curr->next;
					while (curr)
					{
						copyCurr->next = new BucketLink();
						copyCurr = copyCurr->next;
						copyCurr->key = curr->key;
						copyCurr->value = curr->value;
						curr = curr->next;
					}
				}
				else
				{
					buckets[i] = new BucketLink();
					buckets[i]->key = table.buckets[i]->key;
					buckets[i]->value = table.buckets[i]->value;
				}
			}
		}
	}

	// Move constructor
	template <typename T, typename U>
	HashTable<T, U>::HashTable(HashTable<T, U>&& table) noexcept :
		buckets{ table.buckets }, numBuckets{table.numBuckets}, numPairs{table.numPairs}, maxLoad{table.maxLoad}
	{
		// Allocating exactly one bucket in the old table so that it's still valid after the move
		table.numBuckets = 1;
		table.buckets = new BucketLink*[1];
		table.buckets[0] = nullptr;
		table.numPairs = 0;
	}

	template <typename T, typename U>
	HashTable<T, U>::~HashTable()
	{
		clear();
		delete[] buckets;
	}

	// Copy assignment
	template <typename T, typename U>
	HashTable<T, U>& HashTable<T, U>::operator=(const HashTable<T, U>& table)
	{
		if (&table == this)
			return *this;

		clear();
		delete[] buckets;

		numBuckets = table.numBuckets;
		buckets = new BucketLink*[table.numBuckets];
		numPairs = table.numPairs;
		maxLoad = table.maxLoad;
		for (std::size_t i{ 0 }; i < numBuckets; ++i)
		{
			if (table.buckets[i])
			{
				if (table.buckets[i]->next)
				{
					BucketLink* curr{ table.buckets[i] };
					BucketLink* copyCurr{ new BucketLink() };
					buckets[i] = copyCurr;
					copyCurr->key = curr->key;
					copyCurr->value = curr->value;
					curr = curr->next;
					while (curr)
					{
						copyCurr->next = new BucketLink();
						copyCurr = copyCurr->next;
						copyCurr->key = curr->key;
						copyCurr->value = curr->value;
						curr = curr->next;
					}
				}
				else
				{
					buckets[i] = new BucketLink();
					buckets[i]->key = table.buckets[i]->key;
					buckets[i]->value = table.buckets[i]->value;
				}
			}
		}
		return *this;
	}

	// Move assignment
	template <typename T, typename U>
	HashTable<T, U>& HashTable<T, U>::operator=(HashTable<T, U>&& table) noexcept
	{
		clear();
		delete[] buckets;
		numBuckets = table.numBuckets;
		buckets = table.buckets;
		numPairs = table.numPairs;
		maxLoad = table.maxLoad;

		// Allocating exactly one bucket in the old table so that it's still valid after the move
		table.numBuckets = 1;
		table.buckets = new BucketLink*[1];
		table.buckets[0] = nullptr;
		table.numPairs = 0;

		return *this;
	}

	template <typename T, typename U>
	template <typename V> U& HashTable<T, U>::operator[](V&& key)
	{
		return find(static_cast<V&&>(key));
	}

	template <typename T, typename U>
	const U& HashTable<T, U>::operator[](const T& key) const
	{
		return find(key);
	}

	// Returns true if the hash table is empty
	template <typename T, typename U>
	bool HashTable<T, U>::empty() const
	{
		return numPairs == 0;
	}

	// Returns the size of the hash table
	template <typename T, typename U>
	std::size_t HashTable<T, U>::size() const
	{
		return numPairs;
	}

	// Returns true if the hash table contains a key-value pair with the given key
	template <typename T, typename U>
	bool HashTable<T, U>::contains(const T& key) const
	{
		std::size_t index{ bucket(key) };
		if (buckets[index])
		{
			BucketLink* curr{ buckets[index] };
			while (curr)
			{
				if (curr->key == key)
					return true;

				curr = curr->next;
			}
		}
		return false;
	}

	// Inserts the given key-value pair, or updates the value for the given key. Note that calling this method may invalidate iterators. Supports perfect forwarding
	template <typename T, typename U>
	template <typename V, typename W> void HashTable<T, U>::insert(V&& key, W&& value)
	{
		getBucketLink(static_cast<V&&>(key))->value = static_cast<W&&>(value);
	}

	// Returns the value for the given key. Note that calling this method may cause a rehash which would invalidate iterators. Supports perfect forwarding
	template <typename T, typename U>
	template <typename V> U& HashTable<T, U>::find(V&& key)
	{
		return getBucketLink(static_cast<V&&>(key))->value;
	}

	template <typename T, typename U>
	const U& HashTable<T, U>::find(const T& key) const
	{
		return getBucketLink(key)->value;
	}

	// Removes the key-value pair with the given key from the hash table (if it exists)
	template <typename T, typename U>
	void HashTable<T, U>::remove(const T& key)
	{
		std::size_t index{ bucket(key) };
		if (buckets[index])
		{
			BucketLink* prev{ nullptr };
			BucketLink* curr{ buckets[index] };
			while (curr)
			{
				if (curr->key == key)
				{
					if (prev)
						prev->next = curr->next;

					// Removing the head of the bucket
					else
						buckets[index] = curr->next;

					delete curr;
					--numPairs;
					return;
				}
				prev = curr;
				curr = curr->next;
			}
		}
	}

	// Clears all key-value pairs from the hash table
	template <typename T, typename U>
	void HashTable<T, U>::clear()
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
		numPairs = 0;
	}

	// Returns the current number of buckets in the table
	template <typename T, typename U>
	std::size_t HashTable<T, U>::bucketCount() const
	{
		return numBuckets;
	}

	// Hashes the given key and returns the corresponding bucket index
	template <typename T, typename U>
	std::size_t HashTable<T, U>::bucket(const T& key) const
	{
		std::size_t hash{ hasher(key) };
		return hash % numBuckets;
	}

	// Returns the number of key-value pairs in the bucket with the given index
	template <typename T, typename U>
	std::size_t HashTable<T, U>::bucketSize(std::size_t bucketIndex) const
	{
		if (bucketIndex < numBuckets && buckets[bucketIndex])
		{
			std::size_t numPairs{ 0 };
			BucketLink* curr{ buckets[bucketIndex] };
			while (curr)
			{
				++numPairs;
				curr = curr->next;
			}
			return numPairs;
		}
		return 0;
	}

	// Returns the current load factor (the number of key-value pairs divided by the number of buckets)
	template <typename T, typename U>
	float HashTable<T, U>::loadFactor() const
	{
		return static_cast<float>(numPairs) / static_cast<float>(numBuckets);
	}

	// Returns the current maximum load factor
	template <typename T, typename U>
	float HashTable<T, U>::maxLoadFactor() const
	{
		return maxLoad;
	}

	// Sets the current maximum load factor
	template <typename T, typename U>
	void HashTable<T, U>::maxLoadFactor(float max)
	{
		maxLoad = max;
	}

	// Reserves the number of buckets needed to store at least count key-value pairs (without exceeding the maximum load factor) and rehashes
	template<typename T, typename U>
	void HashTable<T, U>::reserve(std::size_t count)
	{
		// Rehashing to the ceiling of this division operation
		float bucketsReq{ static_cast<float>(count) / maxLoadFactor() };
		std::size_t bucketsReqI{ static_cast<std::size_t>(bucketsReq) };
		if (bucketsReq == static_cast<float>(bucketsReqI))
			rehash(bucketsReqI);
		else
			rehash(bucketsReqI + 1);
	}

	// Rehashes the table so that it's under the maximum load factor and has at least count buckets
	template <typename T, typename U>
	void HashTable<T, U>::rehash(std::size_t count)
	{
		std::size_t oldNumBuckets{ numBuckets };
		BucketLink** oldBuckets{ buckets };		
		while (loadFactor() > maxLoadFactor() || numBuckets < count)
		{
			numBuckets *= 2;
		}
		buckets = new BucketLink * [numBuckets];
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
					std::size_t index{ bucket(curr->key) };
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

	// Returns an iterator to the beginning of the hash table. Iterators return constant references to keys
	template <typename T, typename U>
	HashTable<T, U>::HTIter HashTable<T, U>::begin() const
	{
		for (std::size_t i{ 0 }; i < numBuckets; ++i)
		{
			if (buckets[i])
				return HTIter(buckets + i, buckets[i], numBuckets - i);
		}
		return end();
	}

	// Returns an iterator to one past the end of the hash table. Iterators return constant references to keys
	template <typename T, typename U>
	HashTable<T, U>::HTIter HashTable<T, U>::end() const
	{
		return HTIter(buckets + numBuckets, nullptr, 0);
	}

	// Returns the link with the given key. Creates a new link if no link with the given key exists.  Supports perfect forwarding
	template <typename T, typename U>
	template <typename V> HashTable<T, U>::BucketLink* HashTable<T, U>::getBucketLink(V&& key)
	{
		std::size_t index{ bucket(key) };
		BucketLink* curr{ nullptr };
		if (buckets[index])
		{
			BucketLink* prev{ nullptr };
			curr = buckets[index];
			while (curr)
			{
				// Getting an existing key value pair
				if (curr->key == key)
					return curr;

				prev = curr;
				curr = curr->next;
			}
			// Adding a new pair to a non-empty bucket
			curr = new BucketLink();
			prev->next = curr;
		}
		else
		{
			// Adding a new pair to an empty bucket
			curr = new BucketLink();
			buckets[index] = curr;
		}

		curr->key = static_cast<V&&>(key);
		++numPairs;
		if (loadFactor() >= maxLoadFactor())
			rehash();

		return curr;
	}

	// Returns the link with the given key. Throws std::invalid_argument if no link with the specified key exists
	template <typename T, typename U>
	HashTable<T, U>::BucketLink* HashTable<T, U>::getBucketLink(const T& key) const
	{
		std::size_t index{ bucket(key) };
		if (buckets[index])
		{
			BucketLink* curr{ buckets[index] };
			while (curr)
			{
				if (curr->key == key)
					return curr;

				curr = curr->next;
			}
		}
		throw std::invalid_argument("Not a valid key");
	}

	// Bucket link implementation

	template <typename T, typename U>
	HashTable<T, U>::BucketLink::BucketLink()
	{}

	// Hash table iterator implementation

	template <typename T, typename U>
	HashTable<T, U>::HTIter::HTIter(BucketLink** bucketHead, BucketLink* currentLink, std::size_t bucketsLeft) :
		bucketHead{bucketHead}, currentLink{currentLink}, bucketsLeft{bucketsLeft}
	{}

	template <typename T, typename U>
	const T& HashTable<T, U>::HTIter::operator*()
	{
		return currentLink->key;
	}

	template <typename T, typename U>
	void HashTable<T, U>::HTIter::operator++()
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

	template <typename T, typename U>
	bool HashTable<T, U>::HTIter::operator==(const HTIter& iterator) const
	{
		return (bucketHead == iterator.bucketHead) && (currentLink == iterator.currentLink);
	}

	template <typename T, typename U>
	bool HashTable<T, U>::HTIter::operator!=(const HTIter& iterator) const
	{
		return !operator==(iterator);
	}
}
#endif