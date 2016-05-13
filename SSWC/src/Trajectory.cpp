#pragma once
#include "Trajectory.h"

Trajectory::Trajectory(const SpaceObject& obj, Frame frame, const LengthUnit& unit, const float distanceScale) : obj(obj), frame(frame), relativeTo(frame.GetCenterObject())
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

	//relativeTo = &this->frame.GetCenterObject();
};

void Trajectory::SetRelativeTo(const SpaceObject& obj)
{
	relativeTo = SpaceObject(obj);
}

void Trajectory::SetRelativeToFrameCenter()
{
	relativeTo = SpaceObject(frame.GetCenterObject());
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

std::vector<Vector3> Trajectory::GetAbsoluteIncrementalTrajectory(Date curDate)
{
	std::vector<Vector3> res;
	const std::deque<Vector3>& rel = GetIncrementalTrajectory(curDate);

	Vector3T<Length> pos = GetRelativeTo().GetPosition(curDate, frame);
	Vector3 centerPos = Vector3(pos.x.ValueIn(unit), pos.y.ValueIn(unit), pos.z.ValueIn(unit));

	for (const Vector3& v : rel)
		res.push_back(centerPos + v);

	return res;
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

	//Vector3T <Length> pos = relativeTo->GetPosition(t, /*app.GetReferenceFrame()*/this->frame);
	//Vector3 relativePosition (pos.x.ValueIn(this->unit)* distanceScale, pos.y.ValueIn(this->unit) * distanceScale, pos.z.ValueIn(this->unit) * distanceScale);
	if (this->IncrementalDefined())
	{
		/*const std::deque<Vector3>& path = this->GetIncrementalTrajectory(t);
		for (int i = 0; i < path.size(); i++)
			glVertex3f(path.at(i).x * distanceScale + relativePosition.x, path.at(i).y * distanceScale + relativePosition.y, path.at(i).z * distanceScale + relativePosition.z);*/

		std::vector<Vector3> tr = GetAbsoluteIncrementalTrajectory(t);
		for(const Vector3& v : tr)
			glVertex3f(v.x * distanceScale, v.y * distanceScale, v.z * distanceScale);
	}
	glDisable(GL_LINE_SMOOTH);
	glEnd();
	glEnable(GL_LIGHTING);
}