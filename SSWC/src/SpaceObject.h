#pragma once

#include <string>
#include <vector>

#include "CSpiceCore.h"
#include "Vector3.h"
#include "Vector3T.h"
#include "Frame.h"
#include "Date.h"
#include "Orientation.h"
#include "Window.h"

#define SSB_SPICE_ID 0
#define SUN_SPICE_ID 10

#define OBJECT_NAME_MAX_LENGTH 128

class SpaceObject
{
public:
	SpaceObject(long spiceId, const std::string& name = "");
	SpaceObject(const std::string& spiceName, const std::string& name = "");
	virtual ~SpaceObject();

	virtual SpaceObject* Clone() const;


	long GetSpiceId() const;
	std::string GetSpiceName() const;
	const std::string& GetName() const;

	Vector3T<Length> GetPosition(const Date& t, const SpaceObject& relativeTo, const Frame& frame) const;
	Vector3T<Length> GetPosition(const Date& t, const Frame& frame) const;
	Vector3T<Velocity> GetVelocity(const Date& t, const SpaceObject& relativeTo, const Frame& frame) const;
	Vector3T<Velocity> GetVelocity(const Date& t, const Frame& frame) const;

	Window GetCoverage() const;

	bool IsBarycenter() const;
	bool IsPlanetaryBarycenter() const;
	bool IsSSB() const;
	bool IsPlanet() const;
	bool IsMoon() const;
	bool IsBody() const;
	bool IsSun() const;


	static bool ValidateId(long id);

	static bool IsBarycenter(long id);
	static bool IsPlanetaryBarycenter(long id);
	static bool IsSSB(long id);
	static bool IsPlanet(long id);
	static bool IsMoon(long id);
	static bool IsBody(long id);
	static bool IsSun(long id);

	static std::vector<long> FindChildObjectIds(long id, bool includeMassCenter = true);
	static long FindChildMassCenterId(long id);
	static std::vector<long> FindMoonIds(long id);
	static long FindParentObjectId(long id);

	static std::vector<long> GetLoadedSpkIds();

private:
	void Construct(long spiceId, const std::string& name);

protected:
	long spiceId;
	std::string name;

public:
	static const SpaceObject SSB; // Solar System Barycenter
	static const SpaceObject Sun;
};
