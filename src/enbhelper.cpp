#include "enbhelper.h"


bool inInterior(RE::PlayerCharacter* player)
{
	return (player && player->parentCell && player->parentCell->IsInteriorCell());
}

bool isLoaded = false;

bool IsLoaded()
{
	return isLoaded;
}

bool GetTime(float& time)
{
	const auto sky = RE::Sky::GetSingleton();

	if (sky) {
		time = sky->currentGameHour;
		return true;
	}

	return false;
}

bool GetWeatherTransition(float& t)
{
	const auto sky = RE::Sky::GetSingleton();

	if (sky) {
		t = sky->currentWeatherPct;
		return true;
	}

	return false;
}

bool GetCurrentWeather(DWORD& id)
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

bool GetCurrentLocationID(DWORD& id)
{
	const auto player = RE::PlayerCharacter::GetSingleton();

	if (player && player->currentLocation) {
		id = player->currentLocation->GetFormID();
		return true;
	}

	return false;
}

bool GetWorldSpaceID(DWORD& id)
{
	const auto player = RE::PlayerCharacter::GetSingleton();

	if (player && player->GetWorldspace()) {
		id = player->GetWorldspace()->GetFormID();
		return true;
	}

	return false;
}

bool GetSkyMode(DWORD& mode)
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

	return 0xFFFFFFFF;
}

bool GetCurrentWeatherClassification(int& c)
{
	const auto sky = RE::Sky::GetSingleton();
	if (sky && sky->currentWeather) {
		c = GetClassification(sky->currentWeather);
		return true;
	}

	return false;
}

bool GetOutgoingWeatherClassification(int& c)
{
	const auto sky = RE::Sky::GetSingleton();
	if (sky && sky->currentWeather) {
		c = GetClassification(sky->currentWeather);
		return true;
	}

	return false;
}

// Credits aers
bool GetPlayerCameraTransformMatrices(NiTransform& m_local, NiTransform& m_world, NiTransform& m_oldworld)
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
