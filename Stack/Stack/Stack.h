#ifndef JML_STACK_H
#define JML_STACK_H

#include <cstddef>
#include <initializer_list>

namespace JML
{
	template <typename T>
	class Stack
	{
	public:
		Stack();
		Stack(std::initializer_list<T> list);
		Stack(const Stack<T>& stack);  // Copy constructor
		Stack(Stack<T>&& stack) noexcept;  // Move constructor
		~Stack();
		Stack<T>& operator=(const Stack<T>& stack);  // Copy assignment
		Stack<T>& operator=(Stack<T>&& stack) noexcept;  // Move assignment
		friend bool operator==(const Stack<T>& stack1, const Stack<T>& stack2);
		friend bool operator!=(const Stack<T>& stack1, const Stack<T>& stack2);
		std::size_t size() const;
		bool empty() const;
		void clear();
		template <typename U> void push(U&& item);
		T& top();
		const T& top() const;
		void pop();

	private:
		static constexpr std::size_t BLOCKSIZE{ 512 / sizeof(T) ? 512 / sizeof(T) : 1 };
		std::size_t mapSize{ 0 };
		T** map{ nullptr };
		std::size_t topBlock{ 0 };

		std::size_t numItems{ 0 };
		std::size_t topIndex{ 0 };

		void resize();
	};
}
#include "Stack.hpp"
#endif