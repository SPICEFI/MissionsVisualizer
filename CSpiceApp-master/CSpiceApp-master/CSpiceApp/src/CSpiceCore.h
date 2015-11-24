#pragma once

#ifdef _WIN64
#pragma comment(lib, "lib\\cspice64.lib")
#else
#pragma comment(lib, "lib\\cspice32.lib")
#endif

#pragma comment(linker, "/NODEFAULTLIB:LIBCMT.lib")

#include "..\include\CSpice\SpiceUsr.h"
#include "CSpiceUtil.h"

//class Time;
//class Frame;
//class SpaceObject;
//class SpaceBody;
