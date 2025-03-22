#ifndef JADT_HASH_TABLE_H
#define JADT_HASH_TABLE_H

#include <cstddef>
#include <functional>

namespace JADT
{
	template <typename T, typename U = T>
	class HashTable
	{
	private:
		class BucketLink;
		class HTIter;
		class ConstHTIter;

	public:
		HashTable(std::size_t numBuckets = 10);
		~HashTable();
		bool empty() const;
		std::size_t size() const;
		bool contains(const T& key);
		void insert(const T& key, const U& value);
		U& get(const T& key);
		const U& get(const T& key) const;
		U& operator[](const T& key);
		const U& operator[](const T& key) const;
		void remove(const T& key);
		void clear();
		HTIter begin();
		HTIter end();
		ConstHTIter cbegin();
		ConstHTIter cend();
		
	private:
		BucketLink** buckets;
		std::size_t numBuckets;
		std::size_t numValues{ 0 };
		std::hash<T> hasher;

		std::size_t getBucketIndex(const T& key) const;
		BucketLink* getBucketLink(const T& key);
		BucketLink* getBucketLink(const T& key) const;
		void rehash();

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
			T& operator*();
			void operator++();
			bool operator==(const HTIter& iterator) const;
			bool operator!=(const HTIter& iterator) const;

		protected:
			BucketLink** bucketHead{ nullptr };
			BucketLink* currentLink{ nullptr };
			std::size_t bucketsLeft;  // The number of buckets left until the end (one past the end of the bucket array)
		};

		class ConstHTIter : public HTIter
		{
			ConstHTIter(BucketLink** bucketHead, BucketLink* currentLink, std::size_t bucketsLeft);
			const T& operator*() const;
		};
	};
}
#include "HashTable.hpp"
#endif
