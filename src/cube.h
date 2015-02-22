
#ifndef CUBE_H
#define CUBE_H

#include "object.h"

namespace ndim {

	class Cube4D : public Object
	{
		double size;
		Vector4D position;
	public:
		Cube4D(double size = 1.0, 
		       Vector4D position = Vector4D());
		bool isInside(Vector4D point, double tolerance = 0.0) override;
		bool isHit(Vector<4> point, Vector<4> direction, double tolerance = 0.0) override;
	};

}

#endif
