# Minimum required CMake version.
cmake_minimum_required(VERSION 3.10)

# List of source files.
set(SOURCE_FILES
    "Source/Precompiled.hpp"
    "Source/Precompiled.cpp"
    "Source/Application.hpp"
    "Source/Application.cpp"

    "Source/Common/Debug.hpp"
    "Source/Common/Debug.cpp"
    "Source/Common/Math.hpp"
    "Source/Common/Utility.hpp"
    "Source/Common/Utility.cpp"
    "Source/Common/StateMachine.hpp"

    "Source/Types/RuntimeTypes.hpp"
    "Source/Types/RuntimeTypes.cpp"
    "Source/Types/TypeInfo.hpp"
    "Source/Types/TypeInfo.cpp"
    "Source/Types/TypeList.cpp"

    "Source/Serialization/MemoryStream.hpp"
    "Source/Serialization/MemoryStream.cpp"
    "Source/Serialization/Serializable.hpp"
    "Source/Serialization/Serialization.hpp"
    "Source/Serialization/Serialization.cpp"

    "Source/System/Main.cpp"
    "Source/System/Globals.hpp"
    "Source/System/Globals.cpp"
    "Source/System/CommandLine.hpp"
    "Source/System/CommandLine.cpp"
    "Source/System/Logger.hpp"
    "Source/System/Logger.cpp"
    "Source/System/Window.hpp"
    "Source/System/Window.cpp"
    "Source/System/AssetManager.h"
    "Source/System/AssetManager.cpp"
    "Source/System/Console.hpp"
    "Source/System/Console.cpp"

    "Source/Network/Protocol.hpp"
    "Source/Network/Protocol.cpp"
    "Source/Network/Interface/NetworkInterface.hpp"
    "Source/Network/Interface/NetworkBase.hpp"
    "Source/Network/Interface/NetworkBase.cpp"
    "Source/Network/Interface/NetworkOffline.hpp"
    "Source/Network/Interface/NetworkOffline.cpp"
    "Source/Network/Interface/NetworkServer.hpp"
    "Source/Network/Interface/NetworkServer.cpp"
    "Source/Network/Interface/NetworkClient.hpp"
    "Source/Network/Interface/NetworkClient.cpp"
    "Source/Network/Packets/PacketHeader.hpp"
    "Source/Network/Packets/PacketHeader.cpp"
    "Source/Network/Packets/PacketBase.hpp"
    "Source/Network/Packets/PacketBase.cpp"
    "Source/Network/Packets/PacketMessage.hpp"
    "Source/Network/Packets/PacketMessage.cpp"
    "Source/Network/Packets/PacketStateSave.hpp"
    "Source/Network/Packets/PacketStateSave.cpp"
    "Source/Network/Packets/PacketStateUpdate.hpp"
    "Source/Network/Packets/PacketStateUpdate.cpp"

    "Source/Game/GameInstance.hpp"
    "Source/Game/GameInstance.cpp"
    "Source/Game/GameStates/GameStateBase.hpp"
    "Source/Game/GameStates/GameStateBase.cpp"
    "Source/Game/GameStates/GameStateEntry.hpp"
    "Source/Game/GameStates/GameStateEntry.cpp"
    "Source/Game/GameStates/GameStateMainMenu.hpp"
    "Source/Game/GameStates/GameStateMainMenu.cpp"
    "Source/Game/GameStates/GameStateLoading.hpp"
    "Source/Game/GameStates/GameStateLoading.cpp"
    "Source/Game/GameStates/GameStateSession.hpp"
    "Source/Game/GameStates/GameStateSession.cpp"
    "Source/Game/World/Transform.hpp"
    "Source/Game/World/Transform.cpp"
    "Source/Game/World/Handle.hpp"
    "Source/Game/World/Object.hpp"
    "Source/Game/World/Object.cpp"
    "Source/Game/World/World.hpp"
    "Source/Game/World/World.cpp"
    "Source/Game/Objects/Tank.hpp"
    "Source/Game/Objects/Tank.cpp"
    "Source/Game/Objects/Projectile.hpp"
    "Source/Game/Objects/Projectile.cpp"
    "Source/Game/Level.hpp"
    "Source/Game/Level.cpp"
    "Source/Game/PlayerController.hpp"
    "Source/Game/PlayerController.cpp"
    "Source/Game/SnapshotSaveLoad.hpp"
    "Source/Game/SnapshotSaveLoad.cpp"
)

# Create missing source files.
foreach(SOURCE_FILE ${SOURCE_FILES})
    set(SOURCE_FILE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FILE}")

    if(NOT EXISTS ${SOURCE_FILE_PATH})
        file(GENERATE OUTPUT ${SOURCE_FILE_PATH} CONTENT "")
    endif()
endforeach()

# Specify available build configuration types.
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

# Create project target.
project("Tanks")
add_executable("Game" ${SOURCE_FILES})
target_compile_features("Game" PRIVATE cxx_std_17)

# Add source as include directory.
target_include_directories("Game" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source")

# Set binary output directory.
set_target_properties("Game" PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Output")

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
    # Hide console window.
    set_target_properties("Game" PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
    
    # Disable non standard warnings.
    target_compile_definitions("Game" PUBLIC "_CRT_SECURE_NO_WARNINGS")
    target_compile_definitions("Game" PUBLIC "_SCL_SECURE_NO_WARNINGS")

    # Use precompiled headers.
    set_source_files_properties(${SOURCE_FILES} PROPERTIES
        COMPILE_FLAGS "/Yu\"Precompiled.hpp\" /Fp\"${CMAKE_CURRENT_BINARY_DIR}/Source/Precompiled-$(Configuration).pch\""
    )

    set_source_files_properties("Source/Precompiled.cpp" PROPERTIES
        COMPILE_FLAGS "/Yc\"Precompiled.hpp\" /Fp\"${CMAKE_CURRENT_BINARY_DIR}/Source/Precompiled-$(Configuration).pch\""
    )
endif()

# Add JSON library.
target_include_directories("Game" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/External/JSON-3.6.1/include")

# Add SFML library.
set(BUILD_SHARED_LIBS FALSE)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/External/SFML-2.5.1" "External/SFML" EXCLUDE_FROM_ALL)
target_include_directories("Game" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/External/SFML-2.5.1/include")

if(SFML_OS_WINDOWS OR SFML_OS_ANDROID OR SFML_OS_IOS)
    set_property(TARGET "sfml-main" PROPERTY PROJECT_LABEL "SFML-Main")
    set_property(TARGET "sfml-main" PROPERTY FOLDER "External")
    target_link_libraries("Game" "sfml-main")
endif()

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

# Add ImGui library.
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/External/ImGui-1.68" "External/ImGui" EXCLUDE_FROM_ALL)
set_property(TARGET "ImGui" PROPERTY FOLDER "External")
target_link_libraries("Game" "ImGui")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/External/ImGui-1.68/sfml" "External/SFML-ImGui" EXCLUDE_FROM_ALL)
set_property(TARGET "SFML-ImGui" PROPERTY FOLDER "External")
target_link_libraries("Game" "SFML-ImGui")

# Add launcher project.
if(WIN32)
    add_subdirectory("Source/Launcher")
endif()
