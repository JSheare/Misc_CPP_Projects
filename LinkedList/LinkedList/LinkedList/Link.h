#ifndef LINK_H
#define LINK_H

#include <iostream>
namespace JADT
{
	template <typename T>
	class Link
	{
	public:
		T m_data{};
		Link* m_prev{ nullptr };
		Link* m_next{ nullptr };

		Link() = default;

		Link(T data)
			: m_data{ data }
		{}
	};
}

#endif