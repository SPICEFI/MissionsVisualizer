#pragma once

#include "CSpiceCore.h"

#include <sstream>
#include <string>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <vector>

#define CSPICE_ASSERT(expression)																																					\
	if(true)																																										\
	{																																												\
		if(failed_c())																																								\
		{																																											\
			std::stringstream errorStr;																																				\
			errorStr << "CSpice error in " << __FILE__ << " (line " << __LINE__ << "): Error flag was set prior to function call: " << CSpiceUtil::GetShortErrorMessage();			\
			CSpiceUtil::SignalError(errorStr.str());																																\
		}																																											\
		expression;																																									\
		if(failed_c())																																								\
		{																																											\
			std::stringstream errorStr;																																				\
			errorStr << "CSpice error in " << __FILE__ << " (line " << __LINE__ << "): " << CSpiceUtil::GetShortErrorMessage();														\
			CSpiceUtil::SignalError(errorStr.str());																																\
		}																																											\
	}																																												\
	else																																											\
		(void)0

#define KERNEL_FILENAME_LENGTH 1024
#define KERNEL_TYPE_LENGTH 32
#define KERNEL_SOURCE_LENGTH 1024

#define CELL_SIZE_SMALL 128
#define CELL_SIZE_DEFAULT 512
#define CELL_SIZE_LARGE 2048

struct KernelData
{
	std::string filename;
	std::string type;
	std::string source;
};

class CSpiceUtil
{
public:
	static void SetErrorHandlingParams(const std::string& action, const std::string& device);
	static void SetLoggingFile(const std::string& file);

	static void LoadKernel(const std::string& path);
	static std::vector<KernelData> GetLoadedKernels(const std::string& type = "ALL");

	static std::string GetShortErrorMessage();
	static std::string GetExplainErrorMessage();
	static std::string GetLongErrorMessage();
	static std::string GetTraceback();

	static void SignalError(const std::string& errorMsg);
	static void LogError(const std::string& extraMsg);

	static void ResetErrorFlag();

	static std::vector<long> IntCellToVector(SpiceCell cell);
	static std::vector<double> DoubleCellToVector(SpiceCell cell);
	//static std::vector<std::string> CharCellToVector(SpiceCell cell);

private:
	static std::string logFile;
};
