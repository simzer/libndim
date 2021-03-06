
#include "cube.h"

namespace ndim {

Cube4D::Cube4D(double size, Vector4D position)
	: size(size),
	  position(position)
{

	const double coordinates[][4] = {
		{ -0.5, -0.5, -0.5, -0.5  },
		{  0.5, -0.5, -0.5, -0.5  },
		{ -0.5, 0.5, -0.5, -0.5   },
		{  0.5, 0.5, -0.5, -0.5   },
		{ -0.5, -0.5, 0.5, -0.5   },
		{  0.5, -0.5, 0.5, -0.5   },
		{ -0.5, 0.5, 0.5, -0.5    },
		{  0.5, 0.5, 0.5, -0.5    },
		{ -0.5, -0.5, -0.5, 0.5   },
		{  0.5, -0.5, -0.5, 0.5   },
		{ -0.5, 0.5, -0.5, 0.5    },
		{  0.5, 0.5, -0.5, 0.5    },
		{ -0.5, -0.5, 0.5, 0.5    },
		{  0.5, -0.5, 0.5, 0.5    },
		{ -0.5, 0.5, 0.5, 0.5     },
		{  0.5, 0.5, 0.5, 0.5     }
	};
 
	const int facetIndexes[][4] =
	{
		{ 8, 12, 14, 10 }, { 9, 11, 15, 13 }, { 8, 9, 13, 12 },
		{ 10, 14, 15, 11 }, { 8, 10, 11, 9 }, { 12, 13, 15, 14 },
		{ 0, 4, 6, 2 }, { 1, 3, 7, 5 }, { 0, 1, 5, 4 },
		{ 2, 6, 7, 3 }, { 0, 2, 3, 1 }, { 4, 5, 7, 6 },
		{ 0, 8, 0xC, 4 }, { 0, 8, 0xA, 2 }, { 0xA, 2, 6, 0xE }, { 0xE, 6, 4, 0xC },
		{ 1, 9, 0xB, 3 }, { 0xB, 3, 7, 0xF }, { 7, 0xF, 0xD, 5 }, { 0xD, 5, 1, 9 },
		{ 0xC, 0xD, 5, 4 }, { 8, 9, 1, 0 }, { 0xA, 0xB, 3, 2 }, { 0xE, 0xF, 7, 6 }
	};

	for (int i = 0; i < sizeof(facetIndexes) / sizeof(facetIndexes[0]); i++) {
		Facet facet;
		for (int j = 0; j < 4; j++) {
			const double *coors = coordinates[facetIndexes[i][j]];
			auto vertex = Vertex({ coors[0], coors[1], coors[2], coors[3] });
			vertex = vertex * size + position;
			facet.vertices.push_back(vertex);
		}
		facets.push_back(facet);
	}
}

bool Cube4D::isInside(Vector4D point, double tolerance) {
	Vector4D relPoint = (point - position) * (1.0/size);
	for (int i = 0; i < relPoint.getDimensions(); i++) {
		if (fabs(relPoint[i]) > 0.5 + tolerance) // todo: use epsilon
			return false;
	}
	return true;
}

bool Cube4D::isHit(Vector<4> point, Vector<4> direction, double tolerance) {
	// todo: move intersection calc to corresponding module
	Vector4D relPoint = (point - position) * (1.0 / size);
	Vector4D relDir = direction * (1.0 / size);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j <= 1; j++) {
			double pos = -0.5 + j;
			double t = (pos - relPoint[i]) / relDir[i];
			Vector4D section = relPoint + relDir * t;
			section = (section * size) + position;
			if (isInside(section, tolerance)) return true;
		}
	}
	return false;
}

}