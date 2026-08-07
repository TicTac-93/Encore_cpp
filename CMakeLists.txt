cmake_minimum_required(VERSION 4.0)

project(
	Encore
	VERSION 0.1
	LANGUAGES C CXX
)

# Build Vars
set(BIN_DIR "${PROJECT_SOURCE_DIR}/bin")

# Add Raylib
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games

FetchContent_Declare(
    raylib
    GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
    GIT_TAG "5.5"
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(raylib)

# Main Application
add_executable(Encore src/main.cpp)

# Includes and Dependencies
target_include_directories(Encore
	PRIVATE
		src/clay
)

target_link_libraries(Encore
	PRIVATE
		raylib)

# Properties
set_target_properties(Encore
	PROPERTIES
		CXX_STANDARD 20)

# Compile flags
# target_compile_options(Encore
# 	PRIVATE
# 		"-Wno-narrowing")

# Copy resources to build/resources
add_custom_command(TARGET Encore POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${PROJECT_SOURCE_DIR}/src/resources
	${CMAKE_BINARY_DIR}/resources
	COMMENT "Copied resources/ to ${CMAKE_BINARY_DIR}"
)

# Copy build to bin
add_custom_command(TARGET Encore POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:Encore> ${BIN_DIR}/Encore.exe
	COMMENT "Created ${PROJECT_SOURCE_DIR}/bin/Encore.exe"
)

# Copy resources to bin
add_custom_command(TARGET Encore POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${PROJECT_SOURCE_DIR}/src/resources
	${BIN_DIR}/resources
	COMMENT "Copied resources/ to ${BIN_DIR}"
)