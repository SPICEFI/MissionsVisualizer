#pragma once
#include "Trajectory.h"

Trajectory::Trajectory(const SpaceObject& obj, const Frame& frame) : obj(obj), frame(frame)
{

};

void Trajectory::SetDateParams(Date startDate, Time historyDuration, int resolution)
{
	this->historyDuration = historyDuration;
	this->startDate = startDate;
	if (resolution > 0)
		this->stepDuration = historyDuration / resolution;
}

std::vector<Vector3> Trajectory::GetTrajectory(Date curDate, const LengthUnit& unit) const
{
	std::vector<Vector3> trajectory;

	Date fromDate = GetFromDate(curDate);
	Date toDate = GetToDate(curDate);

	Date date = fromDate;
	while (date < toDate)
	{
		trajectory.push_back(GetPosition(date, unit));
	}

	return trajectory;
}

void Trajectory::PushBack(Vector3 newPos)
{
	path.push_back(newPos);
}

void Trajectory::Render()
{
	glLineWidth(lineWidth);
	glColor3f(red, green, blue);

	glDisable(GL_LIGHTING);
	
	glBegin(GL_LINE_STRIP);
	glEnable(GL_LINE_SMOOTH);
	for (int i = 0; i < path.size(); i++)
		glVertex3f(path.at(i).x, path.at(i).y, path.at(i).z);
	glDisable(GL_LINE_SMOOTH);
	glEnd();
	
	glEnable(GL_LIGHTING);
}