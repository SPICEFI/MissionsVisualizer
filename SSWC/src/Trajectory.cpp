#pragma once
#include "Trajectory.h"

Trajectory::Trajectory(const SpaceObject& obj, Frame frame, const LengthUnit& unit, const float distanceScale) : obj(obj), frame(frame)
{
	this->staticDefined = false;
	this->incrementalDefined = false;
	this->unit = unit;

	SetRelativeToFrameCenter();

	incStartDate = -1;
	incHistoryDuration = -1;
	incResolution = -1;
	incStepDuration = -1;
	nextPointDate = -1;
	this->distanceScale = distanceScale;
};

void Trajectory::SetRelativeTo(const SpaceObject& obj)
{
	relativeTo = &obj;
}

void Trajectory::SetRelativeToFrameCenter()
{
	relativeTo = nullptr;
}

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

void Trajectory::Render(Date t/*, App& app*/, float lineWidth, float red, float green, float blue)
{
	glLineWidth(lineWidth);
	glColor3f(red, green, blue);

	glDisable(GL_LIGHTING);

	glBegin(GL_LINE_STRIP);
	glEnable(GL_LINE_SMOOTH);

	Vector3T <Length> pos = relativeTo->GetPosition(t, /*app.GetReferenceFrame()*/this->frame);
	Vector3 relativePosition (pos.x.ValueIn(this->unit)* distanceScale, pos.y.ValueIn(this->unit) * distanceScale, pos.z.ValueIn(this->unit) * distanceScale);
	if (this->IncrementalDefined())
	{
		const std::deque<Vector3>& path = this->GetIncrementalTrajectory(t);
		for (int i = 0; i < path.size(); i++)
			glVertex3f(path.at(i).x * distanceScale + relativePosition.x, path.at(i).y * distanceScale + relativePosition.y, path.at(i).z * distanceScale + relativePosition.z);
	}
	glDisable(GL_LINE_SMOOTH);
	glEnd();
	glEnable(GL_LIGHTING);
}