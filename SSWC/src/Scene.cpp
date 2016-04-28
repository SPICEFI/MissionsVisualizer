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
	RenderStandaloneTrajectories(t, 1, 1, 1, 1);
	RenderStabdaloneSpacePoints(t, app);
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

void Scene::SteClickedToPlanetWithID(int id /*,bool value*/)
{
	for (int i = 0; i < planets.size(); i++)
	{
		if (id < 0)
			break;
		int tempID = planets[i].GetID();
		if (tempID == id)
			planets[i].SetClicked(/*value*/);
	}
}

void Scene::ShowInfoAboutPlanetWithID(int id, App& app)
{
	for (int i = 0; i < planets.size(); i++)
	{
		if (id < 0)
			break;
		int tempID = planets[i].GetID();
		if (tempID == id)
			planets[i].ShowMessageBoxWithInfo(app);
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
	float res = sqrtf((second.x - first.x)*(second.x - first.x) + (second.y - first.y)*(second.y - first.y) +
		(second.z - first.z)*(second.z - first.z));

	return res;
}

float Scene::UpdateTrackingDistances(Vector3& sceneCameraPosition, Date t, App& app)
{
	Vector3 planetPos;
	float dis = 0.0f;
	float minDis = 100.0f;
	int minDisPlanetID = -1;
	for (int i = 1; i < planets.size(); i++)
	{
		planetPos = planets[i].GetPosition(t, app);
		dis = CalculateDistanceBetween(sceneCameraPosition, planetPos);

		if (dis > minDistanceToRenderFullPlanet)
			planets.at(i).SetSimpleRenderMode(true);
		else
			planets.at(i).SetSimpleRenderMode(false);

		if (dis < minDis)
		{
			minDis = dis;
			minDisPlanetID = i;
		}
	}
	/*double res = 10 * planets.at(minDisPlanetID).GetRadius(app);
	if (minDis < res)
	{
		planets.at(minDisPlanetID).SetMark(true);
		return 0.004f;
	}
	else
	{
		planets.at(minDisPlanetID).SetMark(false);*/
		return 0.03f;
	//}
}

void Scene::AddTrajectoryAsSpaceObject(const SpaceObject& obj, Frame frame, Date startingDate)
{
	standaloneTrajectories.push_back(Trajectory(obj, frame, Units::Metric::kilometers));
	Time time(2, Units::Common::minutes);
	standaloneTrajectories.at(standaloneTrajectories.size() - 1).SetIncrementalParams(startingDate, time, 1000);
}

void Scene::RenderStandaloneTrajectories(Date t, float lineWidth, float red, float green, float blue)
{
	glDisable(GL_LIGHTING);
	for (int i = 0; i < standaloneTrajectories.size(); i++)
	{
		glLineWidth(lineWidth);
		glColor3f(red, green, blue);

		glBegin(GL_LINE_STRIP);
		glEnable(GL_LINE_SMOOTH);
		if (standaloneTrajectories[i].IncrementalDefined())
		{
			const std::deque<Vector3>& path = standaloneTrajectories[i].GetIncrementalTrajectory(t);
			for (int i = 0; i < path.size(); i++)
				glVertex3f(path.at(i).x * distanceScale, path.at(i).y * distanceScale, path.at(i).z * distanceScale);
		}
		glDisable(GL_LINE_SMOOTH);
		glEnd();
	}
	glEnable(GL_LIGHTING);
}

void Scene::AddStandaloneSpacePoint(const SpaceObject& obj, Font standalonePointFont)
{
	standaloneSpacePoints.push_back(obj);
	font = standalonePointFont;
}

void Scene::RenderStabdaloneSpacePoints(Date t, App& app)
{
	glDisable(GL_LIGHTING);
	for (int i = 0; i < standaloneSpacePoints.size(); i++)
	{
		glColor3f(1.0f, 1.0f, 0.0f);
		glPointSize(7.0f);
		glEnable(GL_POINT_SMOOTH);

		glBegin(GL_POINTS);
			Vector3T <Length> pos = standaloneSpacePoints.at(i).GetPosition(t, SpaceObject::SSB, app.GetReferenceFrame());
			Vector3 position(pos.x.ValueIn(app.LengthUnit())* distanceScale, pos.y.ValueIn(app.LengthUnit()) * distanceScale, pos.z.ValueIn(app.LengthUnit()) * distanceScale);
			glVertex3d(position.x, position.y, position.z);
		glEnd();

		glColor3f(1.0f, 1.0f, 0.0f);
		glRasterPos3f(position.x, position.y, position.z);
		font.glPrint(standaloneSpacePoints.at(i).GetName().c_str());
	}
	glEnable(GL_LIGHTING);
}