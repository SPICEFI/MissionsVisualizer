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
	{
		AddObject(parent);
	}

	for(size_t i = 0; i < childrenIds.size(); i++)
	{
		long childId = childrenIds[i];

		if(SpaceObject::IsBody(childId))
			AddObject(SpaceBody(childId));
		else
			AddObject(SpaceObject(childId));

		if(recursive)
			LoadChildren(SpaceObject(childId), false);
	}
}

void App::LoadSolarSystem(bool onlyPlanets)
{
	if(onlyPlanets)
	{
		std::vector<long> barycenterIds = SpaceObject::FindChildObjectIds(SpaceObject::SSB.GetSpiceId());

		for(size_t i = 0; i < barycenterIds.size(); i++)
		{
			long planetId = barycenterIds[i] * 100 + 99;

			if(SpaceObject::IsPlanet(planetId))
				AddObject(SpaceBody(planetId));
		}
	}
	else
	{
		LoadChildren(SpaceObject::SSB, true, true);
	}
}

void App::AddObject(const SpaceObject& obj)
{
	//try
	//{
	//	FindObject(obj);
	//}
	//catch(const std::runtime_error&)
	//{
	//	objects.push_back(obj.Clone());
	//}

	if(FindObject(obj) == false)
	{
		objects.push_back(obj.Clone());
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

bool App::FindObject(long id)
{
	for(size_t i = 0; i < objects.size(); i++)
	{
		if(objects[i]->GetSpiceId() == id)
			return true;
	}

	return false;
}

bool App::FindObject(const std::string& name)
{
	for(size_t i = 0; i < objects.size(); i++)
	{
		if(objects[i]->GetName() == name)
			return true;
	}

	return FindObject(SpaceObject(name));
}

bool App::FindObject(const SpaceObject& sample)
{
	return FindObject(sample.GetSpiceId());
}

SpaceObject& App::GetObject(long id)
{
	for(size_t i = 0; i < objects.size(); i++)
	{
		if(objects[i]->GetSpiceId() == id)
			return GetObjectByIndex(i);
	}

	throw std::runtime_error("FindObject haven't found requested object");
}

SpaceObject& App::GetObject(const std::string& name)
{
	for(size_t i = 0; i < objects.size(); i++)
	{
		if(objects[i]->GetName() == name)
			return GetObjectByIndex(i);
	}

	try
	{
		return GetObject(SpaceObject(name));
	}
	catch(const std::runtime_error&)
	{
		throw std::runtime_error("FindObject haven't found requested object");
	}
}

SpaceObject& App::GetObject(const SpaceObject& sample)
{
	return GetObject(sample.GetSpiceId());
}
