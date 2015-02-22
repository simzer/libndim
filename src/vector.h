
#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>
#include <initializer_list>
#include <string>

namespace ndim {

	template <int dimensions> class Vector 
	{

	public:

		Vector()
		{
			for (int axis = 0; axis < dimensions; axis++) 
				c[axis] = 0;
		}

		Vector(std::initializer_list<double> values)
		{
			auto pvalue = values.begin();
			for (int axis = 0; axis < dimensions; axis++) {
				c[axis] = *(pvalue);
				++pvalue;
			}
		}

		static Vector null()
		{
			return Vector();
		}

		static Vector unit(int axis)
		{
			Vector unit;
			unit.c[axis] = 1.0;
			return unit;
		}

		int getDimensions() {
			return dimensions;
		}

		double &operator[](int axis) 
		{
			return c[axis];
		}

		Vector operator-(const Vector &source) const 
		{
			Vector result;
			for (int axis = 0; axis < dimensions; axis++)
				result.c[axis] = c[axis] - source.c[axis];
			return result;
		}

		Vector operator+(const Vector &vector) const 
		{
			Vector result;
			for (int axis = 0; axis < dimensions; axis++)
				result.c[axis] = c[axis] + vector.c[axis];
			return result;
		}

		double operator*(const Vector &vector) const 
		{
			double result;
			for (int axis = 0; axis < dimensions; axis++)
				result += c[axis] * vector.c[axis];
			return result;
		}

		Vector operator*(double scalar) const 
		{
			Vector result;
			for (int axis = 0; axis < dimensions; axis++)
				result.c[axis] = scalar * c[axis];
			return result;
		}

		Vector operator~() const
		{
			return this->normalised();
		}

		Vector normalised() const 
		{
			return *this * (1 / this->abs());
		}
		
		operator double() const {
			return this->abs();
		}

		double abs() const
		{
			double abs = 0;
			for (int axis = 0; axis < dimensions; axis++)
				abs += c[axis] * c[axis];
			return sqrt(abs);
		}

		Vector interpolate(const Vector &vector1,
		                     const Vector &vector2,
		                     double factor)
		{
			return vector1 * (1.0 - factor) + vector2 * factor;
		}

		operator std::string()
		{
			std::string res = "{ ";
			for (int axis = 0; axis < dimensions; axis++)
				res += std::to_string(c[axis]) + "\t";
			res += "}";
			return res;
		}

		Vector operator^(const Vector &vector) const // cross product
		{
			throw std::logic_error("Not defined for other then 3D vectors");
		}

		Vector normal(const Vector v1, const Vector v2)
		{
			throw std::logic_error("Not defined for other then 3D vectors");
		}

		Vector<dimensions-1> centralProjection()
		{
			Vector<dimensions-1> result;
			for (int axis = 0; axis < dimensions - 1; axis++)
				result[axis] = c[axis] / c[dimensions - 1];
			return result;
		}

		Vector<dimensions - 1> paralelProjection()
		{
			Vector<dimensions - 1> result;
			for (int axis = 0; axis < dimensions - 1; axis++)
				result[axis] = c[axis];
			return result;
		}

		Vector<dimensions - 1> radialProjection()
		{
			auto result = paralelProjection();
			return result * (abs() / result.abs());
		}

		const double* data() const
		{
			return c;
		}

		int size() const
		{
			return dimensions;
		}

	protected:

		double c[dimensions];

	};

	template<>
	Vector<3> Vector<3>::operator^(const Vector<3> &vector) const
	{
		Vector<3> cross;
		cross.c[0] = c[1] * vector.c[2] - vector.c[1] * c[2];
		cross.c[1] = c[2] * vector.c[0] - vector.c[2] * c[0];
		cross.c[2] = c[0] * vector.c[1] - vector.c[0] * c[1];
		return cross;
	}

	template<>
	Vector<3> Vector<3>::normal(const Vector<3> v1,
	                                const Vector<3> v2)
	{
		return (v1 ^ v2).normalised();
	}

	typedef Vector<3> Vector3D;
	typedef Vector<4> Vector4D;

}

#endif
