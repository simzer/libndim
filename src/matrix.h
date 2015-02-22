
#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"

#include <string>

namespace ndim {

	template <int dimensions> class Matrix
	{
	public:

		typedef Vector<dimensions> VectorD;

		Matrix()
		{
			for (int axis1 = 0; axis1 < dimensions; axis1++)
				for (int axis2 = 0; axis2 < dimensions; axis2++)
					v[axis1][axis2] = 0;
		}
		
		Matrix(VectorD vectors[dimensions])
		{
			for (int axis1 = 0; axis1 < dimensions; axis1++)
				for (int axis2 = 0; axis2 < dimensions; axis2++)
					v[axis1][axis2] = vectors[axis1][axis2];
			return(result);
		}

		static Matrix unit()
		{
			Matrix unit;
			for (int axis = 0; axis < dimensions; axis++)
				unit.v[axis][axis] = 1.0;
			return unit;
		}

		static Matrix tensorProduct(VectorD v1, VectorD v2)
		{
			Matrix prod;
			for (int axis1 = 0; axis1 < dimensions; axis1++)
				for (int axis2 = 0; axis2 < dimensions; axis2++)
					prod[axis1][axis2] = v1[axis1] * v2[axis2];
			return(prod);
		}

		static Matrix crossProduct(VectorD v)
		{
			throw std::logic_error("Cross product matrix is not implemented for this dimension");
		}

		Matrix transpose()
		{
			Matrix res;
			for (int j = 0; j < dimensions; j++) {
				for (int i = 0; i < dimensions; i++) {
					res[i][j] = v[j][i];
				}
			}
			return res;
		}

		//todo: slow
		Matrix inverse()
		{
			Matrix res;
			double det = 1.0 / determinant();
			for (int j = 0; j < dimensions; j++) {
				for (int i = 0; i < dimensions; i++) {
					res[i][j] = (((i + j) % 2 == 1) ? -1.0 : 1.0)
					            * det * minor(j, i).determinant();
				}
			}
			return res;
		}

		double determinant() {
			double det = 0;
			for (int i = 0; i < dimensions; i++) {
				det += ((i % 2) == 0 ? 1.0 : -1.0) * v[0][i] * minor(0, i).determinant();
			}
			return det;
		}
		
		Matrix<dimensions-1> minor(int row, int col)
		{
			Matrix<dimensions - 1> res;
			int colCount = 0;
			int rowCount = 0;
			for (int i = 0; i < dimensions; i++) {
				if (i == row) continue;
				colCount = 0;
				for (int j = 0; j < dimensions; j++) {
					if (j == col) continue;
					res[rowCount][colCount] = v[i][j];
					colCount++;
				}
				rowCount++;
			}
			return res;
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

		static Matrix rotator(VectorD axis, double angle)
		{
			return   Matrix::unit() * cos(angle)
			       + Matrix::crossProduct(axis) * sin(angle)
			       + Matrix::tensorProduct(axis, axis) * (1 - cos(angle));
		}

		Matrix operator+(Matrix &vector)
		{
			Matrix result;
			for (int row = 0; row < dimensions; row++)
				for (int col = 0; col < dimensions; col++)
					result[row][col] = v[row][col] + vector[row][col];
			return result;
		}

		VectorD operator*(VectorD &vector)
		{
			VectorD result;
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
			Matrix result;
			for (row = 0; row < dimensions; row++)
				for (col = 0; col < dimensions; col++)
					result.v[row][col] *= scalar;
			return result;
		}

		VectorD &operator[](int index)
		{
			return v[index];
		}

		operator std::string()
		{
			std::string res = "{\n";
			for (int axis1 = 0; axis1 < dimensions; axis1++) {
				res += "{ ";
				for (int axis2 = 0; axis2 < dimensions; axis2++)
					res += std::to_string(v[axis1][axis2]) + "\t";
				res += "}\n";
			}
			res += "}\n";
			return res;
		}

	protected:

		VectorD v[dimensions];

	};

	typedef Matrix<3> Matrix3D;
	typedef Matrix<4> Matrix4D;


	template<>
	Matrix<3> Matrix<3>::crossProduct(Vector<3> v)
	{
		Matrix<3> prod;
		prod[0][1] = -v[2];
		prod[0][2] = v[1];
		prod[1][2] = -v[0];
		prod[1][0] = v[2];
		prod[2][0] = -v[1];
		prod[2][1] = v[0];
		return prod;
	}

	template<>
	double Matrix<1>::determinant()
	{
		return v[0][0];
	}



}

#endif
