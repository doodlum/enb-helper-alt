#pragma once

#include <windows.h>

#ifdef FALLOUT4
#define HELPER_DLL "enbhelperf4.dll"
#else
#define HELPER_DLL "enbhelperse.dll"
#endif

// struct ENBAPI::NiTransform
// {
// 	float rot[9];
// 	float pos_x;
// 	float pos_y;
// 	float pos_z;
// 	float scale;
// };

extern bool bLoaded;

extern "C" DLLEXPORT bool IsLoaded();

extern "C" DLLEXPORT bool GetTime(float& time);

extern "C" DLLEXPORT bool GetWeatherTransition(float& t);

extern "C" DLLEXPORT bool GetCurrentWeather(DWORD& id);

extern "C" DLLEXPORT bool GetOutgoingWeather(DWORD& id);

extern "C" DLLEXPORT bool GetPlayerCameraTransformMatrices(RE::NiTransform& m_local, RE::NiTransform& m_world, RE::NiTransform& m_oldworld);

extern "C" DLLEXPORT bool GetCurrentLocationID(DWORD& id);

extern "C" DLLEXPORT bool GetWorldSpaceID(DWORD& id);

extern "C" DLLEXPORT bool GetSkyMode(DWORD& mode);

extern "C" DLLEXPORT bool GetCurrentWeatherClassification(int& c);

extern "C" DLLEXPORT bool GetOutgoingWeatherClassification(int& c);
