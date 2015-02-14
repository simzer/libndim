
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
			Vector4D result4D = vector - center;
			result4D = directions.inverse() * result4D;
			Vector3D result3D = result4D.paralelProjection();
			result3D = directions.minor(3, 3) * result3D;
			result3D = result3D + center.paralelProjection();
			return result3D;
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
