#ifndef EDGE_H
#define EDGE_H

namespace JADT
{
	template <typename T, typename U>
	class Vertex;

	template <typename T, typename U>
	struct Edge
	{
		Vertex<T, U>& m_edge{};
		int m_weight{};
	};
}
#endif
