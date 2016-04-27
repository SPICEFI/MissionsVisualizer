#pragma once
#include <Windows.h>
#include "Planet.h"
#include "Math\Vector3.h"

#include <vector>
#include <gl\GL.h>
#include <math.h>


class Scene
{
protected:
	std::vector<Planet> planets;
	std::vector<Trajectory> standaloneTrajectories;
	float minDistanceToRenderFullPlanet;
	double scale = 0.000004f;
	double distanceScale = 0.0000004f;
public:
	Scene();
	~Scene();

	void addPlanet(Planet& planet);
	void render(Date t, App& app);
	void MarkPlanetWithID(int id, bool value);
	void SteClickedToPlanetWithID(int id/*, bool value*/);
	void ShowInfoAboutPlanetWithID(int id, App& app);

	void RenderPlanetWithID(int id, Date t, App& app);

	const Planet& FindObjectWithID(int ID);

	void AddPlanetToTrackDistanceTo(Planet& planet);
	float UpdateTrackingDistances(Vector3& sceneCameraPosition, Date t, App& app);
	void AddTrajectoryAsSpaceObject(const SpaceObject& obj, Frame frame, Date startingDate);
	void RenderStandaloneTrajectories(Date t, float lineWidth, float red, float green, float blue);
};

