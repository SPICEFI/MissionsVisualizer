#pragma once

#include "Vector3.h"

class Orientation
{
public:
	Orientation(const Vector3& axisX, const Vector3& axisY, const Vector3& axisZ);

	Vector3 GetTiltAxis();
	double GetTiltAngle();

	Vector3 GetRotationAxis();
	double GetRotationAngle();

private:
	Vector3 axisX;
	Vector3 axisY;
	Vector3 axisZ;
};