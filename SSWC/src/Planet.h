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
#include "Font.h"
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
	bool simpleRender = false;
	bool clicked = false;
public:
	Planet(const SpaceBody& body, TGA* texture, const SpaceObject& obj, const Frame& frame, HDC hDC);
	~Planet();

	void SetSimpleRenderMode(bool value) { simpleRender = value; }
	void Render(Date t, App& app);
	int GetID()const{ return ID; }

	void SetMark(bool value)
	{
		marked = value;
	}

	void SetClicked(bool value)
	{
		clicked = value;
	}

	//const Vector3& GetPosition(Date t, App& app);
	Vector3 GetPosition(Date t, App& app);

private:
	void RenderAsSun(Date t, App& app);
	void RenderAsPlanet(Date t, App& app);

protected:
	int ID;
	bool marked;
	Font font;
};

