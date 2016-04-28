#pragma once
#include "windows.h"

#include "Math\Vector3.h"
#include "Math\Quantity.h"

#include "CSpice\SpaceObject.h"
#include "CSpice\Date.h"

#include <vector>
#include <deque>

#include <gl\GL.h>
#include <gl\GLU.h>


class Trajectory
{
protected:
	//std::vector<Vector3> path;

	const SpaceObject& obj;
	Frame frame;
	LengthUnit unit;

	bool staticDefined;
	std::vector<Vector3> staticTrajectory;

	bool incrementalDefined;
	Date prevDate;
	//std::vector<Vector3> incrementalTrajectory;
	std::deque<Vector3> incrementalTrajectory;
	Date incStartDate;
	Time incHistoryDuration;
	int incResolution;
	Time incStepDuration;
	//Date lastPointDate;
	Date nextPointDate;

	float lineWidth = 1.0f;
	float red = 255.0f;
	float green = 255.0f;
	float blue = 255.0f;
public:
	Trajectory(const SpaceObject& obj, Frame frame, const LengthUnit& unit);
	//Traectory(float lineWidth, Vector3 rgb);
	~Trajectory(){};

	void SetStaticParams(Date startDate, Date endDate, int resolution);
	void SetIncrementalParams(Date startDate, Time historyDuration, int resolution);

	const std::deque<Vector3>& GetIncrementalTrajectory(Date curDate);
	const std::vector<Vector3>& GetStaticTrajectory() const;

	bool IncrementalDefined() const
	{
		return incrementalDefined;
	}

	bool StaticDefined() const
	{
		return staticDefined;
	}

	//void Render();
	// void Render(Date curDate);
	//void PushBack(Vector3 newPos);

private:
	Date GetFromDate(Date curDate) const
	{
		Date fromDate = curDate - incHistoryDuration;
		if (fromDate < incStartDate)
			fromDate = incStartDate;

		return fromDate;
	}

	Vector3 GetPosition(Date date, const LengthUnit& unit) const
	{
		Vector3T<Length> pt = obj.GetPosition(date, frame);

		return Vector3(pt.x.ValueIn(unit), pt.y.ValueIn(unit), pt.z.ValueIn(unit));
	}
};