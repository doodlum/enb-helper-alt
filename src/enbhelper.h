#pragma once

#include <windows.h>

#define HELPER_VER 2
#define HELPER_API __stdcall
#define HELPER_DLL "enbhelperf4.dll"

struct NiTransform
{
	float	rot[9];
	float	pos_x;
	float	pos_y;
	float	pos_z;
	float	scale;
};

struct HelperFuncs
{
	DWORD version;
	// version = 1
	bool (HELPER_API *getTime)(float& time);
	bool (HELPER_API *getWeatherTransition)(float& t);
	bool (HELPER_API *getCurrentWeatherId)(DWORD& id);
	bool (HELPER_API *getOutgoingWeatherId)(DWORD& id);
	bool (HELPER_API *getPlayerCameraTransformMatrices)(NiTransform &m_local, NiTransform &m_world, NiTransform &m_oldworld);
	// version = 2
	bool (HELPER_API *getCurrentLocationId)(DWORD& id);
	bool (HELPER_API *getWorldSpaceId)(DWORD& id);
	bool (HELPER_API *getSkyMode)(DWORD& mode);
	bool (HELPER_API *getCurrentWeatherClass)(int& c);
	bool (HELPER_API *getOutgoingWeatherClass)(int& c);
};

#ifdef HELPER_COMPILATION

bool HELPER_API helperInit(HelperFuncs& funcs);
bool HELPER_API helperUninit();

#else

// define in your cpp
// bool (HELPER_API *helperInit)(HelperFuncs& funcs) = NULL;
// bool (HELPER_API *helperUninit)() = NULL;
// HelperFuncs g_HelperFuncs;
// HMODULE g_HelperModule = NULL;

// first call helperLoad(), if succeed call helperInit() to check game version and get the funcs

//extern bool (HELPER_API *helperInit)(HelperFuncs& funcs);
//extern bool (HELPER_API *helperUninit)();
extern HelperFuncs	g_HelperFuncs;
extern HMODULE g_HelperModule;

__forceinline bool HELPER_API helperInit(HelperFuncs& funcs)
{
	funcs.version = HELPER_VER;
	funcs.getTime = (bool (HELPER_API *)(float& time))GetProcAddress(g_HelperModule, "GetTime");
	funcs.getWeatherTransition = (bool (HELPER_API *)(float& t))GetProcAddress(g_HelperModule, "GetWeatherTransition");
	funcs.getCurrentWeatherId = (bool (HELPER_API *)(DWORD& id))GetProcAddress(g_HelperModule, "GetCurrentWeather");
	funcs.getOutgoingWeatherId = (bool (HELPER_API *)(DWORD& id))GetProcAddress(g_HelperModule, "GetOutgoingWeather");
	funcs.getPlayerCameraTransformMatrices = (bool (HELPER_API *)(NiTransform &m_local, NiTransform &m_world, NiTransform &m_oldworld))GetProcAddress(g_HelperModule, "GetPlayerCameraTransformMatrices");
	funcs.getCurrentLocationId = (bool (HELPER_API *)(DWORD& id))GetProcAddress(g_HelperModule, "GetCurrentLocationID");
	funcs.getWorldSpaceId = (bool (HELPER_API *)(DWORD& id))GetProcAddress(g_HelperModule, "GetWorldSpaceID");
	funcs.getSkyMode = (bool (HELPER_API *)(DWORD& mode))GetProcAddress(g_HelperModule, "GetSkyMode");
	funcs.getCurrentWeatherClass = (bool (HELPER_API *)(int& c))GetProcAddress(g_HelperModule, "GetCurrentWeatherClassification");
	funcs.getOutgoingWeatherClass = (bool (HELPER_API *)(int& c))GetProcAddress(g_HelperModule, "GetOutgoingWeatherClassification");

	return true;
}

__forceinline bool HELPER_API helperUninit()
{
	return true;
}

bool helperLoad(char *fullpath, char *filename);
void helperUnload();

#endif