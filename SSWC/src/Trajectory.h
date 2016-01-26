#pragma once
#include "windows.h"

#include "Math\Vector3.h"
#include <vector>

#include <gl\GL.h>
#include <gl\GLU.h>


class Trajectory
{
protected:
	std::vector<Vector3> path;
	float lineWidth = 1.0f;
	float red = 255.0f;
	float green = 255.0f;
	float blue = 255.0f;
public:
	Trajectory();
	//Traectory(float lineWidth, Vector3 rgb);
	~Trajectory(){};

	void Render();
	void PushBack(Vector3 newPos);
};