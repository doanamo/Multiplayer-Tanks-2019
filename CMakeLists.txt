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

# Add SFML library.
set(BUILD_SHARED_LIBS FALSE)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/External/SFML-2.5.1" "External/SFML" EXCLUDE_FROM_ALL)
target_include_directories("Game" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/External/SFML-2.5.1/include")

set_property(TARGET "sfml-main" PROPERTY PROJECT_LABEL "SFML-Main")
set_property(TARGET "sfml-main" PROPERTY FOLDER "External")
target_link_libraries("Game" "sfml-main")

set_property(TARGET "sfml-system" PROPERTY PROJECT_LABEL "SFML-System")
set_property(TARGET "sfml-system" PROPERTY FOLDER "External")
target_link_libraries("Game" "sfml-system")

set_property(TARGET "sfml-window" PROPERTY PROJECT_LABEL "SFML-Window")
set_property(TARGET "sfml-window" PROPERTY FOLDER "External")
target_link_libraries("Game" "sfml-window")

set_property(TARGET "sfml-graphics" PROPERTY PROJECT_LABEL "SFML-Graphics")
set_property(TARGET "sfml-graphics" PROPERTY FOLDER "External")
target_link_libraries("Game" "sfml-graphics")

set_property(TARGET "sfml-audio" PROPERTY PROJECT_LABEL "SFML-Audio")
set_property(TARGET "sfml-audio" PROPERTY FOLDER "External")
target_link_libraries("Game" "sfml-audio")

set_property(TARGET "sfml-network" PROPERTY PROJECT_LABEL "SFML-Network")
set_property(TARGET "sfml-network" PROPERTY FOLDER "External")
target_link_libraries("Game" "sfml-network")
