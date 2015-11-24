#include "Frame.h"

const Frame Frame::J2000 = Frame("J2000");
const Frame Frame::ECLIPJ2000 = Frame("ECLIPJ2000");

Frame::Frame(int spiceId, const std::string& name)
{
	Construct(spiceId, name);
}

Frame::Frame(const std::string& spiceName, const std::string& name)
{
	long spiceId;
	CSPICE_ASSERT( namfrm_c(spiceName.c_str(), &spiceId) );

	Construct(spiceId, name);
}

void Frame::Construct(int spiceId, const std::string& name)
{
	if(!ValidateId(spiceId))
		CSpiceUtil::SignalError("No such CSpice frame is defined");

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

Frame::~Frame()
{
	
}

long Frame::GetSpiceId() const
{
	return spiceId;
}

std::string Frame::GetSpiceName() const
{
	char frameName[FRAME_NAME_MAX_LENGTH];
	CSPICE_ASSERT( frmnam_c(spiceId, FRAME_NAME_MAX_LENGTH, frameName) );

	return std::string(frameName);
}

const std::string& Frame::GetName() const
{
	return name;
}

Frame::FrameInfo Frame::GetFrameInfo() const
{
	SpiceInt centerId;
	SpiceInt clssid;
	SpiceInt frclss;
	SpiceBoolean found;

	CSPICE_ASSERT(frinfo_c(spiceId, &centerId, &frclss, &clssid, &found));

	FrameInfo finfo;
	finfo.centerId = centerId;
	finfo.classId = clssid;
	finfo.frameType = FrameType(frclss);

	return finfo;
}

Vector3 Frame::TransformVector(const Vector3& vec, const Date& t, const Frame& ref) const
{
	double transform[3][3];

	CSPICE_ASSERT(pxform_c(GetSpiceName().c_str(), ref.GetSpiceName().c_str(), t.AsDouble(), transform));

	double axisLocal[3] = {vec.x, vec.y, vec.z};
	double axisGlobal[3];

	CSPICE_ASSERT(mxv_c(transform, axisLocal, axisGlobal));

	return Vector3(axisGlobal);
}

Vector3 Frame::AxisX(const Date& t, const Frame& ref) const
{
	return TransformVector(Vector3::i, t, ref);
}

Vector3 Frame::AxisY(const Date& t, const Frame& ref) const
{
	return TransformVector(Vector3::j, t, ref);
}

Vector3 Frame::AxisZ(const Date& t, const Frame& ref) const
{
	return TransformVector(Vector3::k, t, ref);
}

Orientation Frame::GetOrientation(const Date& t, const Frame& ref) const
{
	return Orientation(AxisX(t, ref), AxisY(t, ref), AxisZ(t, ref));
}

//Matrix3x3 Frame::GetTransformationMatrix(const Date& t, const Frame& ref) const
//{
//	double transform[3][3];
//	CSPICE_ASSERT(pxform_c(GetSpiceName().c_str(), ref.GetSpiceName().c_str(), t.AsDouble(), transform));
//
//	float fMatrix[3][3];
//
//	for(int i = 0; i < 3; i++)
//	{
//		for(int j = 0; j < 3; j++)
//		{
//			fMatrix[i][j] = static_cast<float>(transform[i][j]);
//		}
//	}
//
//	return Matrix3x3(fMatrix);
//}

Matrix4x4 Frame::GetTransformationMatrix(const Date& t, const Frame& ref) const
{
	Matrix4x4 transform = Matrix4x4::Zero;

	Vector3 x = AxisX(t, ref);
	Vector3 y = AxisY(t, ref);
	Vector3 z = AxisZ(t, ref);

	for(int row = 0; row < 3; row++)
	{
		transform.Set(row, 0, x[row]);
		transform.Set(row, 1, y[row]);
		transform.Set(row, 2, z[row]);
	}

	transform.Set(3, 3, 1.0);

	return transform;
}

bool Frame::HasAvailableData() const
{
	long centerId = GetFrameInfo().centerId;

	bool hasPoleRa = (bodfnd_c(centerId, "POLE_RA") != SPICEFALSE);
	bool hasPoleDec = (bodfnd_c(centerId, "POLE_DEC") != SPICEFALSE);
	bool hasPm = (bodfnd_c(centerId, "PM") != SPICEFALSE);
	bool centerHasIAUFrameData = hasPoleRa && hasPoleDec && hasPm;

	bool isIAUFrame = GetSpiceName().substr(0, 3) == "IAU";

	return (isIAUFrame && centerHasIAUFrameData) || HasLimitedCoverage();
}

bool Frame::HasLimitedCoverage() const
{
	const std::vector<long>& pckIds = GetLoadedPckIds();

	std::vector<long>::const_iterator it = std::find(pckIds.begin(), pckIds.end(), this->spiceId);

	return it != pckIds.end();
}

Window Frame::GetCoverage() const
{
	std::vector<KernelData> kernels = CSpiceUtil::GetLoadedKernels("PCK");

	Window coverage;

	for(size_t i = 0; i < kernels.size(); i++)
	{
		std::string file = kernels[i].filename;

		CSPICE_ASSERT(pckcov_c(file.c_str(), this->GetFrameInfo().classId, &coverage.GetSpiceCell()));
	}

	return coverage;
}

bool Frame::ValidateId(long id)
{
	SpiceInt centerId;
	SpiceInt clssid;
	SpiceInt frclss;
	SpiceBoolean found;

	CSPICE_ASSERT(frinfo_c(id, &centerId, &frclss, &clssid, &found));

	return found != SPICEFALSE;
}

long Frame::MakeFrameId(FrameType type, long classId)
{
	long frameId;
	char frameName[FRAME_NAME_MAX_LENGTH];
	long centerId;
	SpiceBoolean found;
	CSPICE_ASSERT(ccifrm_c(type, classId, FRAME_NAME_MAX_LENGTH, &frameId, frameName, &centerId, &found));

	return (found != SPICEFALSE) ? frameId : 0;
}

std::vector<long> Frame::GetBuiltInIds()
{
	SPICEINT_CELL(cell, CELL_SIZE_LARGE);
	CSPICE_ASSERT(bltfrm_c(SPICE_FRMTYP_ALL, &cell));
	return CSpiceUtil::IntCellToVector(cell);
}

std::vector<long> Frame::GetPoolIds()
{
	SPICEINT_CELL(cell, CELL_SIZE_LARGE);
	CSPICE_ASSERT(kplfrm_c(SPICE_FRMTYP_ALL, &cell));
	return CSpiceUtil::IntCellToVector(cell);
}

std::vector<long> Frame::GetLoadedPckIds()
{
	SPICEINT_CELL(cell, CELL_SIZE_LARGE);
	std::vector<KernelData> kernels = CSpiceUtil::GetLoadedKernels("PCK");
	for(size_t i = 0; i < kernels.size(); i++)
	{
		CSPICE_ASSERT(pckfrm_c(kernels[i].filename.c_str(), &cell));
	}

	std::vector<long> classIds = CSpiceUtil::IntCellToVector(cell);
	std::vector<long> frameIds;
	frameIds.reserve(classIds.size());

	for(size_t i = 0; i < classIds.size(); i++)
	{
		frameIds.push_back(Frame::MakeFrameId(FT_PCK, classIds[i]));
	}

	return frameIds;
	
}
