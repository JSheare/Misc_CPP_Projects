#ifndef CITER_H
#define CITER_H

#include "Link.h"
#include "Iter.h"

namespace JADT
{
	template <typename T>
	class CIter : public Iter<T>
	{
	private:
		Link<T>* m_iter{};
	public:
		CIter(Link<T>* nodePtr)
			: Iter<T>{ nodePtr }
		{}

		const T& operator*()
		{
			return m_iter->m_data;
		}
	};
}

#endif
