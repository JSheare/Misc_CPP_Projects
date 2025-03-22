#ifndef JADT_HASH_TABLE_HPP
#define JADT_HASH_TABLE_HPP

#include <cstddef>
#include <functional>
#include <stdexcept>

namespace JADT
{
	template <typename T, typename U>
	HashTable<T, U>::HashTable(std::size_t numBuckets) :
		numBuckets{ numBuckets }, buckets{ new BucketLink*[numBuckets] }
	{
		for (std::size_t i{ 0 }; i < numBuckets; ++i)
		{
			buckets[i] = nullptr;
		}
	}

	template <typename T, typename U>
	HashTable<T, U>::~HashTable()
	{
		clear();
		delete[] buckets;
	}

	// Returns true if the hash table is empty
	template <typename T, typename U>
	bool HashTable<T, U>::empty() const
	{
		return numValues == 0;
	}

	// Returns the size of the hash table
	template <typename T, typename U>
	std::size_t HashTable<T, U>::size() const
	{
		return numValues;
	}

	// Returns true if the hash table contains a key-value pair with the given key
	template <typename T, typename U>
	bool HashTable<T, U>::contains(const T& key)
	{
		std::size_t index{ getBucketIndex(key) };
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

	// Inserts the given key-value pair, or updates the value for the given key. Note that calling this method may invalidate iterators
	template <typename T, typename U>
	void HashTable<T, U>::insert(const T& key, const U& value)
	{
		getBucketLink(key)->value = value;
	}

	// Returns the value for the given key. Note that calling this method may invalidate iterators
	template <typename T, typename U>
	U& HashTable<T, U>::get(const T& key)
	{
		return getBucketLink(key)->value;
	}

	template <typename T, typename U>
	const U& HashTable<T, U>::get(const T& key) const
	{
		return getBucketLink(key)->value;
	}

	// Synonym of get()
	template <typename T, typename U>
	U& HashTable<T, U>::operator[](const T& key)
	{
		return get(key);
	}

	template <typename T, typename U>
	const U& HashTable<T, U>::operator[](const T& key) const
	{
		return get(key);
	}

	// Removes the key-value pair with the given key from the hash table (if it exists)
	template <typename T, typename U>
	void HashTable<T, U>::remove(const T& key)
	{
		std::size_t index{ getBucketIndex(key) };
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
					--numValues;
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
		numValues = 0;
	}

	// Returns an iterator to the beginning of the hash table. Iterators return references to keys
	template <typename T, typename U>
	HashTable<T, U>::HTIter HashTable<T, U>::begin()
	{
		for (std::size_t i{ 0 }; i < numBuckets; ++i)
		{
			if (buckets[i])
				return HTIter(buckets + i, buckets[i], numBuckets - i);
		}
		return end();
	}

	// Returns an iterator to one past the end of the hash table
	template <typename T, typename U>
	HashTable<T, U>::HTIter HashTable<T, U>::end()
	{
		return HTIter(buckets + numBuckets, nullptr, 0);
	}

	// Returns a constant iterator to the beginning of the hash table. Constant iterators return constant references to keys
	template <typename T, typename U>
	HashTable<T, U>::ConstHTIter HashTable<T, U>::cbegin()
	{
		for (std::size_t i{ 0 }; i < numBuckets; ++i)
		{
			if (buckets[i])
				return ConstHTIter(buckets + i, buckets[i], numBuckets - i);
		}
		return cend();
	}

	// Returns a constant iterator to one past the end of the hash table
	template <typename T, typename U>
	HashTable<T, U>::ConstHTIter HashTable<T, U>::cend()
	{
		return ConstHTIter(buckets + numBuckets, nullptr, 0);
	}

	// Hashes the given key and returns the corresponding bucket index
	template <typename T, typename U>
	std::size_t HashTable<T, U>::getBucketIndex(const T& key) const
	{
		std::size_t hash{ hasher(key) };
		return hash % numBuckets;
	}

	// Returns the link with the given key. Creates a new link if no link with the given key exists
	template <typename T, typename U>
	HashTable<T, U>::BucketLink* HashTable<T, U>::getBucketLink(const T& key)
	{
		// Rehashing if we've exceeded the maximum load factor
		if (static_cast<double>(numValues) / static_cast<double>(numBuckets) >= 1.0)
			rehash();

		std::size_t index{ getBucketIndex(key) };
		if (buckets[index])
		{
			BucketLink* prev{ nullptr };
			BucketLink* curr{ buckets[index] };
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
			curr->key = key;
			prev->next = curr;
			++numValues;
			return curr;
		}
		// Adding a new pair to an empty bucket
		BucketLink* link{ new BucketLink() };
		link->key = key;
		buckets[index] = link;
		++numValues;
		return link;
	}

	// Returns the link with the given key. Throws std::invalid_argument if no link with the specified key exists
	template <typename T, typename U>
	HashTable<T, U>::BucketLink* HashTable<T, U>::getBucketLink(const T& key) const
	{
		std::size_t index{ getBucketIndex(key) };
		if (buckets[index])
		{
			BucketLink* prev{ nullptr };
			BucketLink* curr{ buckets[index] };
			while (curr)
			{
				if (curr->key == key)
					return curr;

				prev = curr;
				curr = curr->next;
			}
		}
		throw std::invalid_argument("Not a valid key");
	}

	// Rehashes the hash table to be 2x its current size
	template <typename T, typename U>
	void HashTable<T, U>::rehash()
	{
		std::size_t oldNumBuckets{ numBuckets };
		BucketLink** oldBuckets{ buckets };
		numBuckets *= 2;
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
					std::size_t index{ getBucketIndex(curr->key) };
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

	// Bucket link implementation

	template <typename T, typename U>
	HashTable<T, U>::BucketLink::BucketLink()
	{}

	// Hash table iterator implementation

	template <typename T, typename U>
	HashTable<T, U>::HTIter::HTIter(HashTable<T, U>::BucketLink** bucketHead, HashTable<T, U>::BucketLink* currentLink, std::size_t bucketsLeft) :
		bucketHead{bucketHead}, currentLink{currentLink}, bucketsLeft{bucketsLeft}
	{}

	template <typename T, typename U>
	T& HashTable<T, U>::HTIter::operator*()
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

	// Constant hash table iterator implementation

	template <typename T, typename U>
	HashTable<T, U>::ConstHTIter::ConstHTIter(HashTable<T, U>::BucketLink** bucketHead, HashTable<T, U>::BucketLink* currentLink, std::size_t bucketsLeft) :
		HTIter(bucketHead, currentLink, bucketsLeft)
	{}

	template <typename T, typename U>
	const T& HashTable<T, U>::ConstHTIter::operator*() const
	{
		return HTIter::currentLink->key;
	}
}
#endif