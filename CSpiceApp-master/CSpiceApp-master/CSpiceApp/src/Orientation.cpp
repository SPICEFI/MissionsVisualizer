#include "Orientation.h"

Orientation::Orientation(const Vector3& axisX, const Vector3& axisY, const Vector3& axisZ)
{
	this->axisX = axisX;
	this->axisY = axisY;
	this->axisZ = axisZ;
}

Vector3 Orientation::GetTiltAxis()
{
	return axisZ.CrossProduct(Vector3::k);
}

double Orientation::GetTiltAngle()
{
	return axisZ.AngleBetween(Vector3::k);
}

Vector3 Orientation::GetRotationAxis()
{
	return axisZ;
}

double Orientation::GetRotationAngle()
{
	Vector3 tiltedAxisX;
	Vector3::i.RotateAroudVec(Vector3::j, (float)GetTiltAngle(), tiltedAxisX);

	return tiltedAxisX.AngleBetween(axisX);
}
