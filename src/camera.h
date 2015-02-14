
#ifndef CAMERA_H
#define CAMERA_H

#include <cmath>

#include "vector.h"
#include "matrix.h"

namespace ndim {

	class Camera {

	public:

		Vector4D center;
		Matrix4D directions;
		
		Vector3D project(Vector4D &vector) {
			auto result4D = vector - center;
			result4D = directions * result4D + center;
			return result4D.paralelProjection();
		}
		
		void move(double distance)
		{
			center = center + directions[0] * distance;
		}

		void rotate(Matrix4D rotation)
		{
			directions = rotation * directions;
		}

	private:

		Vector4D transform(Vector4D &vector);

	};

}

#endif
