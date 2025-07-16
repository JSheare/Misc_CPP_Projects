#ifndef JML_SET_H
#define JML_SET_H

#include <cstddef>

namespace JML
{
	template <typename T>
	class Set
	{
	private:
		class BucketLink;
		class Iterator;

	public:
		Set(std::size_t reserveCount = 10, float maxLoad = 1.0);
		Set(const Set<T>& set);  // Copy constructor
		Set(Set<T>&& set) noexcept;  // Move constructor
		~Set();
		Set<T>& operator=(const Set<T>& table);  // Copy assignment
		Set<T>& operator=(Set<T>&& table) noexcept;  // Move assignment
		template <typename T1, typename U1> friend bool operator==(const Set<T1>& set1, const Set<T1>& set2);
		template <typename T1, typename U1> friend bool operator!=(const Set<T1>& set1, const Set<T1>& set2);
		bool empty() const;
		std::size_t size() const;
		bool contains(const T& key) const;
		template <typename V> void insert(V&& key);
		void remove(const T& key);
		void clear();
		std::size_t bucketCount() const;
		std::size_t bucket(const T& key) const;
		std::size_t bucketSize(std::size_t bucketIndex) const;
		float loadFactor() const;
		float maxLoadFactor() const;
		void maxLoadFactor(float newMax);
		void reserve(std::size_t count);
		void rehash(std::size_t count = 1);
		Iterator begin() const;
		Iterator end() const;

	private:
		BucketLink** buckets{};
		std::size_t numBuckets{};
		std::size_t numElements{ 0 };
		float maxLoad{ 1.0 };
		std::hash<T> hasher{};

		template <typename U> void addLink(U&& element);

		class BucketLink
		{
		public:
			T element{};
			BucketLink* next{ nullptr };
			BucketLink* prev{ nullptr };

			BucketLink();
		};

		class Iterator
		{
		public:
			Iterator(BucketLink** bucketHead, BucketLink* currentLink, std::size_t bucketsLeft);
			const T& operator*();
			void operator++();
			void operator++(int);
			bool operator==(const Iterator& iterator) const;
			bool operator!=(const Iterator& iterator) const;

		protected:
			BucketLink** bucketHead{ nullptr };
			BucketLink* currentLink{ nullptr };
			std::size_t bucketsLeft;  // The number of buckets left until the end (one past the end of the bucket array)
		};
	};
}
#include "Set.hpp"
#endif