#pragma once

#include <string>

#include "CSpiceCore.h"
#include "../Math/Quantity.h"

#define FORMAT_STRING_BUFFER 1024

class Date
{
public:
	Date(double et = 0.0);
	Date(std::string str);

	double AsDouble() const;
	std::string AsString(const std::string& format = "Mon DD YYYY HR:MN:SC (UTC+0) ::UTC+0") const;

	Date operator+(double rhs) const
	{
		return Date(etTime + rhs);
	}
	Date operator-(double rhs) const
	{
		return (*this) + (-rhs);
	}
	double operator-(const Date& rhs) const
	{
		return etTime - rhs.AsDouble();
	}
	Date& operator+=(double rhs)
	{
		etTime += rhs;
		return *this;
	}
	Date& operator-=(double rhs)
	{
		etTime -= rhs;
		return *this;
	}
	bool operator<(const Date& rhs) const
	{
		return this->AsDouble() < rhs.AsDouble();
	}
	bool operator>(const Date& rhs) const
	{
		return this->AsDouble() > rhs.AsDouble();
	}
	Date operator+(const Time& rhs) const
	{
		return *this + rhs.ValueIn(Units::Common::seconds);
	}
	Date operator-(const Time& rhs) const
	{
		return *this - rhs.ValueIn(Units::Common::seconds);
	}
	Date& operator+=(const Time& rhs)
	{
		etTime += rhs.ValueIn(Units::Common::seconds);

		return *this;
	}
	Date& operator-=(const Time& rhs)
	{
		etTime -= rhs.ValueIn(Units::Common::seconds);

		return *this;
	}

public:
	const static double second;
	const static double minute;
	const static double hour;
	const static double day;

private:
	double etTime;
};
