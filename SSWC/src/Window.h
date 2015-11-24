#pragma once

#include "CSpiceCore.h"
#include "Time.h"

#include <vector>

#define WINDOW_MAX_INTERVALS 1024

struct Interval
{
public:
	Interval(double left, double right) : left(0.0), right(0.0)
	{
		Set(left, right);
	}

	bool Set(double left, double right)
	{
		if(left > right)
			return false;

		this->left = left;
		this->right = right;

		return true;
	}

	double GetLeft() const
	{
		return left;
	}
	double GetRight() const
	{
		return right;
	}

private:
	double left;
	double right;
};

class Window
{
public:
	Window();
	Window(SpiceCell cell);

	std::vector<Interval> GetIntervals() const;

	bool IsIncluded(double point) const;
	bool IsIncluded(double left, double right) const;
	bool IsIncluded(const Interval& interval) const;

	SpiceCell& GetSpiceCell();

private:
	SpiceCell cell;
};