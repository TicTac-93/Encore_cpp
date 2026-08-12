# Setup boilerplate from JUCE example GUI App

cmake_minimum_required(VERSION 4.0...4.2)
project(
	Encore
	VERSION 0.0.1)

set(BIN_DIR "${PROJECT_SOURCE_DIR}/bin/")


add_subdirectory(modules/wxWidgets)

		
# Move built application to bin directory
add_custom_command(TARGET Encore 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:Encore> ${BIN_DIR}/Encore.exe)
