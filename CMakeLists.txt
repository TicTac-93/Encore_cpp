cmake_minimum_required(VERSION 4.0...4.2)
project(
	Encore
	VERSION 0.0.1
	LANGUAGES C CXX)

message(STATUS "Using ${CMAKE_GENERATOR}")
message(STATUS "Build Type ${CMAKE_BUILD_TYPE}")

# =============================================================================
# Packages

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
# Encore

# Files
LIST(APPEND ENCORE_FILES
	src/Main.cpp
	src/Panels.cpp
	src/Menus.cpp)

LIST(APPEND ENCORE_RESOURCES
	resources/exit.png)

add_executable(Encore
	${ENCORE_FILES})

set_target_properties(Encore PROPERTIES
	RESOURCE "${ENCORE_RESOURCES}")

# Link wxWidgets
target_link_libraries(Encore
	wx::core
	wx::base)

# Optimizations for Release builds
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

install(TARGETS Encore
	RESOURCE DESTINATION
	${CMAKE_INSTALL_PREFIX}/bin/resources)

