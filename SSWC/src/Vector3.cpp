#include "Vector3.h"

const Vector3& Vector3::i = Vector3(1.0f, 0.0f, 0.0f);
const Vector3& Vector3::j = Vector3(0.0f, 1.0f, 0.0f);
const Vector3& Vector3::k = Vector3(0.0f, 0.0f, 1.0f);

void RotationMatrix(float *matrix, const Vector3& axis, float angle)
{
	float cos1 = cos(angle);
	float cos2 = 1 - cos1;
	float sin1 = sin(angle);

	matrix[0] = cos1 + axis.x * axis.x * cos2;
	matrix[1] = axis.x * axis.y * cos2 - axis.y * sin1;
	matrix[2] = axis.x * axis.z * cos2 + axis.x * sin1;

	matrix[3] = axis.y * axis.x * cos2 + axis.z * sin1;
	matrix[4] = cos1 + axis.y * axis.y * cos2;
	matrix[5] = axis.y * axis.z * cos2 - axis.x * sin1;

	matrix[6] = axis.z * axis.x * cos2 - axis.y * sin1;
	matrix[7] = axis.z * axis.y * cos2 + axis.x * sin1;
	matrix[8] = cos1 + axis.z * axis.z * cos2;
}

Vector3::Vector3()
{
	x = 0;
	y = 0;
	z = 0;
}

Vector3::Vector3(double* ptr)
{
	Assign(ptr);
}


Vector3::~Vector3()
{
}

Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3 Vector3::operator+(const Vector3& v1) const
{
	float newX = x + v1.x;
	float newY = y + v1.y;
	float newZ = z + v1.z;

	return Vector3(newX, newY, newZ);
}

Vector3 Vector3::operator-(const Vector3& v1) const
{
	float newX = x - v1.x;
	float newY = y - v1.y;
	float newZ = z - v1.z;

	return Vector3(newX, newY, newZ);
}

Vector3& Vector3::operator=(const Vector3& rVector)
{
	if (this == &rVector)
		return *this;

	this->x = rVector.x;
	this->y = rVector.y;
	this->z = rVector.z;

	return *this;
}

Vector3& Vector3::operator*=(float scalar)
{
	this->x *= scalar;
	this->y *= scalar;
	this->z *= scalar;

	return *this;
}

Vector3 Vector3::operator*(float scalar) const
{
	float newX = x * scalar;
	float newY = y * scalar;
	float newZ = z * scalar;

	return Vector3(newX, newY, newZ);
}

float Vector3::operator[](size_t i)
{
	switch(i)
	{
	case 0:
		return x;
		break;
	case 1:
		return y;
		break;
	case 2:
		return z;
		break;
	}

	throw std::invalid_argument("Vector3::operator[]: argument out of range");
}

float Vector3::Length() const
{
	return(sqrtf(x * x + y * y + z * z));
}

void Vector3::Normalise()
{
	*this *= (1 / this->Length());
}

Vector3& operator+=(Vector3& lVector, const Vector3& rVector)
{
	lVector.x += rVector.x;
	lVector.y += rVector.y;
	lVector.z += rVector.z;

	return(lVector);
}

void Vector3::MultVecByMatrix(float* matrix, Vector3& v2) const
{
	v2.x = this->x * matrix[0] + this->y * matrix[1] + this->z * matrix[2];
	v2.y = this->x * matrix[3] + this->y * matrix[4] + this->z * matrix[5];
	v2.z = this->x * matrix[6] + this->y * matrix[7] + this->z * matrix[8];
}

void Vector3::RotateAroudVec(const Vector3& v2, float angle, Vector3& v3) const
{
	float matrix[16];
	RotationMatrix(matrix, v2, angle);

	this->MultVecByMatrix(matrix, v3);
}

void Vector3::Set(float x, float y, float  z)
{
	x = x;
	y = y;
	z = z;
}

void Vector3::Assign(double* ptr)
{
	x = (float)*ptr;
	y = (float)*(ptr + 1);
	z = (float)*(ptr + 2);
}

float Vector3::DotProduct(const Vector3& rhs) const
{
	return x * rhs.x + y * rhs.y + z * rhs.z;
}

Vector3 Vector3::CrossProduct(const Vector3& rhs) const
{
	float resX = y * rhs.z - z * rhs.y;
	float resY = z * rhs.x - x * rhs.z;
	float resZ = x * rhs.y - y * rhs.x;

	return Vector3(resX, resY, resZ);
}

float Vector3::AngleBetween(const Vector3& rhs) const
{
	float cosinePhi = DotProduct(rhs) / (Length() * rhs.Length());

	return std::acosf(cosinePhi);
}
