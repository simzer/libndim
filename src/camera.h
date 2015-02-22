
#ifndef CAMERA_H
#define CAMERA_H

#include <cmath>
#include <functional>

#include "vector.h"
#include "matrix.h"

namespace ndim {

	class Camera {

	public:

		std::function<bool(Vector4D, Vector4D)> isValidPosition;

		Vector4D center;
		Matrix4D directions;
		
		Vector3D project(Vector4D &vector) {
			Vector4D result4D = vector - center;
			result4D = directions * result4D;
			Vector3D result3D = result4D.paralelProjection();
//			Vector3D result3D = result4D.radialProjection();
//			result3D = directions.minor(3, 3) * result3D;
//			result3D = result3D + center.paralelProjection();
			return result3D;
		}
		
		void move(Vector4D movement)
		{
			auto newCenter = 
				center + directions.transpose() * movement;
			auto p = newCenter;
			auto d = directions/*.transpose()*/[3];
			if (isValidPosition && isValidPosition(p, d))
				center = newCenter;
		}

		void rotate(Matrix4D rotation)
		{
			auto newDir = rotation * directions;
			auto p = center;
			auto d = newDir/*.transpose()*/[3];
			if (isValidPosition && isValidPosition(p, d))
				directions = rotation * directions;
		}

	private:

		Vector4D transform(Vector4D &vector);

	};

}

#endif
