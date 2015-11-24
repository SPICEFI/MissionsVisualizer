#include "Window.h"

Window::Window()
{
	SPICEDOUBLE_CELL(newCell, 2 * WINDOW_MAX_INTERVALS);

	this->cell = newCell;
}

Window::Window(SpiceCell cell)
{
	SPICEDOUBLE_CELL(newCell, 2 * WINDOW_MAX_INTERVALS);

	copy_c(&cell, &newCell);

	this->cell = newCell;
}

std::vector<Interval> Window::GetIntervals() const
{
	SpiceCell cellCopy = cell;

	std::vector<Interval> intervals;

	long count = wncard_c(&cellCopy);
	for(long i = 0; i < count; i++)
	{
		double start, end;

		wnfetd_c(&cellCopy, i, &start, &end);

		intervals.push_back(Interval(start, end));
	}

	return intervals;
}

bool Window::IsIncluded(double point) const
{
	return IsIncluded(point, point);
}

bool Window::IsIncluded(double left, double right) const
{
	SpiceCell cellCopy = cell;
	SpiceBoolean isIncluded = wnincd_c(left, right, &cellCopy);

	if(failed_c())
		CSpiceUtil::SignalError("Include test failed");

	return isIncluded != SPICEFALSE;
}

bool Window::IsIncluded(const Interval& interval) const
{
	return IsIncluded(interval.GetLeft(), interval.GetRight());
}

SpiceCell& Window::GetSpiceCell()
{
	return cell;
}
