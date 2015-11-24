#include "App.h"

App::App() : refFrame(REF_FRAME_DEFAULT)
{
	Init();
}

App::~App()
{
	for(size_t i = 0; i < objects.size(); i++)
	{
		delete objects[i];
		objects[i] = nullptr;
	}

	objects.clear();
}

void App::Init()
{
	CSpiceUtil::SetErrorHandlingParams("return", "null");
	SetDefaultUnits(UT_DEFAULT);
}

void App::LoadKernel(const std::string& file) const
{
	CSpiceUtil::LoadKernel(file);
}

void App::SetLoggingFile(const std::string& file) const
{
	CSpiceUtil::SetLoggingFile(file);
}

void App::SetReferenceFrame(const Frame& ref)
{
	refFrame = ref;
}

const Frame& App::GetReferenceFrame() const
{
	return refFrame;
}

void App::SetDefaultUnits(UnitsType units)
{
	this->units.accUnit = Units::Metric::ms2;
	this->units.gmUnit = Units::Metric::gm;

	switch(units)
	{
	case UT_DEFAULT:
		this->units.lengthUnit = Units::Metric::kilometers;
		this->units.velocityUnit = Units::Metric::kmps;
		this->units.massUnit = Units::Metric::kilograms;
		break;
	case UT_IMPERIAL:
		this->units.lengthUnit = Units::Imperial::miles;
		this->units.velocityUnit = Units::Imperial::mph;
		this->units.massUnit = Units::Imperial::pounds;
		break;
	case UT_METRIC:
		this->units.lengthUnit = Units::Metric::meters;
		this->units.velocityUnit = Units::Metric::mps;
		this->units.massUnit = Units::Metric::kilograms;
		break;
	}
}

const LengthUnit& App::LengthUnit() const
{
	return units.lengthUnit;
}

const VelocityUnit& App::VelocityUnit() const
{
	return units.velocityUnit;
}

const MassUnit& App::MassUnit() const
{
	return units.massUnit;
}

const GravitationalParameterUnit& App::GMUnit() const
{
	return units.gmUnit;
}

const AccelerationUnit& App::AccelerationUnit() const
{
	return units.accUnit;
}

void App::SetIndividualUnits(const DefaultUnits& defUnits)
{
	std::memcpy(&this->units, &defUnits, sizeof(DefaultUnits));
}

void App::LoadChildren(const SpaceObject& parent, bool includeSelf, bool recursive)
{
	long parentId = parent.GetSpiceId();

	std::vector<long> childrenIds = SpaceObject::FindChildObjectIds(parentId);

	if(includeSelf)
		AddObject(parent);

	for(size_t i = 0; i < childrenIds.size(); i++)
	{
		long childId = childrenIds[i];

		if(SpaceObject::IsBody(childId))
			AddObject(SpaceBody(childId));
		else
			AddObject(SpaceObject(childId));

		if(recursive)
			LoadChildren(SpaceObject(childId), false, true);
	}
}

void App::LoadParent(const SpaceObject& child, bool includeMassCenter, bool includeSelf, bool recursive)
{
	if(includeSelf)
		AddObject(child);

	long childId = child.GetSpiceId();
	long parentId = SpaceObject::FindParentObjectId(childId);

	if(childId == parentId)
		return;

	if(SpaceObject::IsBody(parentId))
		AddObject(SpaceBody(parentId));
	else
		AddObject(SpaceObject(parentId));

	if(includeMassCenter)
	{
		long massCenterId = SpaceObject::FindChildMassCenterId(parentId);
		if(massCenterId != parentId)
			AddObject(SpaceBody(massCenterId));
	}

	if(recursive)
		LoadParent(SpaceObject(parentId), includeMassCenter, false, true);
}

void App::LoadMoons(const SpaceObject& planet)
{
	long barycenterId = SpaceObject::FindParentObjectId(planet.GetSpiceId());

	if(!SpaceObject::IsPlanetaryBarycenter(barycenterId))
		CSpiceUtil::SignalError("LoadMoons failed: 'planet' argument is not a valid planet object");

	return LoadChildren(SpaceObject(barycenterId), false, false);
}

void App::LoadMoons(const std::string& parentName)
{
	return LoadMoons(SpaceObject(parentName));
}

void App::LoadSolarSystem(bool entire)
{
	if(!entire)
	{
		std::vector<long> barycenterIds = SpaceObject::FindChildObjectIds(SpaceObject::SSB.GetSpiceId());

		for(size_t i = 0; i < barycenterIds.size(); i++)
		{
			long planetId = barycenterIds[i] * 100 + 99;

			if(SpaceObject::IsPlanet(planetId) || SpaceObject::IsSun(planetId))
				AddObject(SpaceBody(planetId));
		}
	}
	else
	{
		LoadChildren(SpaceObject::SSB, true, true);
	}
}

void App::LoadAllAvailableObjects()
{
	std::vector<long> loadedIds = SpaceObject::GetLoadedSpkIds();
	for(size_t i = 0; i < loadedIds.size(); i++)
	{
		if(SpaceObject::IsBody(loadedIds[i]))
			AddObject(SpaceBody(loadedIds[i]));
		else if(SpaceObject::ValidateId(loadedIds[i]))
			AddObject(SpaceObject(loadedIds[i]));
	}

	AddObject(SpaceObject::SSB);
}

void App::AddObject(const SpaceObject& obj)
{
	if(CheckObjectExists(obj) == false)
	{
		objects.push_back(obj.Clone());
		LoadParent(obj, true, false, true);
	}
}

size_t App::GetObjectsLength() const
{
	return objects.size();
}

SpaceObject& App::GetObjectByIndex(size_t idx)
{
	return *objects[idx];
}

bool App::CheckObjectExists(long id)
{
	for(size_t i = 0; i < objects.size(); i++)
	{
		if(objects[i]->GetSpiceId() == id)
			return true;
	}

	return false;
}

bool App::CheckObjectExists(const std::string& name)
{
	for(size_t i = 0; i < objects.size(); i++)
	{
		if(objects[i]->GetName() == name)
			return true;
	}

	return CheckObjectExists(SpaceObject(name));
}

bool App::CheckObjectExists(const SpaceObject& sample)
{
	return CheckObjectExists(sample.GetSpiceId());
}

SpaceObject& App::RetrieveObject(long id)
{
	for(size_t i = 0; i < objects.size(); i++)
	{
		if(objects[i]->GetSpiceId() == id)
			return GetObjectByIndex(i);
	}

	CSpiceUtil::SignalError("RetrieveObject haven't found requested object");

	throw;
}

SpaceObject& App::RetrieveObject(const std::string& name)
{
	for(size_t i = 0; i < objects.size(); i++)
	{
		if(objects[i]->GetName() == name)
			return GetObjectByIndex(i);
	}

	return RetrieveObject(SpaceObject(name));
}

SpaceObject& App::RetrieveObject(const SpaceObject& sample)
{
	return RetrieveObject(sample.GetSpiceId());
}

std::vector<SpaceObject*> App::GetLoadedPlanets()
{
	std::vector<SpaceObject*> planets;
	for(size_t i = 0; i < objects.size(); i++)
	{
		if(objects[i]->IsPlanet())
			planets.push_back(objects[i]);
	}

	return planets;
}

std::vector<SpaceObject*> App::GetLoadedMoonsOf(const SpaceObject& planet)
{
	std::vector<SpaceObject*> moons;

	std::vector<long> allMoonIds = SpaceObject::FindMoonIds(planet.GetSpiceId());

	for(size_t i = 0; i < allMoonIds.size(); i++)
	{
		if(CheckObjectExists(allMoonIds[i]))
			moons.push_back(&RetrieveObject(allMoonIds[i]));
	}

	return moons;
}

std::vector<SpaceObject*> App::GetLoadedMoonsOf(const std::string& planetName)
{
	return GetLoadedMoonsOf(SpaceObject(planetName));
}

std::vector<SpaceObject*> App::GetLoadedMoons()
{
	std::vector<SpaceObject*> moons;
	for(size_t i = 0; i < objects.size(); i++)
	{
		if(objects[i]->IsMoon())
			moons.push_back(objects[i]);
	}

	return moons;
}

std::vector<SpaceObject*> App::GetLoadedBarycenters()
{
	std::vector<SpaceObject*> barycenters;
	for(size_t i = 0; i < objects.size(); i++)
	{
		if(objects[i]->IsBarycenter())
			barycenters.push_back(objects[i]);
	}

	return barycenters;
}
