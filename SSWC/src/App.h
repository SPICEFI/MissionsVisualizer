#pragma once

#include "CSpice/CSpice.h"

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

	void LoadChildren(const SpaceObject& parent, bool includeSelf = false, bool recursive = false);
	void LoadParent(const SpaceObject& child, bool includeMassCenter, bool includeSelf = false, bool recursive = false);
	void LoadMoons(const SpaceObject& parent);
	void LoadMoons(const std::string& parentName);
	void LoadSolarSystem(bool entire = false);
	void LoadAllAvailableObjects();

	void AddObject(const SpaceObject& obj);

	size_t GetObjectsLength() const;
	SpaceObject& GetObjectByIndex(size_t idx);

	bool CheckObjectExists(long id);
	bool CheckObjectExists(const std::string& name);
	bool CheckObjectExists(const SpaceObject& sample);

	SpaceObject& RetrieveObject(long id);
	SpaceObject& RetrieveObject(const std::string& name);
	SpaceObject& RetrieveObject(const SpaceObject& sample);

	std::vector<SpaceObject*> GetLoadedPlanets();
	std::vector<SpaceObject*> GetLoadedMoonsOf(const SpaceObject& planet);
	std::vector<SpaceObject*> GetLoadedMoonsOf(const std::string& planetName);
	std::vector<SpaceObject*> GetLoadedMoons();
	std::vector<SpaceObject*> GetLoadedBarycenters();

private:
	std::vector<SpaceObject*> objects;
	Frame refFrame;

	DefaultUnits units;
};
