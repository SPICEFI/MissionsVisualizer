#pragma once

#include "glCameara.h"
#include <gl\GL.h>
#include <gl\GLU.h>

glCamera::glCamera()
{
	position = Vector3(0.0, 0.0, 0.0);
	viewVec = Vector3(0.0, 1.0, 0.0);
	upVec = Vector3(0.0, 0.0, 1.0);
}

void glCamera::PositionCamera(const Vector3& position, const Vector3& viewVec, const Vector3& upVec)
{
	this->position = position;
	this->viewVec = viewVec;
	this->upVec = upVec;
}

void glCamera::MoveCamera(float velocity)
{
	Vector3 vVec = viewVec - position;						//Here we get vector of our view
	
	vVec.Normalise();

	position += vVec * velocity;
	viewVec += vVec * velocity;
}

void glCamera::transformOrientation()
{
	gluLookAt(position.x, position.y, position.z,
		viewVec.x, viewVec.y, viewVec.z,
		upVec.x, upVec.y, upVec.z);
}

void glCamera::transformTranslation()
{
	glTranslatef(-position.x, -position.y, -position.z);
}

void glCamera::Rotate(float angle, float x, float y, float z)
{
	Vector3 newView;
	Vector3 curView;
	
	curView = viewVec - position;

	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);

	newView.x = (cosTheta + (1 - cosTheta) * x * x) * curView.x;
	newView.x += ((1 - cosTheta) * x * y - z * sinTheta) * curView.y;
	newView.x += ((1 - cosTheta) * x * z + y * sinTheta) * curView.z;

	newView.y = ((1 - cosTheta) * x * y + z * sinTheta) * curView.x;
	newView.y += (cosTheta + (1 - cosTheta) * y * y) * curView.y;
	newView.y += ((1 - cosTheta) * y * z - x * sinTheta) * curView.z;

	newView.z = ((1 - cosTheta) * x * z - y * sinTheta)   * curView.x;
	newView.z += ((1 - cosTheta) * y * z + x * sinTheta)   * curView.y;
	newView.z += (cosTheta + (1 - cosTheta) * z * z)   * curView.z;

	viewVec = position + newView;
}

void glCamera::setViewByMouse(int middleX, int middleY)
{
	POINT mousePos;

	//int middleX = width >> 1;
	//int middleY = height >> 1;

	float angleY = 0.0f;
	float angleZ = 0.0f;
	static float currentRotX = 0.0f;

	GetCursorPos(&mousePos);

	if ((mousePos.x == middleX) && (mousePos.y == middleY))
		return;

	SetCursorPos(middleX, middleY);

	angleY = (float)((middleX - mousePos.x)) / 1000.0f;
	angleZ = (float)((middleY - mousePos.y)) / 1000.0f;

	static float lastRotX = 0.0f;
	lastRotX = currentRotX;

	if (currentRotX > 1.0f)
	{
		currentRotX = 1.0f;

		if (lastRotX != 1.0f)
		{
			Vector3 vAxis = (viewVec - position).CrossProduct(upVec);
			vAxis.Normalise();
			Rotate(1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
		}
	}

	else if (currentRotX < -1.0f)
	{
		currentRotX = -1.0f;

		if (lastRotX != -1.0f)
		{
			Vector3 vAxis = (viewVec - position).CrossProduct(upVec);
			vAxis.Normalise();
			Rotate(-1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
		}
	}

	else
	{
		Vector3 vAxis = (viewVec - position).CrossProduct(upVec);
		vAxis.Normalise();
		Rotate(angleZ, vAxis.x, vAxis.y, vAxis.z);
	}

	Rotate(angleY, upVec.x, upVec.y, upVec.z); 
}

void glCamera::Strafe(float speed)
{
	position += strafeVec * speed;
	viewVec += strafeVec * speed;
}

void glCamera::Update(int centerX, int centerY)
{
	strafeVec = (viewVec - position).CrossProduct(upVec);
	strafeVec.Normalise();
	setViewByMouse(centerX, centerY);
	transformOrientation();
	transformTranslation();
}