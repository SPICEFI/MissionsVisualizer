#include "Scene.h"


Scene::Scene()
{
}


Scene::~Scene()
{
}

//void Scene::calculatePositions(float time)
//{
//	for (int i = 0; i < planets.size(); i++)
//	{
//		planets.at(i).calculatePosition(time);
//	}
//}
//void Scene::calculatepositions(Vector3 newPosition, unsigned int index)
//{
//	planets.at(index).calculatePosition(newPosition);
//}
void Scene::addPlanet(Planet& planet)
{
	planets.push_back( planet );

}
//void Scene::addPlanet(MyVector3 newPosition, float radius, GLuint textureHandle)
//{
//	planets.push_back(Planet(newPosition, radius, textureHandle));
//}
//
//void Scene::addSatellite(int planetIndex, float distanceFromPlanet, float orbitTime, float rotationTime, float radius, GLuint textureHandle)
//{
//	planets.at(planetIndex).addSatellite(distanceFromPlanet, orbitTime, rotationTime, radius, textureHandle);
//}
//void Scene::renderOrbits(/*MyVector3 newPoint,*/ int index)
//{
//	//for (int i = 0; i < planets.size(); i++)
//	//{
//		planets.at(index).renderOrbit(/*newPoint*/);
//	/*}*/
//}
//
//void Scene::getPlanetPosition(int index, MyVector3& vec)
//{
//	planets.at(index).getPosition(vec);
//}
//
//float Scene::getRadiusOfPlanet(int index)
//{
//	return planets.at(index).getRadius();
//}

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