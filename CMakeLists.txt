# Minimum required CMake version.
cmake_minimum_required(VERSION 3.10)

# List of source files.
set(SOURCE_FILES
    "Source/Precompiled.hpp"
    "Source/Precompiled.cpp"
    "Source/Main.cpp"
)

# Specify available build configuration types.
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

# Create project target.
project("Tanks")
add_executable("Game" ${SOURCE_FILES})

# Add source as include directory.
target_include_directories("Game" PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Organize structure of source files.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES})

# Set target as default project for Visual Studio.
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "Game")
 
# Set main build directory as the working directory.
set_target_properties("Game" PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Deploy/")

# Use precompiled header for quicker compilation of static headers.
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set_source_files_properties(${SOURCE_FILES} PROPERTIES
        COMPILE_FLAGS "/Yu\"Precompiled.hpp\" /Fp\"${CMAKE_CURRENT_BINARY_DIR}/Source/Precompiled-$(Configuration).pch\""
    )

    set_source_files_properties("Source/Precompiled.cpp" PROPERTIES
        COMPILE_FLAGS "/Yc\"Precompiled.hpp\" /Fp\"${CMAKE_CURRENT_BINARY_DIR}/Source/Precompiled-$(Configuration).pch\""
    )
endif()
