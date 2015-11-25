#include "Date.h"

Date::Date(double et)
{
	etTime = et;
}

Date::Date(std::string str)
{
	CSPICE_ASSERT(str2et_c(str.c_str(), &etTime));
}

double Date::AsDouble() const
{
	return etTime;
}

std::string Date::AsString(const std::string& format) const
{
	char timeStr[FORMAT_STRING_BUFFER];
	CSPICE_ASSERT(timout_c(etTime, format.c_str(), FORMAT_STRING_BUFFER, timeStr));

	return std::string(timeStr);
}
