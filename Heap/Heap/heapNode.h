#ifndef HEAPNODE_H
#define HEAPNODE_H

namespace JADT
{
	template <typename T, typename U>
	struct HeapNode
	{
		T m_key{};
		U m_value{};
		HeapNode<T, U>* m_left{ nullptr };
		HeapNode<T, U>* m_right{ nullptr };
	};
}

#endif
