cmake_minimum_required(VERSION 4.0...4.2)
project(
	Encore
	VERSION 0.0.1
	LANGUAGES C CXX)

message(STATUS "Using ${CMAKE_GENERATOR}")
message(STATUS "Build Type ${CMAKE_BUILD_TYPE}")

# =============================================================================
# Packages

# wxWidgets Dynamic Linking

# NOTE: Update this path to reflect your local installation of wxWidgets
# LIST(APPEND CMAKE_PREFIX_PATH "C:/dev/packages/installed/wxWidgets")

# find_package(wxWidgets CONFIG REQUIRED
# 	COMPONENTS
# 		core base)

# if(wxWidgets_USE_FILE) # not defined in CONFIG mode
#     include(${wxWidgets_USE_FILE})
# endif()


# wxWidgets Static Linking

if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
	message(STATUS "Optimizing for Release - Configuring wxWidgets")
	set(CMAKE_BUILD_TYPE "Release")
	set(wxBUILD_DEBUG_LEVEL "0")
else()
	message(STATUS "Debug Mode - Skipping Optimization")
endif()

set(wxBUILD_SHARED OFF)
add_subdirectory(modules/wxWidgets-3.2 EXCLUDE_FROM_ALL)


# =============================================================================
# Project Variables
# set(BIN_DIR "${PROJECT_SOURCE_DIR}/bin/")


# =============================================================================
# Encore
add_executable(Encore
	src/Main.cpp)

# # Link wxWidgets
target_link_libraries(Encore
	wx::core
	wx::base)

if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
	message(STATUS "Optimizing for Release - Setting compile options...")

	target_compile_definitions(Encore
		PUBLIC
			wxDEBUG_LEVEL=0)

	# NOTE: These are configured for gcc, you may need to update them to suit your environment
	target_compile_options(Encore
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

install(
	TARGETS
		Encore)

# install(
# 	FILES
# 		$<TARGET_RUNTIME_DLLS:Encore> TYPE BIN)
