#pragma once

#include "SpaceObject.h"
#include "Quantity.h"

#include <vector>
#include <array>

#define G 6.67384E-11

class SpaceBody : public SpaceObject
{
public:
	enum BulkParameter
	{
		BP_RADIUS,
		BP_GM,
		BP_MASS,
		BP_ACC,
		BP_POLE_RA,
		BP_POLE_DEC,
		BP_PM
	};

public:
	SpaceBody(long spiceId, const std::string& name = "");
	SpaceBody(const std::string& spiceName, const std::string& name = "");
	SpaceBody(const SpaceObject& obj);
	~SpaceBody();

	virtual SpaceBody* Clone() const;

	bool HasIAUFrame() const;
	Frame GetIAUFrame() const;
	bool HasDefaultFrame() const;
	Frame GetDefaultFrame() const;

	//Orientation GetOrientation(const Time& t, const Frame& ref = Frame::J2000) const;

	bool HasParameter(BulkParameter param) const;

	double GetSingleDimParam(BulkParameter param) const;
	std::vector<double> GetMultiDimParam(BulkParameter param) const;

	Length GetRadius() const;
	std::array<Length, 3> GetRadii() const;
	GravitationalParameter GetGM() const;
	Mass GetMass() const;
	Acceleration GetSurfaceAcceleration() const;

private:
	void Init();

private:
	//Frame* frame;
};
