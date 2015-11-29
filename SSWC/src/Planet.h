#pragma once
//#include "SpaceBody.h"
//#include "Date.h"
#include "TGA.h"
//#include "Window.h"
//#include "Vector3T.h"
#include "App.h"
#include <vector>
#include "Traectory.h"
#include "Math\Matrix4x4.h"
//#include "Frame.h"

#undef GetObject

class Planet
{
public:
	SpaceBody body;
	TGA* texture;
	Traectory traectory;
	//std::vector<Vector3> traectory;
	Matrix4x4 m;
	float rotationMatrix[16];
public:
	Planet(const SpaceBody& body, TGA* texture);
	~Planet();

	void Render(Date t, App& app);
	int GetID()const{ return ID; }

protected:
	int ID;
};

