#pragma once

#include "CSpiceCore.h"
#include "Vector3.h"
#include "Date.h"
#include "Orientation.h"
#include "Window.h"
#include "Matrix3x3.h"

#define FRAME_NAME_MAX_LENGTH 64

class Frame
{
public:
	enum FrameType
	{
		FT_INERTIAL = 1,
		FT_PCK,
		FT_CK,
		FT_TK,
		FT_DYNAMIC
	};

	struct FrameInfo
	{
		long centerId;
		FrameType frameType;
		long classId;
	};

public:
	Frame(int spiceId, const std::string& name = "");
	Frame(const std::string& spiceName, const std::string& name = "");
	~Frame();

	long GetSpiceId() const;
	std::string GetSpiceName() const;
	const std::string& GetName() const;

	FrameInfo GetFrameInfo() const;

	Vector3 TransformVector(const Vector3& vec, const Date& t, const Frame& ref) const;
	Vector3 AxisX(const Date& t, const Frame& ref) const;
	Vector3 AxisY(const Date& t, const Frame& ref) const;
	Vector3 AxisZ(const Date& t, const Frame& ref) const;

	Orientation GetOrientation(const Date& t, const Frame& ref) const;
	Matrix3x3 GetTransformationMatrix(const Date& t, const Frame& ref) const;

	bool HasAvailableData() const;
	bool HasLimitedCoverage() const;
	Window GetCoverage() const;

	static bool ValidateId(long id);
	static long MakeFrameId(FrameType type, long classId);

	static std::vector<long> GetBuiltInIds();
	static std::vector<long> GetPoolIds();
	static std::vector<long> GetLoadedPckIds();

private:
	void Construct(int spiceId, const std::string& name);

private:
	long spiceId;
	std::string name;

public:
	static const Frame J2000;
	static const Frame ECLIPJ2000;
};
