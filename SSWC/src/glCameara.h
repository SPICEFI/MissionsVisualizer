#pragma once

#include <windows.h>
#include <math.h>
#include "Vector3.h"
class glCamera
{
public:
	Vector3 viewVec;											//direction of camera view
	Vector3 upVec;												//vertical vector
	Vector3 position;
	Vector3 strafeVec;


	glCamera();

	void PositionCamera(const Vector3& position, const Vector3& viewVec, const Vector3& upVec);
	void RotateAroundPoint(Vector3 center, float x, float y, float z);

	void MoveCamera(float velocity);
	void Strafe(float speed);
	void Rotate(float angle, float x, float y, float z);
	void setViewByMouse(int width, int height);

	void Update(int centerX, int centerY);

private:
	void transformOrientation();
	void transformTranslation();
};
				