#pragma once
#include "windows.h"

#include "Math\Vector3.h"
#include "Math\Quantity.h"

#include "CSpice\SpaceObject.h"
#include "CSpice\Date.h"
#include "App.h"

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

	//const SpaceObject* relativeTo;
	SpaceObject relativeTo;

	bool staticDefined;
	std::vector<Vector3> staticTrajectory;

	bool incrementalDefined;
	Date prevDate;
	std::deque<Vector3> incrementalTrajectory;
	Date incStartDate;
	Time incHistoryDuration;
	int incResolution;
	Time incStepDuration;
	Date nextPointDate;

	float lineWidth = 1.0f;
	float red = 255.0f;
	float green = 255.0f;
	float blue = 255.0f;

	float distanceScale;
public:
	Trajectory(const SpaceObject& obj, Frame frame, const LengthUnit& unit, const float distanceScale);
	//Traectory(float lineWidth, Vector3 rgb);
	~Trajectory(){};

	void SetRelativeTo(const SpaceObject& obj);
	void SetRelativeToFrameCenter();
	const SpaceObject& GetRelativeTo()
	{
		return relativeTo;
	}
	void SetStaticParams(Date startDate, Date endDate, int resolution);
	void SetIncrementalParams(Date startDate, Time historyDuration, int resolution);

	const std::deque<Vector3>& GetIncrementalTrajectory(Date curDate);
	std::vector<Vector3> GetAbsoluteIncrementalTrajectory(Date curDate);
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
	void Render(Date t/*, App& app*/, float lineWidth, float red, float green, float blue);

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
		Vector3T<Length> pt;
		//if (relativeTo == nullptr)
		//	pt = obj.GetPosition(date, frame);
		//else
		//	pt = obj.GetPosition(date, *relativeTo, frame);
		pt = obj.GetPosition(date, relativeTo, frame);

		return Vector3(pt.x.ValueIn(unit), pt.y.ValueIn(unit), pt.z.ValueIn(unit));
	}
};