#pragma once
//#include "SpaceBody.h"
//#include "Date.h"
#include "TGA.h"
//#include "Window.h"
//#include "Vector3T.h"
#include "App.h"
#include <vector>
#include "Trajectory.h"
#include "Math\Matrix4x4.h"
//#include "Frame.h"

#undef GetObject

class Planet
{
public:
	SpaceBody body;
	TGA* texture;
	Trajectory trajectory;
	//std::vector<Vector3> traectory;
	Matrix4x4 m;
	float rotationMatrix[16];
	bool renderAsPlanet = true;
public:
	Planet(const SpaceBody& body, TGA* texture, const SpaceObject& obj, const Frame& frame);
	~Planet();

	void SetRenderMode(bool value) { renderAsPlanet = value; }
	void Render(Date t, App& app);
	int GetID()const{ return ID; }

	void SetMark(bool value)
	{
		marked = value;
	}

	const Vector3& GetPosition(Date t, App& app);

protected:
	int ID;
	bool marked;
};

