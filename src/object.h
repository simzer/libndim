
#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "vector.h"

namespace ndim {
	
	typedef Vector<4> Vertex;

	class Facet {
	public:
		std::vector<Vertex> vertices;
	};

	class Object {
	public:
		std::vector<Facet> facets;
	};

}

#endif
