#pragma once
#include <Windows.h>

#include "Planet.h"
#include "Math\Vector3.h"

#include <vector>
#include <gl\GL.h>

class Scene
{
protected:
	std::vector<Planet> planets;

public:
	Scene();
	~Scene();

	void addPlanet(Planet& planet);
	void render(Date t, App& app);
	const Planet& FindObjectWithID(int ID);

	/*void calculatepositions(float time);
	void calculatepositions(Vector3 newPosition, unsigned int index);
	void addPlanet(float distanceFromTheSun, float orbitTime, float rotationTime, float radius, GLuint textureHandle);
	void addSatellite(int planetIndex, float distanceFromPlanet, float orbitTime, float rotationTime, float radius, GLuint textureHandle);
	void renderOrbits(MyVector3 newPoint, int index);
	void getPlanetPosition(int index, Vector3& vec);
	float getRadiusOfPlanet(int index);*/
};

