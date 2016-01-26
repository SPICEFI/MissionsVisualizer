#include "CosmoPoint.h"


CosmoPoint::CosmoPoint()
{
	position = 0;
	velocity = 0;
	name = "";
	description = "";
}


CosmoPoint::~CosmoPoint()
{
}

void CosmoPoint::Initialize(const Vector3& position, const Vector3& velocity, const std::string name, std::string description)
{
	this->position = position;
	this->velocity = velocity;
	this->name = name;
	this->description = description;
}