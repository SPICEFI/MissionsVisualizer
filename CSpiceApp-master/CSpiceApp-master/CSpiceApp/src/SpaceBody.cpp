#include "SpaceBody.h"

SpaceBody::SpaceBody(long spiceId, const std::string& name) : SpaceObject(spiceId, name)
{
	Init();
}

SpaceBody::SpaceBody(const std::string& spiceName, const std::string& name) : SpaceObject(spiceName, name)
{
	Init();
}

SpaceBody::SpaceBody(const SpaceObject& obj) : SpaceObject(obj.GetSpiceId(), obj.GetName())
{
	Init();
}

void SpaceBody::Init()
{
	//frame = nullptr;
	//if(HasIAUFrame())
	//{
	//	UseBodyFrame(GetIAUFrame());
	//}
}

SpaceBody::~SpaceBody()
{
	//if(this->frame != nullptr)
	//{
	//	delete this->frame;
	//	this->frame = nullptr;
	//}
}

SpaceBody* SpaceBody::Clone() const
{
	return new SpaceBody(*this);
}

bool SpaceBody::HasIAUFrame() const
{
	std::string frameName = "IAU_" + GetSpiceName();

	long frcode;
	CSPICE_ASSERT( namfrm_c(frameName.c_str(), &frcode) );

	return frcode != 0;
}

Frame SpaceBody::GetIAUFrame() const
{
	std::string frameName = "IAU_" + GetSpiceName();

	return Frame(frameName);
}

bool SpaceBody::HasDefaultFrame() const
{
	char frameName[FRAME_NAME_MAX_LENGTH];
	long frameId;
	SpiceBoolean found;

	CSPICE_ASSERT(cidfrm_c(spiceId, FRAME_NAME_MAX_LENGTH, &frameId, frameName, &found));

	return found != SPICEFALSE;
}

Frame SpaceBody::GetDefaultFrame() const
{
	char frameName[FRAME_NAME_MAX_LENGTH];
	long frameId;
	SpiceBoolean found;

	CSPICE_ASSERT(cidfrm_c(spiceId, FRAME_NAME_MAX_LENGTH, &frameId, frameName, &found));

	if(!found)
		CSpiceUtil::SignalError("No such frame available");

	return Frame(frameId);
}

//Orientation SpaceBody::GetOrientation(const Time& t, const Frame& ref) const
//{
//	return frame->GetOrientation(t, ref);
//}

bool SpaceBody::HasParameter(BulkParameter param) const
{
	switch(param)
	{
	case BP_RADIUS:
		return bodfnd_c(spiceId, "RADII") != SPICEFALSE;
		break;

	case BP_GM:
		return bodfnd_c(spiceId, "GM") != SPICEFALSE;
		break;

	case BP_MASS:
		return HasParameter(BP_GM);
		break;

	case BP_ACC:
		return HasParameter(BP_GM) && HasParameter(BP_RADIUS);
		break;

	case BP_POLE_RA:
		return	(bodfnd_c(spiceId, "POLE_RA") != SPICEFALSE);
		break;

	case BP_POLE_DEC:
		return	(bodfnd_c(spiceId, "POLE_DEC") != SPICEFALSE);
		break;

	case BP_PM:
		return	(bodfnd_c(spiceId, "PM") != SPICEFALSE);
		break;
	}

	return false;
}

double SpaceBody::GetSingleDimParam(BulkParameter param) const
{
	double value = -1.0;
	long dim;

	std::vector<double> radii;
	double GM;
	double radius;

	switch(param)
	{
	case BP_RADIUS:
		radii = GetMultiDimParam(BP_RADIUS);
		value = std::pow(radii[0] * radii[1] * radii[2], 1.0 / 3.0); // volumetric mean
		break;

	case BP_GM:
		CSPICE_ASSERT( bodvcd_c(spiceId, "GM", 1, &dim, &value) );
		//value *= 1000.0 * 1000.0 * 1000.0; // km^3 -> m^3
		break;

	case BP_MASS:
		GM = GetSingleDimParam(BP_GM);
		value = GM / G;
		break;

	case BP_ACC:
		GM = GetSingleDimParam(BP_GM);
		radius = GetSingleDimParam(BP_RADIUS);
		value = GM / (radius * radius);
		break;
	}

	return value;
}

std::vector<double> SpaceBody::GetMultiDimParam(BulkParameter param) const
{
	std::vector<double> values;
	long dim;

	switch(param)
	{
	case BP_RADIUS:
		double radii[3];
		CSPICE_ASSERT( bodvcd_c(spiceId, "RADII", 3, &dim, radii) );
		//for(int i = 0; i < 3; i++)
		//	radii[i] *= 1000.0; // km -> m
		for(int i = 0; i < 3; i++)
			values.push_back(radii[i]);
		break;

	case BP_POLE_RA:
		double pole_ra[3];
		CSPICE_ASSERT(bodvcd_c(spiceId, "POLE_RA", 3, &dim, pole_ra));
		for(int i = 0; i < 3; i++)
			values.push_back(pole_ra[i]);
		break;

	case BP_POLE_DEC:
		double pole_dec[3];
		CSPICE_ASSERT(bodvcd_c(spiceId, "POLE_DEC", 3, &dim, pole_dec));
		for(int i = 0; i < 3; i++)
			values.push_back(pole_dec[i]);
		break;

	case BP_PM:
		double pm[3];
		CSPICE_ASSERT(bodvcd_c(spiceId, "PM", 3, &dim, pm));
		for(int i = 0; i < 3; i++)
			values.push_back(pm[i]);

		break;
	}

	return values;
}

Length SpaceBody::GetRadius() const
{
	double value = GetSingleDimParam(BP_RADIUS);

	return Length(value, Units::Metric::kilometers);
}

std::array<Length, 3> SpaceBody::GetRadii() const
{
	std::array<Length, 3> radii;
	std::vector<double> values = GetMultiDimParam(BP_RADIUS);
	for(int i = 0; i < 3; i++)
	{
		radii[i] = Length(values[i], Units::Metric::kilometers);
	}

	return radii;
}

GravitationalParameter SpaceBody::GetGM() const
{
	double value = GetSingleDimParam(BP_GM);

	return GravitationalParameter(value, Units::Metric::gm);
}

Mass SpaceBody::GetMass() const
{
	double value = GetSingleDimParam(BP_MASS);

	return Mass(value, Units::Metric::kilograms);
}

Acceleration SpaceBody::GetSurfaceAcceleration() const
{
	double value = GetSingleDimParam(BP_ACC);

	return Acceleration(value, AccelerationUnit::DerivedUnit(Units::Metric::kilometers, Units::Common::seconds));
}
