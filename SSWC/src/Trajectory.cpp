#pragma once
#include "Trajectory.h"

Trajectory::Trajectory(const SpaceObject& obj, Frame frame, const LengthUnit& unit) : obj(obj), frame(frame)
{
	this->staticDefined = false;
	this->incrementalDefined = false;
	this->unit = unit;

	incStartDate = -1;
	incHistoryDuration = -1;
	incResolution = -1;
	incStepDuration = -1;
	nextPointDate = -1;
};

void Trajectory::SetStaticParams(Date startDate, Date endDate, int resolution)
{
	staticDefined = true;

	Time step = (endDate - startDate) / resolution;
	while (startDate < endDate)
	{
		staticTrajectory.push_back(GetPosition(startDate, unit));
		startDate += step;
	}
}

void Trajectory::SetIncrementalParams(Date startDate, Time historyDuration, int resolution)
{
	incrementalDefined = true;

	this->incHistoryDuration = historyDuration;
	this->incStartDate = startDate;
	this->incResolution = resolution;
	this->incStepDuration = historyDuration / resolution;

	nextPointDate = startDate;
}

const std::deque<Vector3>& Trajectory::GetIncrementalTrajectory(Date curDate)
{
	if (!incrementalDefined)
		throw std::runtime_error("SetIncrementalParams hasn't been called!");

	Date fromDate = GetFromDate(curDate);

	while (nextPointDate < curDate)
	{
		incrementalTrajectory.push_back(GetPosition(nextPointDate, unit));
		nextPointDate += incStepDuration;

		if (incrementalTrajectory.size() > incResolution)
			incrementalTrajectory.pop_front();
	}

	return incrementalTrajectory;
}

const std::vector<Vector3>& Trajectory::GetStaticTrajectory() const
{
	if (!staticDefined)
		throw std::runtime_error("SetStaticParams hasn't been called!");

	return staticTrajectory;
}

//void Trajectory::PushBack(Vector3 newPos)
//{
//	path.push_back(newPos);
//}
//
//void Trajectory::Render()
//{
//	glLineWidth(lineWidth);
//	glColor3f(red, green, blue);
//
//	glDisable(GL_LIGHTING);
//	
//	glBegin(GL_LINE_STRIP);
//	glEnable(GL_LINE_SMOOTH);
//	for (int i = 0; i < path.size(); i++)
//		glVertex3f(path.at(i).x, path.at(i).y, path.at(i).z);
//	glDisable(GL_LINE_SMOOTH);
//	glEnd();
//	
//	glEnable(GL_LIGHTING);
//}