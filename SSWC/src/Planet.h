#pragma once
#include "TGA.h"
#include "App.h"
#include <vector>
#include "Trajectory.h"
#include "Math\Matrix4x4.h"
#include "Font.h"

#undef GetObject

class Planet
{
public:
	SpaceBody body;
	TGA* texture;
	Trajectory trajectory;
	Matrix4x4 m;
	float rotationMatrix[16];
	bool simpleRender = false;
	bool clicked = false;
	double scale = 0.000004f;
	double distanceScale = 0.0000004f;
public:
	Planet(const SpaceBody& body, TGA* texture, const SpaceObject& obj, const Frame& frame, HDC hDC);
	~Planet();

	void SetSimpleRenderMode(bool value) { simpleRender = value; }
	void Render(Date t, App& app);
	int GetID()const{ return ID; }

	void SetMark(bool value){ marked = value; }

	void SetClicked(){ clicked = !clicked; }

	Vector3 GetPosition(Date t, App& app);
	double GetRadius(App& app) { return body.GetRadius().ValueIn(app.LengthUnit()) * scale; }
	void ShowMessageBoxWithInfo(App& app);

private:
	void RenderObject(Date t, App& app);
	void RenderAsPlanet(Date t, App& app);
	void RenderTrajectory(Date t, float lineWidth = 1.0f, float red = 255.0f, float green = 255.0f, float blue = 255.0f);
	void DrawBillboard(Vector3& planetPosition, double radius);

protected:
	int ID;
	bool marked;
	Font font;
};

