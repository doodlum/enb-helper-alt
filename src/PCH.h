#pragma once

#ifdef FALLOUT4
#include "RE/Fallout.h"
#include "F4SE/F4SE.h"
#define SKSE F4SE
#define SKSEAPI F4SEAPI
#define SKSEPlugin_Load F4SEPlugin_Load
#define SKSEPlugin_Query F4SEPlugin_Query
#define RUNTIME_1_5_97 RUNTIME_1_10_163
#else
#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"
#endif

#pragma warning(push)
#ifdef NDEBUG
#	include <spdlog/sinks/basic_file_sink.h>
#else
#	include <spdlog/sinks/msvc_sink.h>
#endif
#pragma warning(pop)

using namespace std::literals;

namespace logger = SKSE::log;

#define DLLEXPORT __declspec(dllexport)

#include "Version.h"
