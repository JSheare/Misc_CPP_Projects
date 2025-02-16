#ifndef JADT_EDGE_H
#define JADT_EDGE_H

namespace JADT
{
	template <typename T, typename U>
	class Vertex;

	template <typename T, typename U>
	struct Edge
	{
		Vertex<T, U>& edge{};
		int weight{};
	};
}
#endif
