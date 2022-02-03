cmake_minimum_required(VERSION 3.20)
set(NAME "ENBHelper")
set(VERSION 1.0.0)

# ---- Options ----

option(COPY_BUILD "Copy the build output to the game directory." FALSE)

option(BUILD_DEBUG "Enable debugging options" OFF)

option(BUILD_SKYRIMAE "Build for Skyrim AE" OFF)
option(BUILD_SKYRIMVR "Build for Skyrim VR" OFF)

option(BUILD_SKYRIMVR "Build for Fallout 4" OFF)


# ---- Cache build vars ----

macro(set_from_environment VARIABLE)
	if (NOT DEFINED ${VARIABLE} AND DEFINED ENV{${VARIABLE}})
		set(${VARIABLE} $ENV{${VARIABLE}})
	endif ()
endmacro()

if(BUILD_SKYRIMVR)
	add_compile_definitions(SKYRIMVR)
	add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
	set(CommonLibName "CommonLibVR")
	set_from_environment(SkyrimVRPath)
	set(GamePath ${SkyrimVRPath})
	set(GameVersion "Skyrim VR")
elseif(BUILD_SKYRIMAE)
	add_compile_definitions(SKYRIMAE)
	set(CommonLibName "CommonLibSSE")
	set_from_environment(Skyrim64Path)
	set(GamePath ${Skyrim64Path})
	set(GameVersion "Skyrim AE")
elseif(BUILD_FALLOUT4)
	add_compile_definitions(FALLOUT4)
	set(CommonLibName "CommonLibF4")
	set_from_environment(Fallout4Path)
	set(GamePath ${Fallout4Path})
	set(GameVersion "Fallout 4")
else()
	add_compile_definitions(SKYRIMSE)
	set(CommonLibName "CommonLibSSE")
	set_from_environment(Skyrim64Path)
	set(GamePath ${Skyrim64Path})
	set(GameVersion "Skyrim SSE")
endif()

if(NOT BUILD_DEBUG)
	add_compile_definitions(NDEBUG)
endif()

set(Boost_USE_STATIC_RUNTIME OFF CACHE BOOL "")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE STRING "")


# ---- Project ----

project(
	${NAME}
	VERSION ${VERSION}
	LANGUAGES CXX
)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/Version.h.in
	${CMAKE_CURRENT_BINARY_DIR}/include/Version.h
	@ONLY
)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.rc.in
	${CMAKE_CURRENT_BINARY_DIR}/version.rc
	@ONLY
)

# ---- Include guards ----

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
	message(
		FATAL_ERROR
			"In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
	)
endif()


# ---- Globals ----

if (MSVC)
	add_compile_definitions(
		_UNICODE
	)

	if (NOT ${CMAKE_GENERATOR} STREQUAL "Ninja")
		add_compile_options(
			/MP	# Build with Multiple Processes
		)
	endif ()
endif ()

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG OFF)

set(Boost_USE_STATIC_LIBS ON)

# ---- Dependencies ----

if(BUILD_SKYRIMVR)
	add_subdirectory("external/CommonLibVR" CommonLibVR)
elseif(BUILD_SKYRIMAE)
	add_subdirectory("external/CommonLibAE" CommonLibSSE)
elseif(BUILD_FALLOUT4)
	add_subdirectory("external/CommonLibF4/CommonLibF4" CommonLibF4)
else()
	add_subdirectory("external/CommonLibSSE" CommonLibSSE)
endif()

find_package(mmio REQUIRED CONFIG)
find_package(spdlog CONFIG REQUIRED)

# ---- Add source files ----

include(cmake/headerlist.cmake)
include(cmake/sourcelist.cmake)

source_group(
	TREE
		${CMAKE_CURRENT_SOURCE_DIR}
	FILES
		${headers}
		${sources}
)

source_group(
	TREE
		${CMAKE_CURRENT_BINARY_DIR}
	FILES
		${CMAKE_CURRENT_BINARY_DIR}/include/Version.h
)

# ---- Create DLL ----

add_library(
	${PROJECT_NAME}
	SHARED
	${headers}
	${sources}
	${CMAKE_CURRENT_BINARY_DIR}/include/Version.h
	${CMAKE_CURRENT_BINARY_DIR}/version.rc
	.clang-format
	.editorconfig
)

target_compile_features(
	${PROJECT_NAME}
	PRIVATE
		cxx_std_23
)

target_include_directories(
	${PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}/include
		${CMAKE_CURRENT_SOURCE_DIR}/src/C++
		${CommonLibPath}/x64/Release
)

target_link_libraries(
	${PROJECT_NAME}
	PRIVATE
		${CommonLibName}::${CommonLibName}
		spdlog::spdlog
)

if (MSVC)
	target_compile_options(
		${PROJECT_NAME}
		PRIVATE
			/sdl	# Enable Additional Security Checks
			/utf-8	# Set Source and Executable character sets to UTF-8
			/Zi	# Debug Information Format

			/permissive-	# Standards conformance

			/Zc:alignedNew	# C++17 over-aligned allocation
			/Zc:auto	# Deduce Variable Type
			/Zc:char8_t
			/Zc:__cplusplus	# Enable updated __cplusplus macro
			/Zc:externC
			/Zc:externConstexpr	# Enable extern constexpr variables
			/Zc:forScope	# Force Conformance in for Loop Scope
			/Zc:hiddenFriend
			/Zc:implicitNoexcept	# Implicit Exception Specifiers
			/Zc:lambda
			/Zc:noexceptTypes	# C++17 noexcept rules
			/Zc:preprocessor	# Enable preprocessor conformance mode
			/Zc:referenceBinding	# Enforce reference binding rules
			/Zc:rvalueCast	# Enforce type conversion rules
			/Zc:sizedDealloc	# Enable Global Sized Deallocation Functions
			/Zc:strictStrings	# Disable string literal type conversion
			/Zc:ternary	# Enforce conditional operator rules
			/Zc:threadSafeInit	# Thread-safe Local Static Initialization
			/Zc:tlsGuards
			/Zc:trigraphs	# Trigraphs Substitution
			/Zc:wchar_t	# wchar_t Is Native Type

			/external:anglebrackets
			/external:W0

			/W4	# Warning level
			/WX # Warning level (warnings are errors)

			"$<$<CONFIG:DEBUG>:/bigobj>"
			"$<$<CONFIG:RELEASE>:/Zc:inline;/JMC-;/Ob3>"
	)

	target_link_options(
		${PROJECT_NAME}
		PRIVATE
			/WX	# Treat Linker Warnings as Errors

			"$<$<CONFIG:DEBUG>:/INCREMENTAL;/OPT:NOREF;/OPT:NOICF>"
			"$<$<CONFIG:RELEASE>:/INCREMENTAL:NO;/OPT:REF;/OPT:ICF;/DEBUG:FULL>"
	)

	# fix a warning with /Ob2 being overridden with /Ob3
	string(REPLACE "/Ob2" "/Ob3" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

endif ()

target_precompile_headers(
	${PROJECT_NAME}
	PRIVATE
		src/PCH.h
)

# ---- Post build ----

if (COPY_BUILD)
	if (DEFINED GamePath)
		add_custom_command(
			TARGET ${PROJECT_NAME}
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${GamePath}/Data/SKSE/Plugins/
			COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_PDB_FILE:${PROJECT_NAME}> ${GamePath}/Data/SKSE/Plugins/
		)
	else ()
		message(
			WARNING
			"Variable ${GamePath} is not defined. Skipping post-build copy command."
		)
	endif ()
endif ()
