#pragma once

#include "CSpice.h"
#include "Matrix3x3.h"
#include "Quantity.h"

#include <vector>
#include <string>
#include <ctime>
#include <fstream>
#include <iomanip>

#define REF_FRAME_DEFAULT Frame::J2000

class App
{
public:
	enum UnitsType
	{
		UT_DEFAULT,
		UT_METRIC,
		UT_IMPERIAL
	};

	struct DefaultUnits
	{
		LengthUnit lengthUnit;
		VelocityUnit velocityUnit;
		MassUnit massUnit;
		AccelerationUnit accUnit;
		GravitationalParameterUnit gmUnit;
	};

public:
	App();
	~App();

	void Init();
	void LoadKernel(const std::string& file) const;
	void SetLoggingFile(const std::string& file) const;

	void SetReferenceFrame(const Frame& ref);
	const Frame& GetReferenceFrame() const;

	void SetDefaultUnits(UnitsType units);
	void SetIndividualUnits(const DefaultUnits& defUnits);

	const LengthUnit& LengthUnit() const;
	const VelocityUnit& VelocityUnit() const;
	const MassUnit& MassUnit() const;
	const GravitationalParameterUnit& GMUnit() const;
	const AccelerationUnit& AccelerationUnit() const;

	void LoadChildren(const SpaceObject& parent, bool includeSelf = false, bool recursive = true);
	void LoadSolarSystem(bool onlyPlanets = false);

	void AddObject(const SpaceObject& obj);

	size_t GetObjectsLength() const;
	SpaceObject& GetObjectByIndex(size_t idx);

	bool FindObject(long id);
	bool FindObject(const std::string& name);
	bool FindObject(const SpaceObject& sample);

	SpaceObject& GetObject(long id);
	SpaceObject& GetObject(const std::string& name);
	SpaceObject& GetObject(const SpaceObject& sample);

private:
	std::vector<SpaceObject*> objects;
	Frame refFrame;

	DefaultUnits units;
};
