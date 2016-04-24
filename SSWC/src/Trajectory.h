#pragma once
#include "windows.h"

#include "Math\Vector3.h"
#include "Math\Quantity.h"

#include "CSpice\SpaceObject.h"
#include "CSpice\Date.h"

#include <vector>

#include <gl\GL.h>
#include <gl\GLU.h>


class Trajectory
{
protected:
	std::vector<Vector3> path;

	const SpaceObject& obj;
	Time historyDuration;
	Time stepDuration;
	Date startDate;
	const Frame& frame;

	float lineWidth = 1.0f;
	float red = 255.0f;
	float green = 255.0f;
	float blue = 255.0f;
public:
	Trajectory(const SpaceObject& obj, const Frame& frame);
	//Traectory(float lineWidth, Vector3 rgb);
	~Trajectory(){};

	void SetDateParams(Date startDate, Time historyDuration, int resolution);

	std::vector<Vector3> GetTrajectory(Date curDate, const LengthUnit& unit) const;

	void Render();
	// void Render(Date curDate);
	void PushBack(Vector3 newPos);

private:
	Date GetFromDate(Date curDate) const
	{
		Date fromDate = curDate - historyDuration;
		if (fromDate < startDate)
			fromDate = startDate;

		return fromDate;
	}

	Date GetToDate(Date curDate) const
	{
		return curDate;
	}

	Vector3 GetPosition(Date date, const LengthUnit& unit) const
	{
		Vector3T<Length> pt = obj.GetPosition(date, frame);

		return Vector3(pt.x.ValueIn(unit), pt.y.ValueIn(unit), pt.z.ValueIn(unit));
	}
};