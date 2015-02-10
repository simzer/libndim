
#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"

#include <string>

namespace Lib4D {

	template <int dimensions> class Matrix
	{

	public:

		Matrix()
		{
			for (int axis1 = 0; axis1 < dimensions; axis1++)
				for (int axis2 = 0; axis2 < dimensions; axis2++)
					v[axis1][axis2] = 0;
		}
		
		Matrix(Vector<dimensions> vectors[dimensions])
		{
			for (int axis1 = 0; axis1 < dimensions; axis1++)
				for (int axis2 = 0; axis2 < dimensions; axis2++)
					v[axis1][axis2] = vectors[axis1][axis2];
			return(result);
		}

		static Matrix unit()
		{
			Matrix<dimensions> unit;
			for (int axis = 0; axis < dimensions; axis++)
				unit.v[axis][axis] = 1.0;
			return unit;
		}

		static Matrix rotator(int axis1, int axis2, double angle)
		{
			Matrix rot = Matrix::unit();
			rot.v[axis1][axis1] = cos(angle);
			rot.v[axis1][axis2] = -sin(angle);
			rot.v[axis2][axis1] = sin(angle);
			rot.v[axis2][axis2] = cos(angle);
			return rot;
		}

		Vector<dimensions> operator*(Vector<dimensions> &vector)
		{
			Vector<dimensions> result;
			for (int row = 0; row < dimensions; row++)
				for (int col = 0; col < dimensions; col++)
					result[row] += vector[col] * v[row][col];
			return result;
		}

		Matrix operator*(Matrix &matrix)
		{
			Matrix result;
			for (int row = 0; row < dimensions; row++)
				for (int col = 0; col < dimensions; col++)
					for (int n = 0; n < dimensions; n++)
						result.v[row][col] += v[row][n] * matrix.v[n][col];
			return result;
		}

		Matrix operator*(double scalar)
		{
			int row, col;
			Matrix result = matrix;
			for (row = 0; row < dimensions; row++)
				for (col = 0; col < dimensions; col++)
					result.v[row][col] *= scalar;
			return result;
		}

		Vector<dimensions> &operator[](int index)
		{
			return v[index];
		}

		operator std::string()
		{
			std::string res = "{\n";
			for (int axis1 = 0; axis1 < dimensions; axis1++) {
				res += "{ ";
				for (int axis2 = 0; axis2 < dimensions; axis2++)
					res += std::to_string(matrix.v[axis1][axis2]) + "\t";
				res += "}\n";
			}
			res += "}\n";
			return res;
		}

	protected:

		Vector<dimensions> v[dimensions];

	};

	typedef Matrix<3> Matrix3D;
	typedef Matrix<4> Matrix4D;


}

#endif
