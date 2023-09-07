#ifndef ITER_H
#define ITER_H

#include "Link.h"

namespace JADT
{
	template <typename T>
	class Iter
	{
	private:
		Link<T>* m_iter{};
	public:
		Iter(Link<T>* nodePtr)
			: m_iter{ nodePtr }
		{}

		T& operator*()
		{
			return m_iter->m_data;
		}

		void operator++()
		{
			if (m_iter->m_next)
			{
				m_iter = m_iter->m_next;
			}
			else
			{
				++m_iter;
			}
		}

		bool operator!=(Iter iterator)
		{
			return (m_iter != iterator.m_iter);
		}
	};
}

#endif
