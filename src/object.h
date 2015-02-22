
#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "vector.h"
#include "matrix.h"

namespace ndim {
	
	typedef Vector<4> Vertex;

	class Facet {
	public:
		std::vector<Vertex> vertices;
	};

	class Object {
	public:
		std::vector<Facet> facets;
		void move(Vector<4> vector) {
			for (auto &facet : facets)
				for (auto &vertex : facet.vertices)
					vertex = vertex + vector;
		}
		virtual bool isInside(Vector<4> point, double tolerance = 0.0) { 
			throw std::logic_error("Not implemented"); 
		}
		virtual bool isHit(Vector<4> point, Vector<4> direction, double tolerance = 0.0) {
			throw std::logic_error("Not implemented");
		}
	};

}

#endif
