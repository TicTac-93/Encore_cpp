# Setup boilerplate from JUCE example GUI App

cmake_minimum_required(VERSION 3.22...4.2)
project(
	GUI_APP_EXAMPLE
	VERSION 0.0.1)

set(BIN_DIR "${PROJECT_SOURCE_DIR}/build/bin/")


# Add JUCE to the build, in this case as a submodule

# find_package(JUCE CONFIG REQUIRED)        # If you've installed JUCE to your system
# or
add_subdirectory(modules/JUCE)              # If you've put JUCE in a subdirectory called JUCE


# `juce_add_gui_app` adds an executable target with the name passed as the first argument.
# This target is a normal CMake target, but has a lot of extra properties set
# up by default. This function accepts many optional arguments. Check the readme at
# `docs/CMake API.md` in the JUCE repo for the full list.

juce_add_gui_app(Encore
    # VERSION ...                       # Set this if the app version is different to the project version
    # ICON_BIG ...                      # ICON_* arguments specify a path to an image file to use as an icon
    # ICON_SMALL ...
    # DOCUMENT_EXTENSIONS ...           # Specify file extensions that should be associated with this app
    # COMPANY_NAME ...                  # Specify the name of the app's author
    PRODUCT_NAME "Gui App Example"      # The name of the final executable, which can differ from the target name
    NEEDS_WEB_BROWSER FALSE             # Set to TRUE if you need to use webviews
    NEEDS_CURL FALSE)                   # Set to TRUE if you need to use curl


# Assign files to compile for the build
# (dependencies should automatically be collected and added)
target_sources(Encore
    PRIVATE
        src/Main.cpp
        src/MainComponent.cpp)

# Add src/ as an include directory, for more robust #includes
target_include_directories(Encore
	PRIVATE
		${PROJECT_SOURCE_DIR}/src)


# `target_compile_definitions` adds some preprocessor definitions to our target. In a Projucer
# project, these might be passed in the 'Preprocessor Definitions' field. JUCE modules also make use
# of compile definitions to switch certain features on/off, so if there's a particular feature you
# need that's not on by default, check the module header for the correct flag to set here. These
# definitions will be visible both to your code, and also the JUCE module code, so for new
# definitions, pick unique names that are unlikely to collide! This is a standard CMake command.

target_compile_definitions(Encore
    PRIVATE
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_gui_app` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_gui_app` call
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:Encore,JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:Encore,JUCE_VERSION>")


# `target_link_libraries` links libraries and JUCE modules to other libraries or executables. Here,
# we're linking our executable target to the `juce::juce_gui_extra` module. Inter-module
# dependencies are resolved automatically, so `juce_core`, `juce_events` and so on will also be
# linked automatically. If we'd generated a binary data target above, we would need to link to it
# here too. This is a standard CMake command.

target_link_libraries(Encore
    PRIVATE
        # GuiAppData            # If we'd created a binary data target, we'd link to it here
        juce::juce_gui_basics
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

		
# Move built application to bin directory
add_custom_command(TARGET Encore 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:Encore> ${BIN_DIR}/Encore.exe)
