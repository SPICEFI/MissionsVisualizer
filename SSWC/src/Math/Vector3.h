//*********************************************
//*********************************************
//******This class is made to ease ************
//***********work with camera******************
//*********************************************
//*********************************************

#pragma once
#include <cmath>

#include <stdexcept>

class Vector3
{
public:
	float x, y, z;																		//Vector coordinates
public:
	Vector3();
	Vector3(float x, float y, float z);
	Vector3(double* ptr);

	~Vector3();

	void Set(float x, float y, float  z);
	void Assign(double* ptr);

	void Normalise();

	float Length() const;

	void MultVecByMatrix(float* matrix, Vector3& result) const;											//v2 - is vector v1, multiplied by rotation matrix
	void RotateAroudVec(const Vector3& around, float angle, Vector3& result) const;						//v3 - is vector v1, rotated around v2 on angle

	float DotProduct(const Vector3& rhs) const;
	Vector3 CrossProduct(const Vector3& rhs) const;

	float AngleBetween(const Vector3& rhs) const;

	Vector3 operator+(const Vector3& v1) const;
	Vector3 operator-(const Vector3& v1) const;
	Vector3& operator=(const Vector3& rVector);
	Vector3& operator*=(float scalar);
	Vector3 operator*(float scalar) const;
	float operator[](size_t i);

	friend Vector3& operator+=(Vector3& lVector, const Vector3& rVector);

	const static Vector3& i;
	const static Vector3& j;
	const static Vector3& k;

};

void RotationMatrix(float *matrix, const Vector3& axis, float angle);									//creates rotation matrix around axis on angle
