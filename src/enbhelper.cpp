#include "enbhelper.h"

bool bLoaded = false;

#ifdef FALLOUT4
#include "RE/Sky.h"
#endif

extern "C" DLLEXPORT bool GetTime(float& time)
{
	const auto sky = RE::Sky::GetSingleton();

	if (sky) {
		time = sky->currentGameHour;
		return true;
	}

	return false;
}

extern "C" DLLEXPORT bool GetWeatherTransition(float& t)
{
	const auto sky = RE::Sky::GetSingleton();

	if (sky) {
		t = sky->currentWeatherPct;
		return true;
	}

	return false;
}

#ifdef LIGHTING_TEMPLATE
bool inInterior(RE::PlayerCharacter* player)
{
	return (player && player->parentCell && player->parentCell->IsInteriorCell());
}

extern "C" DLLEXPORT bool GetCurrentWeather(DWORD& id)
{
	const auto sky = RE::Sky::GetSingleton();
	const auto player = RE::PlayerCharacter::GetSingleton();

	if (inInterior(player) && player->parentCell->lightingTemplate) {
		id = player->parentCell->lightingTemplate->GetFormID();
		return true;
	} else if (sky && sky->currentWeather) {
		id = sky->currentWeather->GetFormID();
		return true;
	}

	return false;
}

bool GetOutgoingWeather(DWORD& id)
{
	const auto sky = RE::Sky::GetSingleton();
	const auto player = RE::PlayerCharacter::GetSingleton();

	if (inInterior(player) && player->parentCell->lightingTemplate) {
		id = player->parentCell->lightingTemplate->GetFormID();
		return true;
	} else if (sky && sky->lastWeather) {
		id = sky->lastWeather->GetFormID();
		return true;
	}

	return false;
}
#else
extern "C" DLLEXPORT bool GetCurrentWeather(DWORD& id)
{
	const auto sky = RE::Sky::GetSingleton();
	
	if (sky && sky->currentWeather) {
		id = sky->currentWeather->GetFormID();
		return true;
	}

	return false;
}

extern "C" DLLEXPORT bool GetOutgoingWeather(DWORD& id)
{
	const auto sky = RE::Sky::GetSingleton();
	
	if (sky && sky->lastWeather) {
		id = sky->lastWeather->GetFormID();
		return true;
	}

	return false;
}
#endif

extern "C" DLLEXPORT bool GetCurrentLocationID(DWORD& id)
{
	const auto player = RE::PlayerCharacter::GetSingleton();

#ifndef SKYRIMVR
	if (player && player->currentLocation) {
		id = player->currentLocation->GetFormID();
		return true;
	}
#else
	if (player) {
		const auto currentLocation = player->GetCurrentLocation();
		if (currentLocation) {
			id = currentLocation->GetFormID();
			return true;
		}
	}
#endif
	return false;
}

extern "C" DLLEXPORT bool GetWorldSpaceID(DWORD& id)
{
	const auto player = RE::PlayerCharacter::GetSingleton();

	#ifdef FALLOUT4
	if (player && player->cachedWorldspace) {
		id = player->cachedWorldspace->GetFormID();
		return true;
	}
	#else
	if (player && player->GetWorldspace()) {
		id = player->GetWorldspace()->GetFormID();
		return true;
	}
	#endif

	return false;
}

extern "C" DLLEXPORT bool GetSkyMode(DWORD& mode)
{
	const auto sky = RE::Sky::GetSingleton();

	if (sky) {
		mode = sky->mode.underlying();
		return true;
	}

	return false;
}

// Credits aers
// Papyrus Weather.GetClassification
int32_t GetClassification(RE::TESWeather* weather)
{
	#ifdef FALLOUT4
	const auto flags = weather->weatherData[11];

	if ((flags & 1) != 0)
		return 0;
	if ((flags & 2) != 0)
		return 1;
	if ((flags & 4) != 0)
		return 2;
	if ((flags & 8) != 0)
		return 3;
	#else
	typedef RE::TESWeather::WeatherDataFlag Flags;
	const auto flags = weather->data.flags;

	if ((flags & Flags::kPleasant) != Flags::kNone)
		return 0;
	if ((flags & Flags::kCloudy) != Flags::kNone)
		return 1;
	if ((flags & Flags::kRainy) != Flags::kNone)
		return 2;
	if ((flags & Flags::kSnow) != Flags::kNone)
		return 3;
	#endif

	return 0xFFFFFFFF;
}

extern "C" DLLEXPORT bool GetCurrentWeatherClassification(int& c)
{
	const auto sky = RE::Sky::GetSingleton();
	if (sky && sky->currentWeather) {
		c = GetClassification(sky->currentWeather);
		return true;
	}

	return false;
}

extern "C" DLLEXPORT bool GetOutgoingWeatherClassification(int& c)
{
	const auto sky = RE::Sky::GetSingleton();
	if (sky && sky->currentWeather) {
		c = GetClassification(sky->currentWeather);
		return true;
	}

	return false;
}

// Credits aers
extern "C" DLLEXPORT bool GetPlayerCameraTransformMatrices(RE::NiTransform& m_local, RE::NiTransform& m_world, RE::NiTransform& m_oldworld)
{
	const auto playerCamera = RE::PlayerCamera::GetSingleton();
	if (playerCamera && playerCamera->cameraRoot) {
		const auto cameraNode = playerCamera->cameraRoot;

		memcpy(&m_local, &(cameraNode->local), sizeof(RE::NiTransform));
		memcpy(&m_world, &(cameraNode->world), sizeof(RE::NiTransform));
		memcpy(&m_oldworld, &(cameraNode->previousWorld), sizeof(RE::NiTransform));

		return true;
	}
	return false;
}

extern "C" DLLEXPORT bool IsLoaded() {
	if (bLoaded)
		logger::info("ENB Helper Loaded"sv);
	return bLoaded;
}
