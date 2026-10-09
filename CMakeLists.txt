cmake_minimum_required(VERSION 4.0...4.2)
project(
	Encore
	VERSION 0.0.1
	LANGUAGES C CXX)

message(STATUS "Using ${CMAKE_GENERATOR}")
message(STATUS "Build Type ${CMAKE_BUILD_TYPE}")

include(FetchContent)

# NOTE:
# This CMakeLists was predominantly configured for MinGW64, for building on Windows.
# It has also been tested with MSVC, but may have some issues.
# Other build environments are left as an exercise to the reader :)

# =============================================================================
# Packages

FetchContent_Declare(
	OpenEXR
	GIT_REPOSITORY https://github.com/AcademySoftwareFoundation/openexr.git
	GIT_TAG        69b2604fc76e370615438bdc8d2cd95b9349c12e # release 3.5.2
)

FetchContent_Declare(
	wxWidgets
	GIT_REPOSITORY https://github.com/wxWidgets/wxWidgets.git
	GIT_TAG        aa1a25e7b511c0a4e341a3c722e6eac2aa159ab6 # release 3.2.10
)

if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
	message(STATUS "Optimizing for Release - Configuring wxWidgets")
	set(CMAKE_BUILD_TYPE "Release")
	set(wxBUILD_DEBUG_LEVEL "0")
else()
	message(STATUS "Debug Mode - Skipping Optimization")
endif()

set(wxBUILD_SHARED OFF)

FetchContent_MakeAvailable(OpenEXR wxWidgets)


# =============================================================================
# Encore

# Files
set(ENCORE_FILES
	src/Main.cpp
	src/internal/SequenceImport.cpp)

if(WIN32)
    # Include a RC file for windows
    list(APPEND ENCORE_FILES src/encore.rc)
endif()

add_executable(${PROJECT_NAME} WIN32 ${ENCORE_FILES})

set_property(TARGET Encore PROPERTY CXX_STANDARD 20)

# Link wxWidgets
target_link_libraries(${PROJECT_NAME}
	wx::core
	wx::base
	OpenEXR::OpenEXR)


# =============================================================================
# Optimizations for Release builds
if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
	message(STATUS "Optimizing for Release - Setting compile options...")

	target_compile_definitions(${PROJECT_NAME}
		PUBLIC
			wxDEBUG_LEVEL=0)

	# NOTE: These are configured for gcc, you may need to update them to suit your build environment
	target_compile_options(${PROJECT_NAME}
		PUBLIC
			-Os
			-s
			-ffunction-sections
			-fdata-sections)

	target_link_options(Encore
		PUBLIC
			-Wl,--gc-sections)
else()
	message(STATUS "Debug Mode - Skipping Optimization")
endif()


# =============================================================================
# Resources, Manifest + bundling

# Resource manifest should be included automatically with MinGW
# This should take care of it for MSVC
if(MSVC)
    # Use manifest from wx.rc
    set_target_properties(${PROJECT_NAME} PROPERTIES
        COMPILE_FLAGS "/DwxUSE_RC_MANIFEST"
        LINK_FLAGS "/MANIFEST:NO"
    )
endif()

# LIST(APPEND ENCORE_RESOURCES
# 	resources/exit.png)

# set_target_properties(Encore PROPERTIES
# 	RESOURCE "${ENCORE_RESOURCES}")


# install(TARGETS Encore
# 	RESOURCE DESTINATION
# 	${CMAKE_INSTALL_PREFIX}/bin/resources)

