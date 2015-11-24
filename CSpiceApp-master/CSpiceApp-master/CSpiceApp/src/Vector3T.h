#pragma once

#include <cmath>

template<typename T>
class Vector3T
{
public:
	Vector3T()
	{

	}
	Vector3T(const T& x, const T& y, const T& z)
	{
		Set(x, y, z);
	}
	Vector3T(const T* pv)
	{
		Assign(pv);
	}

	void Set(const T& x, const T& y, const T& z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	void Assign(const T* pv)
	{
		x = *(pv + 0);
		y = *(pv + 1);
		z = *(pv + 2);
	}

	T Length() const
	{
		double ydivx = y / x;
		double zdivx = z / x;

		return x * std::sqrt(1.0 + ydivx*ydivx + zdivx*zdivx);
	}

	T& operator+=(const Vector3T& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;

		return *this;
	}
	T& operator-=(const Vector3T& rhs)
	{
		return (*this += (-rhs));
	}
	T& operator*=(double rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;

		return *this;
	}
	T& operator/=(double rhs)
	{
		return (*this *= (1.0 / rhs));
	}

public:
	T x;
	T y;
	T z;

	friend T operator+(const Vector3T& lhs, const Vector3T& rhs)
	{
		T x = lhs.x + rhs.x;
		T y = lhs.y + rhs.y;
		T z = lhs.z + rhs.z;

		return Vector3T(x, y, z);
	}
	friend T operator-(const Vector3T& lhs, const Vector3T& rhs)
	{
		return lhs + (-rhs);
	}
	friend T operator-(const Vector3T& op)
	{
		return (-1.0) * op;
	}
	friend T operator*(double lhs, const Vector3T& rhs)
	{
		T x = lsh * rhs.x;
		T y = lsh * rhs.y;
		T z = lsh * rhs.z;

		return Vector3T(x, y, z);
	}
	friend T operator*(const Vector3T& lhs, double rhs)
	{
		return rhs * lhs;
	}
	friend T operator/(const Vector3T& lhs, double rhs)
	{
		return lhs * (1.0)/rhs;
	}
};