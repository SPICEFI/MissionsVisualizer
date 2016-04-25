#include "Scene.h"


Scene::Scene()
{
	minDistanceToRenderFullPlanet = 10;
}


Scene::~Scene()
{
}

void Scene::addPlanet(Planet& planet)
{
	planets.push_back( planet );

}

void Scene::render(Date t, App& app)
{
	for (int i = 0; i < planets.size(); i++)
	{
		planets.at(i).Render(t, app);
	}
}

const Planet& Scene::FindObjectWithID(int id)
{
	for (int i = 0; i < planets.size(); i++)
	{
		if (id < 0)
			break;
		int tempID = planets[i].GetID();
		if (tempID == id)
			return planets[i];
	}
}

void Scene::MarkPlanetWithID(int id, bool value)
{
	for (int i = 0; i < planets.size(); i++)
	{
		if (id < 0)
			break;
		int tempID = planets[i].GetID();
		if (tempID == id)
			planets[i].SetMark(value);
	}
}

void Scene::RenderPlanetWithID(int id, Date t, App& app)
{
	for (int i = 0; i < planets.size(); i++)
	{
		if (id < 0)
			break;
		int tempID = planets[i].GetID();
		if (tempID == id)
			planets[i].Render(t, app);
	}
}

float CalculateDistanceBetween(Vector3& first, Vector3& second)
{
	float res = sqrt((first.x - second.x)*(first.x - second.x) + (first.y - second.y)*(first.y - second.y) +
		(first.z - second.z)*(first.z - second.z));

	return res;
}

void Scene::UpdateTrackingDistances(Vector3& sceneCameraPosition, Date t, App& app)
{
	Vector3 planetPos;
	float dis;
	for (int i = 0; i < planets.size(); i++)
	{
		planetPos = planets[i].GetPosition(t, app);
		dis = CalculateDistanceBetween(sceneCameraPosition, planetPos);
		if (dis > minDistanceToRenderFullPlanet)
			planets.at(i).SetSimpleRenderMode(true);
		else
			planets.at(i).SetSimpleRenderMode(false);
	}
}