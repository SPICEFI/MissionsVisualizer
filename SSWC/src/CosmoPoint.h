#pragma once
#include "Math\Vector3.h"
#include <string.h>
class CosmoPoint
{
public:
	Vector3 position;
	Vector3 velocity;
	std::string name;
	std::string description;

	CosmoPoint();
	~CosmoPoint();

	void Initialize(const Vector3& position = 0, const Vector3& velocity = 0, const std::string name = "", std::string description = "");

	CosmoPoint& operator=(const CosmoPoint& rPoint);
};

