#pragma once

#include "glCameara.h"

bool modeWasChanged = false;

extern double scale;

glCamera::glCamera()
{
	position = Vector3(0.0, 0.0, 0.0);
	viewVec = Vector3(0.0, 1.0, 0.0);
	upVec = Vector3(0.0, 0.0, 1.0);
	speed = 0.03f;
}

void glCamera::PositionCamera(const Vector3& position, const Vector3& viewVec, const Vector3& upVec)
{
	this->position = position;
	this->viewVec = viewVec;
	this->upVec = upVec;
}

void glCamera::MoveCamera(float velocity)
{
	velocity *= speed;
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

	if (modeWasChanged)
	{
		SetCursorPos(middleX, middleY);
		return;
	}

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

void glCamera::Strafe(float velocity)
{
	velocity *= speed;
	position += strafeVec * velocity;
	viewVec += strafeVec * velocity;
}

void glCamera::Update(int centerX, int centerY, bool orientationMode)
{
	strafeVec = (viewVec - position).CrossProduct(upVec);
	strafeVec.Normalise();
	if (orientationMode)
	{
		setViewByMouse(centerX, centerY);
		modeWasChanged = false;
	}
	else
		modeWasChanged = true;

	transformOrientation();
	transformTranslation();
}

int glCamera::RetrieveObjectID(int x, int y, int width, int height, Scene scene, Date t, App& app)
{
	int objectsFound = 0;

	int viewportCoordinates[4] = { 0 };
	GLuint selectBuffer[32] = { 0 };//Array for storing ID of objects which were clicked 32 is Opengl requriments

	glSelectBuffer(32, selectBuffer);//Register buffer for objects
	glGetIntegerv(GL_VIEWPORT, viewportCoordinates);//To get current viewport coordinates
	glMatrixMode(GL_PROJECTION);

	glPushMatrix();
	glRenderMode(GL_SELECT);//Render objects without framebiffer changes
	glLoadIdentity();
	gluPickMatrix(x, viewportCoordinates[3] - y, 2, 2, viewportCoordinates);

	gluPerspective(60.0f, (GLfloat)(width) / (GLfloat)(height), 0.01f, 500.0f);

	glMatrixMode(GL_MODELVIEW);

	scene.render(t, app);

	objectsFound = glRenderMode(GL_RENDER);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	if (objectsFound > 0)
	{
		GLuint lowestDepth = selectBuffer[1];
		int selectedObject = selectBuffer[3];

		for (int i = 1; i < objectsFound; i++)
		{
			if (selectBuffer[(i * 4) + 1] < lowestDepth)
			{
				lowestDepth = selectBuffer[(i * 4) + 1];
				selectedObject = selectBuffer[(i * 4) + 3];
			}
		}
		return selectedObject;
	}
	else
		return 0;
}

void glCamera::FocusOnPlanet(const Planet& planet, Date t, App& app)
{
	//position = planet.GetPosition(t, app);
}

