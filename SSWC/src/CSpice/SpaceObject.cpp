#include "SpaceObject.h"

SpaceObject::SpaceObject(long spiceId, const std::string& name)
{
	Construct(spiceId, name);
}

SpaceObject::SpaceObject(const std::string& spiceName, const std::string& name)
{
	SpiceInt spiceId;
	SpiceBoolean found;
	CSPICE_ASSERT( bodn2c_c(spiceName.c_str(), &spiceId, &found) );

	if(!found)
		CSpiceUtil::SignalError("Cannot translate " + spiceName + " into ID code");

	Construct(spiceId, name);
}

void SpaceObject::Construct(long spiceId, const std::string& name)
{
	if(!ValidateId(spiceId))
		CSpiceUtil::SignalError("No such CSpice object ID code is defined: " + std::to_string(spiceId));

	this->spiceId = spiceId;

	if(name != "")
	{
		this->name = name;
	}
	else
	{
		this->name = GetSpiceName();
	}
}

SpaceObject::~SpaceObject()
{

}

SpaceObject* SpaceObject::Clone() const
{
	return new SpaceObject(*this);
}

long SpaceObject::GetSpiceId() const
{
	return spiceId;
}

std::string SpaceObject::GetSpiceName() const
{
	char objName[OBJECT_NAME_MAX_LENGTH];
	SpiceBoolean found;
	CSPICE_ASSERT(bodc2n_c(spiceId, OBJECT_NAME_MAX_LENGTH, objName, &found));

	return std::string(objName);
}

const std::string& SpaceObject::GetName() const
{
	return name;
}

Vector3T<Length> SpaceObject::GetPosition(const Date& t, const SpaceObject& relativeTo, const Frame& frame) const
{
	double etTime = t.AsDouble();
	long observerId = relativeTo.GetSpiceId();
	std::string frameName = frame.GetSpiceName();

	double position[3];
	double lt;

	CSPICE_ASSERT(spkgps_c(this->spiceId, etTime, frameName.c_str(), observerId, position, &lt));

	Length pos[3];
	for(int i = 0; i < 3; i++)
		pos[i] = Length(position[i], Units::Metric::kilometers);

	return Vector3T<Length>(pos);
}

Vector3T<Length> SpaceObject::GetPosition(const Date& t, const Frame& frame) const
{
	Frame::FrameInfo frInfo = frame.GetFrameInfo();
	
	return GetPosition(t, frame.GetCenterObject(), frame);
}

Vector3T<Velocity> SpaceObject::GetVelocity(const Date& t, const SpaceObject& relativeTo, const Frame& frame) const
{
	double etTime = t.AsDouble();
	long observerId = relativeTo.GetSpiceId();
	std::string frameName = frame.GetSpiceName();

	double state[6];
	double lt;

	CSPICE_ASSERT(spkgeo_c(this->spiceId, etTime, frameName.c_str(), observerId, state, &lt));

	Velocity vel[3];
	for(int i = 0; i < 3; i++)
	{
		vel[i] = Velocity(state[i + 3], Units::Metric::kmps);
	}

	return Vector3T<Velocity>(vel);
}

Vector3T<Velocity> SpaceObject::GetVelocity(const Date& t, const Frame& frame) const
{
	Frame::FrameInfo frInfo = frame.GetFrameInfo();

	return GetVelocity(t, SpaceObject(frInfo.centerId), frame);
}

Window SpaceObject::GetCoverage() const
{
	std::vector<KernelData> kernels = CSpiceUtil::GetLoadedKernels("SPK");

	Window coverage;

	for(size_t i = 0; i < kernels.size(); i++)
	{
		std::string file = kernels[i].filename;

		CSPICE_ASSERT( spkcov_c(file.c_str(), this->spiceId, &coverage.GetSpiceCell()) );
	}

	return coverage;
}

bool SpaceObject::IsBarycenter() const
{
	return SpaceObject::IsBarycenter(spiceId);
}

bool SpaceObject::IsPlanetaryBarycenter() const
{
	return SpaceObject::IsPlanetaryBarycenter(spiceId);
}

bool SpaceObject::IsSSB() const
{
	return SpaceObject::IsSSB(spiceId);
}

bool SpaceObject::IsPlanet() const
{
	return SpaceObject::IsPlanet(spiceId);
}

bool SpaceObject::IsMoon() const
{
	return SpaceObject::IsMoon(spiceId);
}

bool SpaceObject::IsBody() const
{
	return SpaceObject::IsBody(spiceId);
}

bool SpaceObject::IsSun() const
{
	return SpaceObject::IsSun(spiceId);
}

bool SpaceObject::ValidateId(long id)
{
	char objName[OBJECT_NAME_MAX_LENGTH];
	SpiceBoolean found;
	CSPICE_ASSERT(bodc2n_c(id, OBJECT_NAME_MAX_LENGTH, objName, &found));

	return found != SPICEFALSE;
}

bool SpaceObject::IsBarycenter(long id)
{
	if(ValidateId(id) == false)
		return false;

	return IsPlanetaryBarycenter(id) || IsSSB(id);
}

bool SpaceObject::IsPlanetaryBarycenter(long id)
{
	if(ValidateId(id) == false)
		return false;

	return (id > 0 && id < 10);
}

bool SpaceObject::IsSSB(long id)
{
	if(ValidateId(id) == false)
		return false;

	return id == SSB_SPICE_ID;
}

bool SpaceObject::IsPlanet(long id)
{
	if(ValidateId(id) == false)
		return false;

	return (id > 100 && id < 1000 && (id % 100) == 99);
}

bool SpaceObject::IsMoon(long id)
{
	if(ValidateId(id) == false)
		return false;

	return IsBody(id) && !IsPlanet(id) && !IsSun(id);
}

bool SpaceObject::IsBody(long id)
{
	if(ValidateId(id) == false)
		return false;

	return ((id > 100 && id < 1000) || IsSun(id));
}

bool SpaceObject::IsSun(long id)
{
	if(ValidateId(id) == false)
		return false;

	return id == SUN_SPICE_ID;
}

std::vector<long> SpaceObject::FindChildObjectIds(long id, bool includeMain)
{
	std::vector<long> ids;

	if(id == SSB_SPICE_ID)
	{
		for(int i = 1; i <= 10; i++)
		{
			if(ValidateId(i))
				ids.push_back(i);
		}
	}
	else if(IsPlanetaryBarycenter(id))
	{
		int upperSatelliteId;
		if(includeMain)
			upperSatelliteId = 99; // X99 - id of a most massive body in the X-th barycenter
		else
			upperSatelliteId = 98;

		for(int i = 1; i <= upperSatelliteId; i++)
		{
			long satelliteId = 100 * id + i;
			if(ValidateId(satelliteId))
				ids.push_back(satelliteId);
		}
	}

	return ids;
}

long SpaceObject::FindChildMassCenterId(long id)
{
	if(IsPlanetaryBarycenter(id))
		return id * 100 + 99;
	else if(IsSSB(id))
		return SUN_SPICE_ID;
	else
		return id;
}

std::vector<long> SpaceObject::FindMoonIds(long id)
{
	long barycenterId = SpaceObject::FindParentObjectId(id);

	if(!SpaceObject::IsPlanetaryBarycenter(barycenterId))
		CSpiceUtil::SignalError("FindMoonIds failed: 'id' argument is not a valid planet id");

	return FindChildObjectIds(barycenterId, false);
}

long SpaceObject::FindParentObjectId(long id)
{
	if(id == SUN_SPICE_ID || IsPlanetaryBarycenter(id))
	{
		return SSB.GetSpiceId();
	}
	else if(IsBody(id))
	{
		return id / 100;
	}
	else
	{
		return id;
	}
}

std::vector<long> SpaceObject::GetLoadedSpkIds()
{
	SPICEINT_CELL(cell, CELL_SIZE_LARGE);
	std::vector<KernelData> kernels = CSpiceUtil::GetLoadedKernels("SPK");
	for(size_t i = 0; i < kernels.size(); i++)
	{
		CSPICE_ASSERT(spkobj_c(kernels[i].filename.c_str(), &cell));
	}

	return CSpiceUtil::IntCellToVector(cell);
}

const SpaceObject SpaceObject::SSB = SpaceObject(SSB_SPICE_ID, "Solar System Barycenter");
const SpaceObject SpaceObject::Sun = SpaceObject(SUN_SPICE_ID, "Sun");
