#ifndef JML_HASH_TABLE_H
#define JML_HASH_TABLE_H

#include <cstddef>
#include <functional>

namespace JML
{
	template <typename T, typename U = T>
	class HashTable
	{
	private:
		class BucketLink;
		class HTIter;

	public:
		HashTable(std::size_t numBuckets = 10);
		HashTable(const HashTable<T, U>& table);  // Copy constructor
		HashTable(HashTable<T, U>&& table) noexcept;  // Move constructor
		~HashTable();
		HashTable<T, U>& operator=(const HashTable<T, U>& table);  // Copy assignment
		HashTable<T, U>& operator=(HashTable<T, U>&& table) noexcept;  // Move assignment
		template <typename V> U& operator[](V&& key);
		const U& operator[](const T& key) const;
		bool empty() const;
		std::size_t size() const;
		bool contains(const T& key) const;
		template <typename V, typename W> void insert(V&& key, W&& value);
		template <typename V> U& find(V&& key);
		const U& find(const T& key) const;
		void remove(const T& key);
		void clear();
		std::size_t bucketCount() const;
		std::size_t bucket(const T& key) const;
		std::size_t bucketSize(std::size_t bucketIndex) const;
		float loadFactor() const;
		float maxLoadFactor() const;
		void maxLoadFactor(float max);
		void reserve(std::size_t count);
		void rehash(std::size_t count = 1);
		HTIter begin() const;
		HTIter end() const;
		
	private:
		BucketLink** buckets{};
		std::size_t numBuckets{};
		std::size_t numPairs{ 0 };
		float maxLoad{ 1.0 };
		std::hash<T> hasher{};

		template <typename V> BucketLink* getBucketLink(V&& key);
		BucketLink* getBucketLink(const T& key) const;

		class BucketLink
		{
		public:
			T key{};
			U value{};
			BucketLink* next{ nullptr };

			BucketLink();
		};

		class HTIter
		{
		public:
			HTIter(BucketLink** bucketHead, BucketLink* currentLink, std::size_t bucketsLeft);
			const T& operator*();
			void operator++();
			bool operator==(const HTIter& iterator) const;
			bool operator!=(const HTIter& iterator) const;

		protected:
			BucketLink** bucketHead{ nullptr };
			BucketLink* currentLink{ nullptr };
			std::size_t bucketsLeft;  // The number of buckets left until the end (one past the end of the bucket array)
		};
	};
}
#include "HashTable.hpp"
#endif
